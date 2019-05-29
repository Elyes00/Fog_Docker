//temp
#include "DHT.h"
#define PIR_MOTION 2
#define DHTPIN A0
#define DHTTYPE DHT22
///lcd
#include <TM1637Display.h>
#include<ArduinoJson.h>
//int sensor = 2; 
int ledred=6;
int ledgreen=5;// the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0;    
int analogValue = 0; 
int lightSensorPin = A1;

const int buzzer = 10;
DHT dht(DHTPIN, DHTTYPE);

const int CLK = 9; //Set the CLK pin connection to the display
const int DIO = 8; 
DynamicJsonBuffer jBuffer;
    JsonObject& root = jBuffer.createObject();
  
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
   humtempsens();
if(Serial.available()>0){
 //temps();
 delay(1000);
 //lig();
   //  DynamicJsonBuffer jBuffer1;
  //JsonObject& root1 = jBuffer1.createObject();
      String a=Serial.readString();
     
  if(a=="1"){
     
         if (temp > 30)
   {
    
    Serial.print("temperature is higher than 30 *C " );
    Serial.println("air conditionning is activated");
    digitalWrite(ledred, HIGH);
    tone(buzzer, 300);
    
   }
      else {if (temp < 30)
   {digitalWrite(ledred, LOW);
    noTone(buzzer);
    
   } }
    }
    
  else if(a=="2"){
    
  if (ispeople()==true && analogValue > 300){
  Serial.print("someone is present :");
  Serial.println("sensibility is over 300 : light is down");
  digitalWrite(ledgreen, LOW);
 // delay(1000);
  }
      else if( ispeople()== true && analogValue  <200){
        //Serial.println(analogValue);
  Serial.print("someone is present :");
  Serial.println("sensibility is below 200 :led on");
  digitalWrite(ledgreen, HIGH);
  //delay(1000);
  
  }
      else if (ispeople()== false && analogValue <200){
    //Serial.println(analogValue);
    Serial.print("no one is present ");
    Serial.println("light is always down ");
    digitalWrite(ledgreen, LOW);
  //delay(1000);
  }

      else{ if(ispeople()== false && analogValue >300)
        {
          //Serial.println(analogValue);
          Serial.println("lights down");
          digitalWrite(ledgreen, LOW);}
}
}

}
}

void humtempsens(){
  analogValue = analogRead(lightSensorPin);
   float hum1 = dht.readHumidity();
    float temp1 = dht.readTemperature();
  //DynamicJsonBuffer jBuffer;
    //JsonObject& root = jBuffer.createObject();
  //root["temperature"]= temp1;
  //root["humidity"]=hum1;
  Serial.print(temp1,1);
Serial.print("\t");
Serial.print(hum1,1);
Serial.print("\t");
Serial.print(analogValue);
Serial.println("\t");
  //Serial.print("Sensibility :");
  //Serial.println(analogValue);
    
    
    //Serial.print("Temperature:"); 
    //Serial.println(temp1);
    display.showNumberDec(temp1);
  //Serial.print(" Humidity:"); 
    //Serial.println(hum1);
    delay(1000);
    display.showNumberDec(hum1);
  delay(1000);
  display.showNumberDec(analogValue);
  delay(1000);
  //root.prettyPrintTo(Serial);
  //Serial.println();
}

boolean ispeople(){
  
  int state=digitalRead(PIR_MOTION);
  if(state == HIGH) return true;
  else return false;
  
  }

   