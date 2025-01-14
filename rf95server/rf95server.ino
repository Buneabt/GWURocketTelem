#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS   16
#define RFM95_INT  21
#define RFM95_RST  17
<<<<<<< HEAD

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
=======
#define RX_PIN     1
#define TX_PIN     0

RH_RF95 rf95(RFM95_CS, RFM95_INT);
String inputString = "";
boolean stringComplete = false;

void setup() {
  pinMode(RFM95_RST, OUTPUT);
  pinMode(RFM95_CS, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  digitalWrite(RFM95_CS, HIGH);
  digitalWrite(RFM95_RST, HIGH);
  
  Serial.begin(9600);
  delay(1000);
  
  digitalWrite(RFM95_RST, LOW);
  delay(100);
  digitalWrite(RFM95_RST, HIGH);
  delay(100);

  SPI.begin();
  
  if (!rf95.init() || !rf95.setFrequency(915.0)) {
    Serial.println("Fail");
    while(1) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(100);
    }
  }

  rf95.setTxPower(20, false);
  Serial1.begin(9600);
  inputString.reserve(200);
}

void loop() {
  while (Serial1.available()) {
    char inChar = (char)Serial1.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }

  if (stringComplete) {
    digitalWrite(LED_BUILTIN, HIGH);
    inputString.trim();
    
    if (inputString.length() > 0) {
      uint8_t radioBuffer[RH_RF95_MAX_MESSAGE_LEN];
      inputString.getBytes(radioBuffer, min(inputString.length() + 1, RH_RF95_MAX_MESSAGE_LEN));
      rf95.send(radioBuffer, min(inputString.length(), RH_RF95_MAX_MESSAGE_LEN));
      rf95.waitPacketSent();
    }
    
    inputString = "";
    stringComplete = false;
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  delay(10);
}
>>>>>>> 03f7627ffd4e02408355441b067dca90b71772ad
