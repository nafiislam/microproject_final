int i;
void setup() {
  // put your setup code here, to run once:
  pinMode(A0,INPUT);
  pinMode(4,OUTPUT);
  pinMode(8,INPUT);
  pinMode(6,INPUT);
  pinMode(7,OUTPUT);
  Serial.begin(9600);
}
int a;
void loop() {
  // put your main code here, to run repeatedly:
  a=analogRead(A0);
  if(a>975&&digitalRead(8)==LOW){
    digitalWrite(4,HIGH);
    for(i=0;i<10;i++){
      tone(5,450,500);
      delay(1000);
    }
    Serial.println("large");
  }
  else{
    digitalWrite(4,LOW);
  }
  
  if(digitalRead(6)==LOW){
    digitalWrite(7,HIGH);
  }
  else{
    digitalWrite(7,LOW);
  }
  
}
