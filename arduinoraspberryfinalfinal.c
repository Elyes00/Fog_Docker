

//temp
#include "DHT.h"
#define PIR_MOTION 2
#define DHTPIN A0
#define DHTTYPE DHT22
///lcd
#include <TM1637Display.h>
#include<ArduinoJson.h>
//int sensor = 2;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0;    
int analogValue = 0; 
int lightSensorPin = A1;

int ledred=6;
int ledgreen=5;


const int buzzer = 10;
DHT dht(DHTPIN, DHTTYPE);

const int CLK = 9; //Set the CLK pin connection to the display
const int DIO = 8; 
TM1637Display display(CLK, DIO);

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
 display.setBrightness(0x0a);
 pinMode(PIR_MOTION, INPUT);
 pinMode(buzzer, OUTPUT);
 pinMode(ledgreen,OUTPUT);
 pinMode(ledred,OUTPUT);


}
void loop() {
  float hum = dht.readHumidity();
    float temp = dht.readTemperature();
   val = digitalRead(PIR_MOTION);
   monitor();
  
if(Serial.available()>0){
  String a=Serial.readString();
  
  if(a=="2"){
  if (ispeople()==true && analogValue > 300){
 
  Serial.print("someone is present :");
  Serial.println("sensibility is over 300 : light is down");
  digitalWrite(ledgreen, LOW);
  //delay(2000);
  }
else if( ispeople()== true && analogValue  <200){
   Serial.print("someone is present :");
  Serial.println("sensibility is below 200 :led on");
  digitalWrite(ledgreen, HIGH);
  delay(2000);
  digitalWrite(ledgreen, LOW);
  
  
  }
  else if (ispeople()== false && analogValue <200){
    Serial.print("no one is present ");
    Serial.println("light is always down ");
    digitalWrite(ledgreen, LOW);
    delay(1000);
    
  }
   else {if(ispeople()== false && analogValue >300)
  {  Serial.println("lights down");
          digitalWrite(ledgreen, LOW);
          delay(1000);
          }
   }
  }

else {
  if(a=="1"){
  
if (temp > 30)
   {Serial.print("temperature is higher than 30 *C " );
    Serial.println("air conditionning is activated");
    digitalWrite(ledred, HIGH);
    tone(buzzer, 400,1000);
    delay(1000);
   }
else {if (temp < 30)
   {
   digitalWrite(ledred, LOW);
    noTone(buzzer);
         delay(1000);
   } }
  }
}
//else {Serial.println("eroor");}
}
}
  

void monitor(){
    float hum1 = dht.readHumidity();
    float temp1 = dht.readTemperature();
    analogValue = analogRead(lightSensorPin);
 Serial.print(temp1,1);
Serial.print("\t");
Serial.print(hum1,1);
Serial.print("\t");
Serial.print(analogValue);
Serial.println("\t");

}

boolean ispeople(){
  
  int state=digitalRead(PIR_MOTION);
  if(state == HIGH) return true;
  else return false;
  
  }
