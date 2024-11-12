/**
* @brief: A simple nRF24L01 transmitter example using an Arduino Nano.
* @file: nRF24L01_TX.ino
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
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();

  Serial.println("setup complete");
}


void loop() {

  const char text[] = "Hello World";

  bool is_sent_prev = true;

  for (uint32_t i = 1;;) {

    // new message will attempt to be sent
    if (true == is_sent_prev) {

      Serial.print("sending message [");
      Serial.print(i);
      Serial.println("]...");
    }

    // send the message
    bool is_sent = radio.write(&text, sizeof(text));

    // message was successfully sent
    if (true == is_sent) {

      Serial.print("message [");
      Serial.print(i++);
      Serial.println("] sent");
    }

    is_sent_prev = is_sent;

    delay(1000);
  }
}
