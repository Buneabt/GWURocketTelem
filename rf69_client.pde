#include <SPI.h>
#include <RH_RF69.h>

// Singleton instance of the radio driver
RH_RF69 rf69(8, 7); // Adafruit Feather 32u4

// Constants
const char* CALLSIGN = "YOURCALL"; // Replace with your actual callsign
const unsigned long TRANSMISSION_INTERVAL = 5000; // 5 seconds in milliseconds

// Variables
unsigned long missionStartTime;
unsigned long lastTransmissionTime = 0;

void setup() 
{
  Serial.begin(9600);
  while (!Serial) 
    ;
  delay(250);

  if (!rf69.init())
    Serial.println("init failed");
  
  if (!rf69.setFrequency(433.0))
    Serial.println("setFrequency failed");

  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  //rf69.setTxPower(14, true);

  if(rf69.init() & rf69.setFrequency(433.0))
    Serial.println("Initialized and Freq 433 found.");

  // The encryption key has to be the same as the one in the server
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);

  missionStartTime = millis();
}

void loop()
{
  unsigned long currentTime = millis();
  
  if (currentTime - lastTransmissionTime >= TRANSMISSION_INTERVAL)
  {
    lastTransmissionTime = currentTime;
    
    // Calculate mission time
    unsigned long missionTime = (currentTime - missionStartTime) / 1000; // Convert to seconds
    
    // Prepare the message
    char message[RH_RF69_MAX_MESSAGE_LEN];
    snprintf(message, sizeof(message), "%s+%lu+HelloWorld<EOM>", CALLSIGN, missionTime);
    
    Serial.print("Sending: ");
    Serial.println(message);
    
    rf69.send((uint8_t*)message, strlen(message));
    rf69.waitPacketSent();
    
    Serial.println("Message sent");
  }
}