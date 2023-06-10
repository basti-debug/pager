/*

  Pager Code v4
  by basti_debug

  This is script is in charge of:
*  handling incomming traffic from the module 
*  displaying:
*  * right menu (activate/deactivate bluetooth)
*  * main menu (see latest message)
*  handling user input

* v3 
* Handling Bluetooth Actions 

* v4
* send menu 
* basic sending possible

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
int btdataIndex = 0;


// MACROS

// ! you can change this !  but be aware that corresponding programms may not work anymore 

// Clearance macro 
char res = '%'; // if this is send over the Serial2 connection the current message will be cleared 

// end of message macro 
char end = ';';

// bluetooth macro
char blmacro = '$';
bool bllora;

// BUFFERS

// Input LORA Data Buffer
char data; 

// Bluetooth Buffer
char btdata;
char btMAdata; // macro alert buffer






// MENU AREA

// Menu Variables Area
int selectedMenu = 1; // 0 left messages menu, 1 Main Menu , 2 right bluetooth menu

bool bldisco = false;
bool blon = false;
int blstatus = 0; // 0 = error, 1 = connected, 2 = open
String blname = "pager"; //+ random(1,999); // Here you could change the default device name / currently pager with random number 

// Recieve Mark so when true message will be cleared
bool bread;



// Variables needed for settings menu selection
int sc = 1; // cycle
int maxsc = 2;// Number of Entrys ! change if more entrys added ! 
int masxcursor = 3; // Number of Entrys send menu ! change if more entrys in the menu added !


int e1 = 0;
int e2 = 0; 
int e3 = 0;

int cursor = 1;

// Messages 
char* msg1 = "Ahoi";
char* msg2 = "Hello";
char* msg3 = "Im ok";


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

void rdLORA(){
    digitalWrite(32,HIGH); // Traffic LED for testing 
    data = Serial2.read(); 
    
    if (dataIndex < MAX_DATA_LENGTH ) {
      receivedData[dataIndex] = data;
      dataIndex++;
    } 
    
    digitalWrite(32,LOW);
}

void rdBluetooth(){
    btdata = SerialBT.read(); 
    Serial.println(btdata);
    if (btdataIndex < MAX_DATA_LENGTH && isAscii(btdata)) {
      Serial.println(btdata);
      receivedData[btdataIndex] = btdata;
      btdataIndex++;
    } 
}
void overloaddata(){
    if (dataIndex > MAX_DATA_LENGTH || btdataIndex > MAX_DATA_LENGTH || bread == true || data == res || btdata == res) {
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

void receive(){    // handling incomming data from the module and writing to the receivedData variable
  if (Serial2.available()) { // LORA receive
    rdLORA();
    overloaddata();    
  }
  if (bllora == true){    // Bluetooth receive 
    rdBluetooth();
    overloaddata();   
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
  display.println(msg1);
  display.println(msg2);
  display.println(msg3);

  if(cursor==1){
    //first message 
    display.setCursor(100, 32);
    display.setTextColor(SH110X_BLACK, SH110X_WHITE);
    display.print("send");
    //second entry, second msg 
    display.setCursor(100,40);
    display.setTextColor(SH110X_WHITE,SH110X_BLACK);
    display.print("send");
    //third entry, thrid msg
    display.setCursor(100,48);
    display.print("send");
  } 

  if (cursor == 2){
    //first message 
    display.setCursor(100, 32);
    display.print("send");
    //second entry, second msg
    display.setTextColor(SH110X_BLACK, SH110X_WHITE); 
    display.setCursor(100,40);
    display.print("send");
    //third entry, thrid msg
    display.setTextColor(SH110X_WHITE,SH110X_BLACK);
    display.setCursor(100,48);
    display.print("send");
  }

  if (cursor == 3){
     //first message 
    display.setCursor(100, 32);
    display.print("send");
    //second entry, second msg
    display.setCursor(100,40);
    display.print("send");
    //third entry, thrid msg
    display.setTextColor(SH110X_BLACK, SH110X_WHITE); 
    display.setCursor(100,48);
    display.print("send");
    display.setTextColor(SH110X_WHITE,SH110X_BLACK);
  }

  display.display();
}

// Menupage with 4 parameters
// (0-not selected/off, 1-not selected/on, 2-selected/off, 3-selected/on)
// sc == cursor 
// Page also displays name and status of the connection

void menupage(int e1, int e2, int e3, int sc){
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

  // cursor display function

  if(sc==1){
    //first entry, Bluetooth on off toggle
    display.setCursor(100, 16);
    display.setTextColor(SH110X_BLACK,SH110X_WHITE); 
    if(e1==0){
      display.print("off");
    }
    if(e1==1){
      display.print("on");
    }
    display.setTextColor(SH110X_WHITE,SH110X_BLACK);
    
    //second entry, discoverable toggle on off
    display.setCursor(100,32);
    if(e2==0){
      display.print("off");
    }
    if(e2==1){
      display.print("on");
    }

  } 

  if (sc == 2){
    //first entry, Bluetooth on off toggle
    display.setCursor(100, 16);
    if(e1==0){
      display.print("off");
    }
    if(e1==1){
      display.print("on");
    }
    
    //second entry, discoverable toggle on off
    display.setCursor(100,32);
    display.setTextColor(SH110X_BLACK,SH110X_WHITE); 
    if(e2==0){
      display.print("off");
    }
    if(e2==1){
      display.print("on");
    }
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

  delay(250);
  display.begin(i2c_address, true);  // initalize Screen

  display.display(); 
  delay(2000);
  Serial.println(blname +" serial online");

}

void loop() {

  receive();
  

  // vars of the button states
  int rightstate = digitalRead(RIGHT_PIN);
  int leftstate = digitalRead(LEFT_PIN);
  int enterstate = digitalRead(ENTER_PIN);
  int returnstate = digitalRead(RETURN_PIN);
  int topstate = digitalRead(POWER_PIN);
  
// BUTTON Handling

  if(topstate == LOW){  // Bread Button so when pressed notification deleted 
    bread = true;
  }

  if (rightstate == LOW) {  // Scroll Right
    if(selectedMenu >2){}
    else{selectedMenu++;}
  }

  if(leftstate == LOW){   // Scroll Left
    if(selectedMenu < 0){}
    else{selectedMenu--;}
  }

  if(returnstate == LOW){ // Cycle through the Settings options
    if(selectedMenu == 2){  // When Settings Menu selected
      sc++;
    }
    if(selectedMenu == 0){  // When in Send Menu
      cursor++;
    }
    
    if(cursor > masxcursor){
      cursor = 1;
    }

    if (sc > maxsc){
      sc = 1;
    }
    delay(25);
  }
  if(enterstate == LOW){ // Event when enter is pressed - so setting is changed 
    Serial.print("enter");
    if(selectedMenu == 2){  // When Settings Menu selected
      switch (sc){ // switch case where cursor is
      case 1: // Cursor position 1 
        e1++;
        if (e1 == 2){e1 = 0;}
      break;
      case 2: // Cursor position 2
        e2 ++;
        if (e2 == 2){e2= 0;}
      break;
    }
    }
    if(selectedMenu == 0){  // When in send Menu
    Serial.print("send");
      switch (cursor)
      {
      case 1:
        Serial2.println(msg1);
        break;
      case 2:
        Serial2.println(msg2);
        break;	
      case 3:
        Serial2.println(msg3);
        break;
      default:
        break;
      }
    }
    
    

  }

// MENU HANDLING 

  if(selectedMenu == 0){
    sendpage(cursor);
  }
  if(selectedMenu == 1){
    // MAIN PAGE
    mainpage(bread); 
  }
  if(selectedMenu == 2){
    // SETTINGS PAGE
    
    menupage(e1,e2,e3,sc);
  }

  // Bluetooth Handling
  if (SerialBT.available()) {
    btMAdata = SerialBT.read();
  }

  // Reacting to the bluetooth handler
  if(btMAdata == blmacro){
    Serial.println("Bluetooth to LORA activate");
    bllora = true;
  }

// BLUETOOTH STATE HANDLER

  if(e1 == 1){ // Bluetooth ON
    SerialBT.begin(blname); // Start Bluetooth service
  }
  if(e1 == 0){ // Bluetooth off
    SerialBT.end();
  }
  delay(100);
}
