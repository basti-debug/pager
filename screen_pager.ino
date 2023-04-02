/*

  Pager Code v1
  by basti_debug

  This is script is in charge of:
*  handling incomming traffic from the module 
*  displaying send menu 
*  handling user input
*  handling usb c input regarding  


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

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define MAX_DATA_LENGTH 50

char receivedData[MAX_DATA_LENGTH];
int dataIndex = 0;

char data;

void setup() {

  Serial.begin(9600);
  delay(250);
  display.begin(i2c_address, true); // sending address

  display.display();
  delay(2000);

  display.clearDisplay();

  display.setTextSize(1);               // Text Size
  display.setTextColor(SH110X_WHITE);   // Text Color
  display.setCursor(0, 0);              // set the Cursor
  display.println("Offline           ..."); 
  display.println("Last Message: ");           
  display.println("");
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.println("null");

  display.println("");
  display.setTextSize(0.3);
  display.setTextColor(SH110X_WHITE);
  display.println("Press X for Menu");

  display.display();

  Serial2.begin(9600);
  Serial2.println("pager 1 online");


}

void loop() {
   if (Serial2.available()) {
    data = Serial2.read();


    if (dataIndex < MAX_DATA_LENGTH) {
      receivedData[dataIndex] = data;
      dataIndex++;
    }
  }
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    display.print(receivedData);
    display.display();
  
}
