**Welcome to the GWURocket Telemetry Team**

Our purpose of the SRAD computer is to develop a reliable 
downlink system for real time telemetry.

Action items below:

  [X] Connection Between boards <br/>
  [X] Maintain solid RSSI numbers < 150 dB <br/>
  [X] Read from CSV <br/>
  [X] Attach new antenna to the Client Board (Rocket) <br/>
  [X] Connect with real data collection system to perform compatibility test <br/>

  The attached project code now correctly works when run on two "_Adafruit Feather RP2040 RFM69 Packet Radio_" and a Raspberry Pico 

  https://www.adafruit.com/product/5712

How to run this code:
  - Libraries required:
    - SPI
    - RH_RF95 (Radiohead)

1. Install proper Arduino IDE and Compiler
2. Upload 'Client' code to your desired ground station board.
3. Upload 'Server' to rocket RP2040
4. Upload 'Main' to Raspberry Pico
5. Run Main

Pre - Launch this system trasmitted data, without error, over 1,000 ft. Approx RSSI ~ 120 
