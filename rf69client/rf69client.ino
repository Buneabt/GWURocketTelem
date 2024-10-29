// rf95_client.pde for Adafruit Feather RP2040 RFM
// -*- mode: C++ -*-

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
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  
  // Manual reset the radio
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  Serial.begin(9600);
  while (!Serial) delay(100);  // Wait for serial console
  
  if (!rf95.init()) {
    Serial.println("RF95 init failed");
    while (1);
  }
  Serial.println("RF95 init OK!");

  // Set frequency - must match the server
  if (!rf95.setFrequency(915.0)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  rf95.setTxPower(23, false);  // Set to 23 dBm for maximum range
}

void loop()
{
  Serial.println("Sending to rf95_server");
  digitalWrite(LED, HIGH);
  
  // Send a message to rf95_server
  uint8_t data[] = "Hello World!";
  rf95.send(data, sizeof(data));
  
  rf95.waitPacketSent();
  digitalWrite(LED, LOW);
  
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(3000))
  { 
    if (rf95.recv(buf, &len))
    {
      digitalWrite(LED, HIGH);
      Serial.print("Got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
      digitalWrite(LED, LOW);
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
  else
  {
    Serial.println("No reply, is rf95_server running?");
  }
  
  delay(2000);  // Wait 2 seconds before next transmission
}
