#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

const char* ssid = "Nafi";
const char* password =  "nafitheboss";

const uint16_t port = 7000;
const char * host = "192.168.43.227";

char c;
char charPacket[1000];
int noOfChars=0;

WiFiClient client;
String pass;
boolean lenArrived = false;
int len;
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.println("...");
  }
  //Serial.println("successful wifi");
  while(!client.connect(host, port)) {
        //Serial1.println("Connection to host failed");
        delay(1000);
  }
}

void loop() {
  while(client.available()>0){
      c = client.read();
      Serial.print(c);
  }
  if(!client.connected()){
    while(!client.connect(host, port)) {
        //Serial1.println("Connection to host failed");
        delay(1000);
    }
  }
  while(Serial.available()>0){
    c = Serial.read();
    //Serial.print(c);
    client.write(c);
  }
}
