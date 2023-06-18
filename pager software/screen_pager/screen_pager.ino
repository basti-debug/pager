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

* v5 
* fixed send menu 
* added Led support

* v6
* bluetooth handling added

*/


// needed Dependencys

#pragma region Dependencys

#include <Wire.h> // IIC 
// Screen
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// Bluetooth

#include "BluetoothSerial.h"

#include "Arduino.h"

#pragma endregion

#pragma region Macros
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

#define bleLED 4
#define notifyLED 12
#define batterylowLED 12

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define MAX_DATA_LENGTH 60

#pragma endregion

#pragma region Global Variables

char receivedData[MAX_DATA_LENGTH];
int dataIndex = 0;
int btdataIndex = 0;


// MACROS

// ! you can change this !  but be aware that corresponding programms may not work anymore 

// Clearance macro 
char res = '%'; // if this is send over the Serial2 connection the current message will be cleared 

// end of message macro 
char end = ';';

// Alert Message Macro 
char alertmacro = '|';

// bluetooth macro
char blmacro = '$';
bool bllora;

char blloopmacro = '~';
bool blloop; 
// BUFFERS

// Input LORA Data Buffer
char data; 

// Bluetooth Buffer
char btdata;
char btMAdata; // macro alert buffer

// Bluetooth forwording buffer
char rrDBuffer[MAX_DATA_LENGTH]; 

// Variable which helps that messages are not send everytime over the Bluetooth line
int comp;

// MELODY FOR NOTIFICATION

int melody[] = {200,200,100,500,500,100,500,200};
int durations[] = {100, 100, 100, 100, 100, 100, 100, 100};




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
int masxcursor = 4; // Number of Entrys send menu ! change if more entrys in the menu added !


int e1 = 0;
int e2 = 0; 
int e3 = 0;

int cursor = 1;

// Messages 
String msg1 = "Ahoi";
String msg2 = "Hello";
String msg3 = "Im ok";


BluetoothSerial SerialBT;

#pragma endregion

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

void sendDataLORA(String data){ // send Data over LORA
  Serial2.print(data);
}

void rdLORA(){
    digitalWrite(batterylowLED,HIGH); // Traffic LED for testing 
    data = Serial2.read(); 
    
    if (dataIndex < MAX_DATA_LENGTH ) {
      receivedData[dataIndex] = data;
      dataIndex++;
    } 
    
    digitalWrite(batterylowLED,LOW);
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

void blloopfunction(){
  comp = memcmp(rrDBuffer,receivedData,sizeof(rrDBuffer));
  if (comp != 0)
  {
    SerialBT.println(receivedData);
    strcpy(rrDBuffer, receivedData);
  }
}
void overloaddata(){
    if (dataIndex > MAX_DATA_LENGTH || btdataIndex > MAX_DATA_LENGTH || bread == true || data == res || btdata == res) {
      Serial.println("buffer cleared");
      for (int i = 0; i < MAX_DATA_LENGTH; i++) {
        receivedData[i] = 0;
      }
      for(int i = 0; i < MAX_DATA_LENGTH; i++){
        rrDBuffer[i]=0;
      }
      dataIndex = 0;
      bread = false;
      digitalWrite(notifyLED,LOW);
      data = 0;
      delay(10);
    }
}

void handleAlerts(){
  if (data == alertmacro){
    digitalWrite(notifyLED,HIGH);
    for (int i = 0; i < sizeof(melody)/sizeof(int); i++) {
    tone(BUZZER_PIN, melody[i]);
    delay(durations[i]);
    noTone(BUZZER_PIN);
    delay(100);
  }
  }
}

void receive(){    // handling incomming data from the module and writing to the receivedData variable
  overloaddata();
  if (Serial2.available()) { // LORA receive
    rdLORA();
    handleAlerts();
  }
  if (bllora == true){    // Bluetooth receive 
    rdBluetooth();  
  }
  if(blloop == true){
    blloopfunction();
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
  display.setTextColor(SH110X_BLACK, SH110X_WHITE);
  display.println("ALERT");
  display.setTextColor(SH110X_WHITE,SH110X_BLACK);

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
    //fourth entry
    display.setCursor(100,56);
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
    //fourth entry
    display.setCursor(100,56);
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
    //fourth entry
    display.setCursor(100,56);
    display.print("send");
  }
  if (cursor == 4){
     //first message 
    display.setCursor(100, 32);
    display.print("send");
    //second entry, second msg
    display.setCursor(100,40);
    display.print("send");
    //third entry, thrid msg
    display.setCursor(100,48);
    display.print("send");
    //fourth entry
    display.setTextColor(SH110X_BLACK, SH110X_WHITE); 
    display.setCursor(100,56);
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
    display.setCursor(90,50);
    display.print("connected");
  }
  if(blstatus == 2)
  {
    display.setCursor(90,50);
    display.print("open");
  }
  if(blstatus == 3)
  {
    display.setCursor(90,50);
    display.print("closed");
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

  pinMode(bleLED,OUTPUT); //Bluetooth LED
  pinMode(notifyLED, OUTPUT); // Notification LED 
  pinMode(batterylowLED,OUTPUT); // ERROR / Battery LED
  pinMode(BUZZER_PIN,OUTPUT); //Buzzer


  Serial.begin(9600); // Start Serial intern / only TESTING
  Serial2.begin(9600); // Start LORA   

  delay(250);
  display.begin(i2c_address, true);  // initalize Screen

  display.display(); 
  delay(2000);
  Serial.println(blname +" serial online");

}

void loop() {

  receive(); // This Function Handles all the incomming Data, also place of message manipluation
  
 
  // BUTTON Handling

  if(digitalRead(POWER_PIN) == LOW){  // Bread Button so when pressed notification deleted 
    bread = true;
  }

  if (digitalRead(RIGHT_PIN) == LOW) {  // Scroll Right
    if(selectedMenu >2){}
    else{selectedMenu++;}
  }

  if(digitalRead(LEFT_PIN) == LOW){   // Scroll Left
    if(selectedMenu < 0){}
    else{selectedMenu--;}
  }

#pragma region Cycle Button pressed Event

  if(digitalRead(RETURN_PIN) == LOW){ // Cycle through the Settings options
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
#pragma endregion

  // EVENT ENTER
#pragma region ENTER Pressed
  // Event when enter is pressed - so setting is changed 
  if(digitalRead(ENTER_PIN) == LOW){ 
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
    Serial.print(cursor);
      switch (cursor)
      {
      case 1:
        sendDataLORA(msg1);
        break;
      case 2:
        sendDataLORA(msg2);
        break;	
      case 3:
        sendDataLORA(msg3);
        break;
      case 4: 
        sendDataLORA(String(alertmacro));
        break;
      default:
        break;
      }
    }
    
    

  }
#pragma endregion
// MENU HANDLING 
#pragma region Menu Area when which Menu is selected which action should be taken
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

  if(btMAdata == blloopmacro){
    //Serial.println("Internal Bluetooth Forwording from LORA to Bluetooth active");
    blloop = true;
  }
#pragma endregion

#pragma region Bluetooth Handling ON OFF
// BLUETOOTH STATE HANDLER

  if(e1 == 1){ // Bluetooth ON
    SerialBT.begin(blname); // Start Bluetooth service
    digitalWrite(bleLED, HIGH);
    blstatus = 2; 
  }
  if(e1 == 0){ // Bluetooth off
    SerialBT.end();
    digitalWrite(bleLED, LOW);
    blstatus = 3;
  }

#pragma endregion

  delay(100);
}
