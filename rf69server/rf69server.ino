#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS      8
#define RFM95_INT     7
#define RFM95_RST     4
#define LED           13

#define RF95_FREQ 915

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() 
{
  Serial.begin(115200);
  while (!Serial) { delay(1); }

  pinMode(LED, OUTPUT);     
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, LOW);


  // Manual reset
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  
  if (!rf95.init()) {
    Serial.println("RFM95 radio init failed");
    while (1);
  }
  Serial.println("RFM95 radio init OK!");
  
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
  }

  rf95.setTxPower(20, true);

  Serial.print("RFM95 radio @");  Serial.print((int)RF95_FREQ);  Serial.println(" MHz");
  Serial.println("Waiting for messages...");
}

void loop() {
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len)) {
      if (!len) return;
      buf[len] = 0; // Null terminate the string
      
      Serial.print("Received: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);

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
