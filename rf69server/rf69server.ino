#include <SPI.h>
#include <RH_RF69.h>

#define RFM69_CS      8
#define RFM69_INT     7
#define RFM69_RST     4
#define LED           13

#define RF69_FREQ 433.0

RH_RF69 rf69(RFM69_CS, RFM69_INT);

void setup() 
{
  Serial.begin(115200);
  while (!Serial) { delay(1); }

  pinMode(LED, OUTPUT);     
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);


  // Manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);
  
  if (!rf69.init()) {
    Serial.println("RFM69 radio init failed");
    while (1);
  }
  Serial.println("RFM69 radio init OK!");
  
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }

  rf69.setTxPower(20, true);

  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");
  Serial.println("Waiting for messages...");
}

void loop() {
  if (rf69.available()) {
    uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    
    if (rf69.recv(buf, &len)) {
      if (!len) return;
      buf[len] = 0; // Null terminate the string
      
      Serial.print("Received: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf69.lastRssi(), DEC);

      // Parse the received message
      char* callsign = strtok((char*)buf, "+");
      char* missionTime = strtok(NULL, "+");
      char* data = strtok(NULL, "<EOM>");

      if (callsign && missionTime && data) {
        Serial.print("Callsign: ");
        Serial.println(callsign);
        Serial.print("Mission Time: ");
        Serial.println(missionTime);
        Serial.print("Data: ");
        Serial.println(data);
      }

      Blink(LED, 40, 3); // Blink LED 3 times, 40ms between blinks
    } else {
      Serial.println("Receive failed");
    }
  }
}

void Blink(byte PIN, byte DELAY_MS, byte loops) {
  for (byte i=0; i<loops; i++)  {
    digitalWrite(PIN,HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN,LOW);
    delay(DELAY_MS);
  }
}