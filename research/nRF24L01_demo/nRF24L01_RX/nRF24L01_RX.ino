/**
* @brief: A simple nRF24L01 receiver example using an Arduino Nano.
* @file: nRF24L01_RX.ino
*
* @author: jkieltyka15
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";


void setup() {

  Serial.begin(9600);

  // begin the radio
  if(false == radio.begin()) {
    Serial.println("*ERROR* failed to start the radio");
  }

  // configure radio
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  Serial.println("setup complete");
}


void loop() {

  bool is_available_prev = true;

  for (uint32_t i = 1;;) {

    bool is_available = radio.available();

    // message received
    if (true == is_available) {

      char text[32] = "";
      radio.read(&text, sizeof(text));
      Serial.print("messaged received: ");
      Serial.println(text);
    }

    // waiting to receive message
    else if (true == is_available_prev) {
    
      Serial.print("waiting for message [");
      Serial.print(i++);
      Serial.println("]...");
    }

    is_available_prev = is_available;

    delay(10);
  }
}
