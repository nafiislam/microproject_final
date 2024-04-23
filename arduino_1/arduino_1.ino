#include <MFRC522.h>
#include <HCSR04.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Keypad_I2C.h> // I2C Keypad library by Joe Young https://github.com/joeyoung/arduino_keypads
#include <LiquidCrystal_I2C.h>  // I2C LCD Library by Francisco Malpartida https://github.com/fmalpartida/New-LiquidCrystal.git

#define lcd_addr 0x27     // I2C address of typical I2C LCD Backpack
#define keypad_addr 0x20  // I2C address of I2C Expander module (A0-A1-A2 dip switch to off position)

// LCD Pins to I2C LCD Backpack - These are default for HD44780 LCD's
#define Rs_pin 0
#define Rw_pin 1
#define En_pin 2
#define BACKLIGHT_PIN 3
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7


MFRC522 mfrc522(10, 9); 
// Create instance for LCD called: i2c_lcd
LiquidCrystal_I2C i2c_lcd(lcd_addr,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

// Define the keypad pins
const byte ROWS = 4; 
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Keypad pins connected to the I2C-Expander pins P0-P6
byte rowPins[ROWS] = {0, 1, 2, 3}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7}; // connect to the column pinouts of the keypad

// Create instance of the Keypad name I2C_Keypad and using the PCF8574 chip
Keypad_I2C I2C_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS, keypad_addr, PCF8574 );


UltraSonicDistanceSensor distanceSensor(7,8);// Initialize sensor that uses digital pins 7 and 8.
SoftwareSerial ArduinoUno(2, 3);
bool obstacleDetected=true, passwordCorrect = false, rfidCorrect = false,imageCorrect = false, voiceCorrect = false,fingerprint=false;

String inputPassword="";
char key;
char c;
String ch;
bool isClear = false;
int tryCount=0;
int ldr;
int itr;

void func(){
  c=ArduinoUno.read();
  while(c=='\r'||c=='\n'){
    c=ArduinoUno.read();
  }
  while(ArduinoUno.available()==0);
      c=ArduinoUno.read();
}

void setup() {
  ArduinoUno.begin(9600);

  Serial.begin(9600);
  
  lcd_keypad_setup();
  pinMode(6,OUTPUT); //Door lock pin
  digitalWrite(6,LOW);

  pinMode(5,OUTPUT);
  pinMode(A0,OUTPUT);
  digitalWrite(A0,LOW);
  isClear = true;

  pinMode(4,INPUT);
  
  delay(2000);
  
  i2c_lcd.setBacklight(HIGH); 
  i2c_lcd.print(F("Type password"));
  ArduinoUno.println('a');
  func();
  
  SPI.begin();   
  mfrc522.PCD_Init(); 
}


