#include <SPI.h>
#include <RH_RF69.h>

#define RFM69_CS      8
#define RFM69_INT     7
#define RFM69_RST     4
#define LED           13

#define RF69_FREQ 433.0

RH_RF69 rf69(RFM69_CS, RFM69_INT);

const char* CALLSIGN = "KQ4NPQ";
unsigned long missionStartTime;
unsigned long lastTransmissionTime = 0;
const unsigned long TRANSMISSION_INTERVAL = 5000; // 5 seconds

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

  rf69.setTxPower(20, true);  // Range from 14-20 for power, 2nd arg must be true for 69HCW

  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");

  missionStartTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastTransmissionTime >= TRANSMISSION_INTERVAL) {
    lastTransmissionTime = currentTime;
    
    unsigned long missionTime = (currentTime - missionStartTime) / 1000; // Convert to seconds
    
    String data = "Test Data"; // Replace with actual data collection function

    char message[RH_RF69_MAX_MESSAGE_LEN];
    snprintf(message, sizeof(message), "%s+%lu+%s<EOM>", CALLSIGN, missionTime, data.c_str());
    
    Serial.print("Sending: ");
    Serial.println(message);
    
    rf69.send((uint8_t*)message, strlen(message));
    rf69.waitPacketSent();
    
    Serial.println("Message sent");

    Blink(LED, 50, 3); // Blink LED 3 times, 50ms between blinks
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