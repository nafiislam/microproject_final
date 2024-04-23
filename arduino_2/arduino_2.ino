#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <TMRpcm.h>
File myFile;
SoftwareSerial ArduinoUno(2,3);//2->Rx 3->Tx

TMRpcm audio;
int len;
char c;
char dum;
char cmd;
String pass = "i am not a robot";

void setup() {
  // put your setup code here, to run once:
//  espcam.begin(115200);
//  ArduinoUno.begin(115200);
  audio.CSPin = 4;
  audio.speakerPin = 9;
  Serial.begin(9600);
  ArduinoUno.begin(115200);
  while (!SD.begin(4)) {  
//    Serial.println(F("fail sd"));
    delay(1000);
  }
}

void loop() {
//    Serial.println("chole");
//    audio.play("lu.wav");
//    delay(12000);
    if(Serial.available()>0){
      cmd = Serial.read();
      dum = Serial.peek();
      if(dum=='\r')
        dum = Serial.read();
      dum = Serial.peek();
      if(dum=='\n')
        dum = Serial.read();
     //Serial.println(cmd);
     if(cmd=='0'){
        tone(5,450,500);
        delay(500);
//        Serial.println(cmd);
        audio.startRecording("mic.wav",16000,A0);
        delay(3000);
        audio.stopRecording("mic.wav");
        tone(5,450,500);
//        Serial.println(cmd);
        myFile = SD.open(F("mic.wav"));
        if (myFile){
          
          // read from the file until there's nothing else in it:
          if (myFile.available()>0) {
            ArduinoUno.print("0");
            while(myFile.available()>0){
              c = myFile.read();
              ArduinoUno.print(c);
              //Serial.print(c);
            }
            ArduinoUno.print(F("<EOF>"));
            ArduinoUno.print(pass);
      
            ArduinoUno.print(F("<EOF2>"));
            while(ArduinoUno.available()==0);
            c=ArduinoUno.read();
            Serial.println(c);
            //Serial.write(c);
            while(Serial.available()>0){
                c= Serial.read();
            }
          }
          // close the file:
          myFile.close();
        } else {
          //myFile did not opened
        }
     }
     else if(cmd=='1'){
        tone(5,800,500);
        delay(500);
        ArduinoUno.print("1");
        while(ArduinoUno.available()==0);
        c=ArduinoUno.read();
        tone(5,800,500);
        delay(500);
        Serial.println(c);
     }
     else if(cmd=='2'){
        ArduinoUno.print("2");
        while(Serial.available()==0);
        ArduinoUno.print(Serial.readStringUntil('\n'));
        Serial.read();
        ArduinoUno.print(F("<EOF2>"));
        while(ArduinoUno.available()==0);
        c=ArduinoUno.read();
        Serial.println(c);
        
     }
     else if(cmd=='3'){
        ArduinoUno.print("3");
        while(Serial.available()==0);
        ArduinoUno.print(Serial.readStringUntil('\n'));
        Serial.read();
        ArduinoUno.print(F("<EOF2>"));
        while(ArduinoUno.available()==0);
        c=ArduinoUno.read();
        Serial.println(c);
        
     }
     else if(cmd=='a'){
        ArduinoUno.print("a");
        while(ArduinoUno.available()==0);
        c=ArduinoUno.read();
        Serial.println(c);
     }
     else if(cmd=='b'){
        ArduinoUno.print("b");
        while(ArduinoUno.available()==0);
        c=ArduinoUno.read();
        Serial.println(c);
     }
     else if(cmd=='c'){
        ArduinoUno.print("c");
        while(ArduinoUno.available()==0);
        c=ArduinoUno.read();
        Serial.println(c);
     }
     else if(cmd=='d'){
        ArduinoUno.print("d");
        while(ArduinoUno.available()==0);
        c=ArduinoUno.read();
        Serial.println(c);
     }
     else if(cmd=='e'){
        ArduinoUno.print("e");
        while(ArduinoUno.available()==0);
        c=ArduinoUno.read();
        Serial.println(c);
     }
     else if(cmd=='f'){
        ArduinoUno.print("f");
        while(ArduinoUno.available()==0);
        c=ArduinoUno.read();
        Serial.println(c);
     }
     else if(cmd=='g'){
        ArduinoUno.print("g");
        while(ArduinoUno.available()==0);
        c=ArduinoUno.read();
        Serial.println(c);
     }
     else if(cmd=='h'){
        ArduinoUno.print("h");
        while(ArduinoUno.available()==0);
        c=ArduinoUno.read();
        Serial.println(c);
     }
     else if(cmd=='i'){
        ArduinoUno.print("i");
        while(ArduinoUno.available()==0);
        c=ArduinoUno.read();
        Serial.println(c);
     }
     else if(cmd=='j'){
        ArduinoUno.print("j");
        while(ArduinoUno.available()==0);
        c=ArduinoUno.read();
        Serial.println(c);
     }
     else if(cmd=='k'){
        ArduinoUno.print("k");
        while(ArduinoUno.available()==0);
        c=ArduinoUno.read();
        Serial.println(c);
     }
   }
}