void loop() {
  
  if(tryCount==3){
    i2c_lcd.clear();
    i2c_lcd.print(F("Intruder Alert!!"));
    i2c_lcd.setCursor(0,1);
    i2c_lcd.print(F("Intruder Alert!!"));
    ArduinoUno.println('i');
    func();
    delay(10000);
    inputPassword="";
    i2c_lcd.clear();
    i2c_lcd.print(F("Type password"));
    ArduinoUno.println('a');
    func();
    isClear = true;
    passwordCorrect = false;
    rfidCorrect = false;
    imageCorrect = false;
    voiceCorrect = false;
    fingerprint = false;
    tryCount = 0;
  }
 
  if(digitalRead(4)==HIGH){
    i2c_lcd.clear();
    i2c_lcd.print(F("Intruder Alert!!"));
    i2c_lcd.setCursor(0,1);
    i2c_lcd.print(F("Intruder Alert!!"));
    
//    for(itr=0;itr<10;itr++){
//      tone(5,450,500);
//      delay(500);
//    }
    delay(10000);
    inputPassword="";
    i2c_lcd.clear();
    i2c_lcd.print(F("Type password"));
    ArduinoUno.println('a');
    func();
    isClear = true;
    passwordCorrect = false;
    rfidCorrect = false;
    imageCorrect = false;
    voiceCorrect = false;
    fingerprint = false;
    tryCount = 0;
  }
  if(obstacleDetected&&passwordCorrect&&rfidCorrect&&imageCorrect&&voiceCorrect&&fingerprint){
    inputPassword="";
    i2c_lcd.clear();
    i2c_lcd.print(F("Type password"));
    ArduinoUno.println('a');
    func();
    isClear = true;
    passwordCorrect = false;
    rfidCorrect = false;
    imageCorrect = false;
    voiceCorrect = false;
    fingerprint = false;
    tryCount = 0;
  }
  else if(distanceSensor.measureDistanceCm()<30){
    
    digitalWrite(5,LOW);
    i2c_lcd.setBacklight(HIGH); 
    if(!obstacleDetected){
      i2c_lcd.print(F("Type password"));
      isClear = true;
      ArduinoUno.println('a');
      func();
      
      tryCount =0;
    }
    obstacleDetected=true;
  }
  else{
    
    digitalWrite(5,HIGH);
    inputPassword="";
    i2c_lcd.clear();
    i2c_lcd.setBacklight(LOW); 
    obstacleDetected=false;
    passwordCorrect = false;
    rfidCorrect = false;
    imageCorrect = false;
    voiceCorrect = false;
    fingerprint = false;
  }
  if(obstacleDetected&&!passwordCorrect&&!rfidCorrect&&!imageCorrect&&!voiceCorrect&&!fingerprint){
    lcd_take_action(I2C_Keypad.getKey());
  }
  else if(obstacleDetected&&passwordCorrect&&!rfidCorrect&&!imageCorrect&&!voiceCorrect&&!fingerprint){
    if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) { //card ase kina
      return;
    }
    
    i2c_lcd.clear();
    i2c_lcd.print(F("Matching rfid..."));
    i2c_lcd.setCursor(0,1);
    i2c_lcd.print(F("  Please wait   "));
    ArduinoUno.print('3');
    ArduinoUno.print(mfrc522.uid.uidByte[0]);
    ArduinoUno.print(mfrc522.uid.uidByte[1]);
    ArduinoUno.print(mfrc522.uid.uidByte[2]);
    ArduinoUno.print(mfrc522.uid.uidByte[3]);
    ArduinoUno.println();
    c=ArduinoUno.read();
    while(c=='\r'||c=='\n'){
      c=ArduinoUno.read();
    }
    while(ArduinoUno.available()==0);
    c=ArduinoUno.read();
    if(c=='1'){
        tryCount=0;
        rfidCorrect = true;
        i2c_lcd.clear();
        i2c_lcd.print(F("    Success     "));
        i2c_lcd.setCursor(0,1);
        i2c_lcd.print(F("Give your speech"));
        ArduinoUno.println('e');
        func();
    }
    else if(c=='0'){
        tryCount++;
        i2c_lcd.clear();
        i2c_lcd.print(F("    failure     "));
        i2c_lcd.setCursor(0,1);
        i2c_lcd.print(F("Please try again"));
        ArduinoUno.println('d');
        func();
    }
  }
  
  else if(obstacleDetected&&passwordCorrect&&rfidCorrect&&!imageCorrect&&!voiceCorrect&&!fingerprint){
    ArduinoUno.print('0');
    i2c_lcd.clear();
    i2c_lcd.print(F("Recording audio"));
    delay(500);
    delay(3000);
    i2c_lcd.clear();
    i2c_lcd.print(F("verifying audio"));
    i2c_lcd.setCursor(0,1);
    i2c_lcd.print(F("  Please wait   "));
    //Serial.println("strt");
    while(ArduinoUno.available()==0);
    //Serial.println("aise ki");
    c=ArduinoUno.read();
    while(c=='\r'||c=='\n'){
      c=ArduinoUno.read();
    }
    while(ArduinoUno.available()==0);
    c=ArduinoUno.read();
    //Serial.println("aise");
    //Serial.println(c);
    if(c=='1'){
      tryCount=0;
      i2c_lcd.clear();
      voiceCorrect = true;
      i2c_lcd.print(F("    Correct     "));
      i2c_lcd.setCursor(0,1);
      i2c_lcd.print(F("Face the camera"));
      ArduinoUno.println('g');
      func();
    }
    else if(c=='0'){
      tryCount++;
      i2c_lcd.clear();
      i2c_lcd.print(F("   Incorrect    "));
      i2c_lcd.setCursor(0,1);
      i2c_lcd.print(F("Please try again"));
      ArduinoUno.println('f');
      func();
    }
  }
  else if(obstacleDetected&&passwordCorrect&&rfidCorrect&&!imageCorrect&&voiceCorrect&&!fingerprint){
    i2c_lcd.clear();
    i2c_lcd.print(F("Capturing photo"));
    ArduinoUno.print('1');
    c=ArduinoUno.read();
    while(c=='\r'||c=='\n'){
      c=ArduinoUno.read();
    }
    i2c_lcd.clear();
    i2c_lcd.print(F("Close&open eyes "));
    i2c_lcd.setCursor(0,1);
    i2c_lcd.print(F("slowly two times"));
    delay(3000);
    i2c_lcd.clear();
    i2c_lcd.print(F("Only one person "));
    i2c_lcd.setCursor(0,1);
    i2c_lcd.print(F("   at a time    "));
    while(ArduinoUno.available()==0);
    c=ArduinoUno.read();
    if(c == '0'){
      tryCount++;
      i2c_lcd.clear();
      i2c_lcd.print(F("   Incorrect    "));
      i2c_lcd.setCursor(0,1);
      i2c_lcd.print(F("Please try again"));
      ArduinoUno.println('h');
      func();
    }
    else if(c == '1'){
      tryCount=0;
      imageCorrect = true;
      i2c_lcd.clear();
      i2c_lcd.print(F("Please scan your"));
      i2c_lcd.setCursor(0,1);
      i2c_lcd.print(F("   fingerprint  "));
      ArduinoUno.println('j');
      func();
      
    }
  }
  else if(obstacleDetected&&passwordCorrect&&rfidCorrect&&imageCorrect&&voiceCorrect&&!fingerprint){
    i2c_lcd.clear();
    i2c_lcd.print(F("Please scan your"));
    i2c_lcd.setCursor(0,1);
    i2c_lcd.print(F("   fingerprint  "));
    
    
    while(Serial.available()==0);
    c=Serial.read();
    if(c=='3'){
      digitalWrite(A0,HIGH);
      tryCount=0;
      fingerprint = true;
      i2c_lcd.clear();
      i2c_lcd.print(F("    Correct     "));
      i2c_lcd.setCursor(0,1);
      i2c_lcd.print(F("Door unlocked"));
      digitalWrite(6,HIGH);
      delay(10000);
      digitalWrite(6,LOW);
      digitalWrite(A0,LOW);
      delay(2000);
    }
    else if(c=='1'){
        tryCount++;
        i2c_lcd.clear();
        i2c_lcd.print(F("    failure     "));
        i2c_lcd.setCursor(0,1);
        i2c_lcd.print(F("Please try again"));
        ArduinoUno.println('k');
        func();
    }
  }
}

