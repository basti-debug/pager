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
#include <Wire.h> // IIC 
// Screen
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// Bluetooth

#include "BluetoothSerial.h"

#include "Arduino.h"



// Screen Address
#define i2c_address 0x3c

// Screen Dimentions
#define screen_width 128
#define screen_height 64

// Initalisation of the Screen with the Adafruit library
#define oled_reset -1
Adafruit_SH1106G display = Adafruit_SH1106G(screen_width, screen_height, &Wire, oled_reset);

// define pins
#define BUZZER_PIN 18
#define RIGHT_PIN 14
#define ENTER_PIN 27
#define LEFT_PIN 25
#define RETURN_PIN 26 
#define POWER_PIN 33

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define MAX_DATA_LENGTH 60

char receivedData[MAX_DATA_LENGTH];
int dataIndex = 0;

char data; 

// Menu Variables Area
int selectedMenu = 1; // 0 left messages menu, 1 Main Menu , 2 right bluetooth menu

bool bldisco = false;
bool blon = false;
int blstatus = 0; // 0 = error, 1 = connected, 2 = open
String blname = "pager"; //+ random(1,999); // Here you could change the default device name / currently pager with random number 

// Recieve Mark
bool bread;

// Clearance macro ! you can change this ! 
char res = '%'; // if this is send over the Serial2 connection the current message will be cleared 

// end of message macro 
char end = ';';


// Variables needed for settings menu selection
int sc = 0; // cycle
int maxsc = 4;// Number of Entrys ! change if more entrys added ! 

int e1 = 1;
int e2 = 0; 
int e3 = 0;

BluetoothSerial SerialBT;


// Functions 

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
    
    if (dataIndex < MAX_DATA_LENGTH ) {
      receivedData[dataIndex] = data;
      dataIndex++;
    } 
    
    digitalWrite(32,LOW);
  }
  if (dataIndex > MAX_DATA_LENGTH || bread == true || data == res) {
      Serial.println("buffer cleared");
      for (int i = 0; i < MAX_DATA_LENGTH; i++) {
        receivedData[i] = 0;
      }
      dataIndex = 0;
      bread = false;
      data = 0;
      delay(10);
    }
}

// PAGES 

// Display the Main Page, automaticly clears the screen so cant be used when displaying additionally
void mainpage(bool read){
  display.clearDisplay();

// Heading

  display.setTextSize(1); 
  display.setTextColor(SH110X_WHITE); 
  display.setCursor(0,0);
  display.println("home page");
  display.println("");
// Info  
  display.println("Last Message:");
  display.println("");
  
  display.println(receivedData);

  

  display.display();
}

// Send Page
void sendpage(int cursor){
  display.clearDisplay();

  display.setTextSize(1); 
  display.setTextColor(SH110X_WHITE); 
  display.setCursor(0,0);
  display.println("send page");

  display.println("");
  display.println("- Quick Sender -");
  display.println("");
  display.println("Hi");
  display.println("Bye");
  display.println("Simon");
 

  if(cursor == 0){
    //notselected 1 
    display.setCursor(100,16);
    display.print("send");
  }
  if(cursor == 1){
    //selected 1
    display.setCursor(100,16);
    display.setTextColor(SH110X_BLACK,SH110X_WHITE); 
    display.print("send");
    display.setTextColor(SH110X_WHITE,SH110X_BLACK); 
  }



  if(cursor == 2){
    //notseleceted 2   
    display.setCursor(100,32);
    display.print("send");
    
  }
  if(cursor == 3){
    //selected 2
    display.setCursor(100,32);
    display.setTextColor(SH110X_BLACK,SH110X_WHITE); 
    display.print("send");
    display.setTextColor(SH110X_WHITE,SH110X_BLACK); 
  }
  if(cursor == 4){
    //notselected 3
    display.setCursor(100,50);
    display.print("send");
  }
  if(cursor == 5){
    //selcted 3
    display.setCursor(100,50);
    display.setTextColor(SH110X_BLACK,SH110X_WHITE); 
    display.print("send");
    display.setTextColor(SH110X_WHITE,SH110X_BLACK); 
  }


  display.display();
}

// Menupage with 4 parameters
// (0-not selected/off, 1-not selected/on, 2-selected/off, 3-selected/on)
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




// Setup

void setup() {

  Serial.print(blname);

  // pinmodes

  pinMode(ENTER_PIN, INPUT_PULLUP); // Enter Button
  pinMode(LEFT_PIN, INPUT_PULLUP); // Left Button
  pinMode(RIGHT_PIN, INPUT_PULLUP); // Right Button
  pinMode(RETURN_PIN, INPUT_PULLUP); // Return Button
  pinMode(POWER_PIN, INPUT_PULLUP); // Top Button

  pinMode(32,OUTPUT); //LED
  pinMode(18,OUTPUT); //Buzzer


  Serial.begin(9600); // Start Serial intern / only TESTING
  Serial2.begin(9600); // Start LORA   
  SerialBT.begin(blname); // Start Bluetooth service

  delay(250);
  display.begin(i2c_address, true);  // initalize Screen

  display.display(); 
  delay(2000);
  Serial.println(blname +" serial online");
  SerialBT.print(blname + " online");
}

void loop() {

  lorarecive();
  

  // vars of the button states
  int rightstate = digitalRead(RIGHT_PIN);
  int leftstate = digitalRead(LEFT_PIN);
  int enterstate = digitalRead(ENTER_PIN);
  int returnstate = digitalRead(RETURN_PIN);
  int topstate = digitalRead(POWER_PIN);
  
  if(topstate == LOW){
    bread = true;
  }

  if (rightstate == LOW) {
    if(selectedMenu >2){}
    else{selectedMenu++;}
  }

  if(leftstate == LOW){
    if(selectedMenu < 0){}
    else{selectedMenu--;}
  }

  if(selectedMenu == 0){
    sendpage(0);
  }
  if(selectedMenu == 1){
    // MAIN PAGE
    mainpage(bread); 
  }
  if(selectedMenu == 2){
    // SETTINGS PAGE
    if(sc==1){
      e1 + 2;
      delay(100);
      e1 - 2;
    }
    if(sc==2){
      e2 + 2;
      delay(100);
      e2 - 1;
    }
    if(sc==3){
      e3 + 2;
      delay(100);
      e3 - 1;
    }
   
    menupage(e1,e2,e3);
  }


  if(returnstate == LOW){
    if (sc <= maxsc){
      Serial.print("ok");
      sc = sc +1;
    }
    else {sc = 0; Serial.print("aua");};
  }
  if(enterstate == LOW){
    
  }

  delay(100);
}
