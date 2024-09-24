#include <SPI.h>
#include <RH_RF69.h>

// Singleton instance of the radio driver
RH_RF69 rf69(8, 7); // Adafruit Feather 32u4

void setup() 
{
  Serial.begin(9600);
  while (!Serial) 
    ;
  
  if (!rf69.init())
    Serial.println("RF69 init failed");
  
  if (!rf69.setFrequency(433.0))
    Serial.println("setFrequency failed");

  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  //rf69.setTxPower(14, true);

  // The encryption key has to be the same as the one in the client
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);
  
  if(rf69.init() & rf69.setFrequency(433.0))
    Serial.println("RF69 initialized and Freq 433 found.");

  Serial.println("Timestamp,Callsign,Mission Time,Data");
  Serial.println("Listening for messages...");
}

void loop()
{
  if (rf69.available())
  {
    uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf69.recv(buf, &len))
    {
      // Null-terminate the received data
      buf[len] = '\0';
      
      // Get current timestamp
      unsigned long currentTime = millis();
      
      // Print formatted data to Serial
      Serial.print(currentTime);
      Serial.print(",");
      Serial.println((char*)buf);
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
}