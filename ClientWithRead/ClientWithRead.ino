//Needed libraries
#include <SD.h>
#include <SPI.h>
#include <RH_RF95.h>


//RFM 95 to sd Pinouts
#define RFM95_CS   16
#define RFM95_INT  21
#define RFM95_RST  17
#define SD_CS      10

RH_RF95 rf95(RFM95_CS, RFM95_INT);
File dataFile;

//Setup for both RFM and SD
void setup() {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(9600);
  while (!Serial) delay(100);

  // Reset and initialize radio
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!rf95.init() || !rf95.setFrequency(915.0)) {
    Serial.println("Radio init failed!");
    while (1);
  }
  rf95.setTxPower(20, false);

  if (!SD.begin(SD_CS)) {
    Serial.println("SD card failed!");
    while (1);
  }
}

void loop() {
  dataFile = SD.open("hot_dog_winners.csv"); //This portion of the loop() aims to get the last line of the csv each time
  if (dataFile) {
    String lastLine = "";
    
    // Read through file to get last line
    while (dataFile.available()) {
      String currentLine = dataFile.readStringUntil('\n');
      if (currentLine.length() > 0) {
        lastLine = currentLine;
      }
    }
    dataFile.close();

    if (lastLine.length() > 0) {
      digitalWrite(LED_BUILTIN, HIGH);
      
      // Transmit last line
      uint8_t radioBuffer[RH_RF95_MAX_MESSAGE_LEN]; //Here we transmit the last line of the CSV file
      lastLine.getBytes(radioBuffer, min(lastLine.length() + 1, RH_RF95_MAX_MESSAGE_LEN));
      
      rf95.send(radioBuffer, min(lastLine.length(), RH_RF95_MAX_MESSAGE_LEN));
      rf95.waitPacketSent();
      
      // Wait for reply
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
      
      if (rf95.waitAvailableTimeout(3000)) { //Here we print the reply from the server this is used for debugging
        if (rf95.recv(buf, &len)) {
          Serial.print("Reply: ");
          Serial.println((char*)buf);
        }
      }
      
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
  
  delay(1000); // Wait before next transmission
}