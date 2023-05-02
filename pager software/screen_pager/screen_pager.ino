/*

  Pager Code v1
  by basti_debug

  This is script is in charge of:
*  handling incomming traffic from the module 
*  displaying send menu 
*  handling user input


*/


// needed Dependencys
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "Arduino.h"


// Screen Address
#define i2c_address 0x3c

// Screen Dimentions
#define screen_width 128
#define screen_height 64

// Initalisation of the Screen with the Adafruit library
#define oled_reset -1
Adafruit_SH1106G display = Adafruit_SH1106G(screen_width, screen_height, &Wire, oled_reset);

// Buzzer Pin
#define BUZZER_PIN 18

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define MAX_DATA_LENGTH 20

char receivedData[MAX_DATA_LENGTH];
int dataIndex = 0;

char data;

void setup() {

  pinMode(27, INPUT_PULLUP); // Enter Button
  pinMode(32,OUTPUT); //LED
  pinMode(18,OUTPUT);
  Serial.begin(9600);

  delay(250);
  display.begin(i2c_address, true);  // sending address

  display.display();
  delay(2000);

  Serial2.begin(9600);

  Serial.println("ready steady go");
}

void loop() {

  // Melody
  int melody[] = {200,200,100,500,500,100,500,200};
  int durations[] = {100, 100, 100, 100, 100, 100, 100, 100};

  int buttonState = digitalRead(27);
  if (buttonState == LOW) {
    Serial2.print(".-.");
    Serial.print("pressed");
  } 

  if (Serial2.available()) {
    digitalWrite(32,HIGH);
    data = Serial2.read();

    for (int i = 0; i < sizeof(melody)/sizeof(int); i++) {
    tone(BUZZER_PIN, melody[i]);
    delay(durations[i]);
    noTone(BUZZER_PIN);
    delay(100);
  }

    if (dataIndex < MAX_DATA_LENGTH) {
      receivedData[dataIndex] = data;
      dataIndex++;
    } else {
      for (int i = 0; i <= MAX_DATA_LENGTH; i++) {
        receivedData[i] = 0;
      }
      dataIndex = 0;
    }
    digitalWrite(32,LOW);
  }


  display.clearDisplay();

  display.setTextSize(1);              // Text Size
  display.setTextColor(SH110X_WHITE);  // Text Color
  display.setCursor(0, 0);             // set the Cursor
  display.println("Online            ...");
  display.println("Last Message: ");
  display.println("");
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);

  display.setTextColor(SH110X_WHITE);
  display.print(receivedData);



  display.display();
  

  delay(10);
}
