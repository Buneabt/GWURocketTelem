// rf95_server.pde for Adafruit Feather RP2040 RFM
#include <SPI.h>
#include <RH_RF95.h>

// Pin definitions for Feather RP2040 RFM
#define RFM95_CS   16    // "B" pin on RFM boards
#define RFM95_INT  21    // "A" pin on RFM boards
#define RFM95_RST  17    // "C" pin on RFM boards
#define LED        13    // Built-in LED

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() 
{
  // Set up GPIO pins
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  
  // Manual reset the radio
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  Serial.begin(9600);
  while (!Serial) delay(100); // Wait for serial port to be available
  
  if (!rf95.init())
    Serial.println("init failed");
  Serial.println("init success");
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // Set frequency to match your region (915.0 for US)
  if (!rf95.setFrequency(915.0)) {
    Serial.println("setFrequency failed");
  }

  // The default transmitter power is 13dBm, using PA_BOOST.
  // Set transmitter power to 20 dBm, using PA_BOOST
  rf95.setTxPower(20, false);
}

void loop()
{
  if (rf95.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
      digitalWrite(LED, HIGH);  // Turn on LED to show received packet
      Serial.print("got request: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
      
      // Send a reply
      uint8_t data[] = "And hello back to you";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
      digitalWrite(LED, LOW);  // Turn off LED
    }
    else
    {
      Serial.println("recv failed");
    }
  }
}