void lcd_keypad_setup(){
  i2c_lcd.begin (16,2); //  our LCD is a 16x2, change for your LCD if needed
  
  // LCD Backlight ON
  i2c_lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  i2c_lcd.setBacklight(LOW); 
  i2c_lcd.clear(); // Clear the LCD screen
                
  I2C_Keypad.begin();
}

void lcd_take_action(char key){
  switch (key) {
    case '1':
      if(isClear){
        isClear=false;
        i2c_lcd.clear();
      }
      inputPassword+=key;
      i2c_lcd.print(key);
    break;

    case '2':
      if(isClear){
        isClear=false;
        i2c_lcd.clear();
      }
      inputPassword+=key;
      i2c_lcd.print(key);
    break;

    case '3':
      if(isClear){
        isClear=false;
        i2c_lcd.clear();
      }
      inputPassword+=key;
      i2c_lcd.print(key);
    break;

    case '4':
      if(isClear){
        isClear=false;
        i2c_lcd.clear();
      }
      inputPassword+=key;
      i2c_lcd.print(key);
    break;

    case '5':
      if(isClear){
        isClear=false;
        i2c_lcd.clear();
      }
      i2c_lcd.print(key);
      inputPassword+=key;
    break;

    case '6':
      if(isClear){
        isClear=false;
        i2c_lcd.clear();
      }
      i2c_lcd.print(key);
      inputPassword+=key;
    break;

    case '7':
      if(isClear){
        isClear=false;
        i2c_lcd.clear();
      }
      i2c_lcd.print(key);
      inputPassword+=key;
    break;

    case '8':
      if(isClear){
        isClear=false;
        i2c_lcd.clear();
      }
      i2c_lcd.print(key);
      inputPassword+=key;
    break;

    case '9':
      if(isClear){
        isClear=false;
        i2c_lcd.clear();
      }
      i2c_lcd.print(key);
      inputPassword+=key;
    break;

    case '0':
      if(isClear){
        isClear=false;
        i2c_lcd.clear();
      }
      i2c_lcd.print(key);
      inputPassword+=key;
    break;

    case '*':
      inputPassword="";
      i2c_lcd.clear();
    break;

    case '#':
      isClear = true;
      i2c_lcd.clear();
      i2c_lcd.print(F("  Matching the  "));
      i2c_lcd.setCursor(0,1);
      i2c_lcd.print(F("   password...  "));
      ArduinoUno.print('2');
      ArduinoUno.println(inputPassword);
      c=ArduinoUno.read();
      while(c=='\r'||c=='\n'){
        c=ArduinoUno.read();
      }
      while(ArduinoUno.available()==0);
      c=ArduinoUno.read();
      Serial.print(c);
      Serial.print("gese");
      i2c_lcd.clear();
      if(c=='1'){
        tryCount=0;
        i2c_lcd.print(F("    Success     "));
        i2c_lcd.setCursor(0,1);
        i2c_lcd.print(F("Insert RFID card"));
        passwordCorrect = true;
        ArduinoUno.println('c');
        func();
      }
      else if(c=='0'){
        tryCount++;
        
        
        i2c_lcd.print(F("    Failure     "));
        i2c_lcd.setCursor(0,1);
        i2c_lcd.print(F("Please try again"));
        ArduinoUno.println('b');
        func();
      }
      inputPassword="";
    break;

    case 'A':
      if(isClear){
        isClear=false;
        i2c_lcd.clear();
      }
      i2c_lcd.print(key);
      inputPassword+=key;
    break;

    case 'B':
      if(isClear){
        isClear=false;
        i2c_lcd.clear();
      }
      i2c_lcd.print(key);
      inputPassword+=key;
    break;

    case 'C':
      if(isClear){
        isClear=false;
        i2c_lcd.clear();
      }
      i2c_lcd.print(key);
      inputPassword+=key;
    break;
    
    case 'D':
      if(isClear){
        isClear=false;
        i2c_lcd.clear();
      }
      i2c_lcd.print(key);
      inputPassword+=key;
    break;
  }
}
