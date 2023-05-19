/*

  Pager Code v2
  by basti_debug

  This is script is in charge of:
*  handling incomming traffic from the module 
*  displaying:
*  * right menu (activate/deactivate bluetooth, devices etc?)
*  * main menu (see latest message)
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

// Menu Variables Area
int selectedMenu = 0; // 0 Main Menu, 1 right bluetooth menu, 2 left messages menu

bool bldisco = false;
bool blon = false;
int blstatus = 0; // 0 = error, 1 = connected, 2 = open
char blname[] = "pager"; // Here you could change the default device name

/* 
  DUMP AREA

  int buttonState = digitalRead(27);
  if (buttonState == LOW) {


  } 

*/

void Snotifcation(){
  // Melody
  int melody[] = {200,200,100,500,500,100,500,200};
  int durations[] = {100, 100, 100, 100, 100, 100, 100, 100};

  for (int i = 0; i < sizeof(melody)/sizeof(int); i++) {
    tone(BUZZER_PIN, melody[i]);
    delay(durations[i]);
    noTone(BUZZER_PIN);
    delay(100);
    }
}

void sendDataLORA(char data){ // send Data over LORA
  Serial2.print(data);
}

void lorarecive(){    // handling incomming data from the module and writing to the receivedData variable
 if (Serial2.available()) {
   
    digitalWrite(32,HIGH); // Traffic LED for testing 
    data = Serial2.read(); 

    if (dataIndex < MAX_DATA_LENGTH) {
      receivedData[dataIndex] = data;
      dataIndex++;
    } 
    else {
      Serial.println("clearing ongoing");
      for (int i = 0; i <= MAX_DATA_LENGTH; i++) {
        receivedData[i] = 0;
      }
      dataIndex = 0;
      delay(10);
    }
    digitalWrite(32,LOW);

    Serial.println(receivedData);
  }
}



// Display the Main Page, automaticly clears the screen so cant be used when displaying additionally
void mainpage(){
  display.clearDisplay();

// Heading

  display.setTextSize(1); 
  display.setTextColor(SH110X_WHITE); 
  display.setCursor(0,0);
  display.println("home page");
  
// Info  
  display.println("Last Message:");
  display.println("");
  
  display.println(receivedData);

  

  display.display();
}

// Menupage with 4 parameters
// (0-not selected/off, 1-not selected/on, 2-selected/off, 3-selected/on)
// blname == Name of device 
void menupage(int e1, int e2, int e3){
  display.clearDisplay();

  display.setTextSize(1); 
  display.setTextColor(SH110X_WHITE); 
  display.setCursor(0,0);
  display.println("connection settings");

  display.println("");
  display.println("- Bluetooth -");
  display.println("");
  display.println("discoverable");
  display.println("name");
  display.println("status");



  // BLonoff AREA

  if(e2 == 0){
    // 0-not selected/off
    display.setCursor(100,16);
    display.print("off");
  }
  if(e2 == 1){
    // 1-not selected/on
    display.setCursor(100,16);
    display.print("on");
  }
  if(e2 == 2){
    // 2-selected/off
    display.setCursor(100,16);
    display.setTextColor(SH110X_BLACK,SH110X_WHITE); 
    display.print("off");
    display.setTextColor(SH110X_WHITE,SH110X_BLACK); 
  }
  if(e2 == 3){
    // 3-selected/on
    display.setCursor(100,16);
    display.setTextColor(SH110X_BLACK,SH110X_WHITE); 
    display.print("on");
    display.setTextColor(SH110X_WHITE,SH110X_BLACK); 
  }


  // BLDiscoverable AREA

  if(e1 == 0){
    // 0-not selected/off
    display.setCursor(100,32);
    display.print("off");
  }
  if(e1 == 1){
    // 1-not selected/on
    display.setCursor(100,32);
    display.print("on");
  }
  if(e1 == 2){
    // 2-selected/off
    display.setCursor(100,32);
    display.setTextColor(SH110X_BLACK,SH110X_WHITE); 

    display.print("off");
    display.setTextColor(SH110X_WHITE,SH110X_BLACK); 
  }
  if(e1 == 3){
    // 3-selected/on
    display.setCursor(100,32);
    display.setTextColor(SH110X_BLACK,SH110X_WHITE); 
    display.print("on");
    display.setTextColor(SH110X_WHITE,SH110X_BLACK); 
  }



  Serial.println(blstatus);
  Serial.println(blname);


  // Display the Name
  display.setCursor(80,42);
  display.print(blname);

  // Display Status

  if(blstatus == 0)
  {
    display.setCursor(90,50);
    display.print("error");
  }
  if(blstatus == 1)
  {
    display.setCursor(100,64);
    display.print("connected");
  }
  if(blstatus == 2)
  {
    display.setCursor(100,64);
    display.print("open");
  }

  display.display();
}






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

  lorarecive();
  
  menupage(0,2,0);
  //mainpage();  

  delay(5);
}
