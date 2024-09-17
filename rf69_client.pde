// rf69_client.pde
// -*- mode: C++ -*-

#include <SPI.h>
#include <RH_RF69.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// Singleton instance of the radio driver
// RH_RF69 rf69;
//RH_RF69 rf69(15, 16); // For RF69 on PJRC breakout board with Teensy 3.1
//RH_RF69 rf69(4, 2); // For MoteinoMEGA https://lowpowerlab.com/shop/moteinomega
RH_RF69 rf69(8, 7); // Adafruit Feather 32u4



Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire); //oled board

void setup() 
{
  Serial.begin(9600);
  while (!Serial) 
    ;
  delay(250);
  display.begin(0x3C, true);

  display.display(); //loads starting screen and then clears it
  delay(1000);
  display.clearDisplay();
  display.display();

  display.setTextSize(1); //Sets up text settings
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);

  if (!rf69.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  if (!rf69.setFrequency(433.0))
    Serial.println("setFrequency failed");
  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  //rf69.setTxPower(14, true);

  if(rf69.init() & rf69.setFrequency(433.0))
    Serial.println("Initialized and Freq 433 found.C");

  // The encryption key has to be the same as the one in the server
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);

}


void loop()
{
  display.clearDisplay();
  display.display();
  delay(1000);
  display.setTextSize(1); //Sets up text settings
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  display.println("Sending to rf69\nserver");

  // Send a message to rf69_server

  //New stuff might change back

  uint8_t data[] = "Hello World!";
  rf69.send(data, sizeof(data));
  
  rf69.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf69.waitAvailableTimeout(500))
  { 
    // Should be a reply message for us now   
    if (rf69.recv(buf, &len))
    {
      display.print("got reply: \n");
      display.println((char*)buf);
    }
    else
    {
      display.println("recv failed");
    }
  }
  else
  {
    display.println("\nNo reply,\nis rf69_server running?");
  }
  display.display();
  delay(3000);
}
