#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS   16
#define RFM95_INT  21
#define RFM95_RST  17

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  Serial.begin(9600);
  delay(3000);
  Serial.println("Starting initialization...");

  pinMode(RFM95_RST, OUTPUT);
  pinMode(RFM95_CS, OUTPUT);
  digitalWrite(RFM95_CS, HIGH);

  SPI.begin();
  delay(100);

  // Reset sequence
  digitalWrite(RFM95_RST, HIGH);
  delay(150);
  digitalWrite(RFM95_RST, LOW);
  delay(150);
  digitalWrite(RFM95_RST, HIGH);
  delay(150);

  if (!rf95.init() || !rf95.setFrequency(915.0)) {
    Serial.println("Radio init failed!");
    while(1);
  }

  // Configure radio
  rf95.setTxPower(20, false);

  Serial.println("Initialization complete - Ready for messages");
}

void loop() {
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)) {
      buf[len] = 0;
      Serial.print("Data: ");
      Serial.print((char*)buf);
      Serial.print(", ");
      Serial.println(rf95.lastRssi(), DEC);
    }
  }
  delay(10);
}