/*
 * https://circuits4you.com
 * 2016 November 25
 * Load Cell HX711 Module Interface with Arduino to measure weight in Kgs
 Arduino 
 pin 
 2 -> HX711 CLK
 3 -> DOUT
 5V -> VCC
 GND -> GND

 Most any pin on the Arduino Uno will be compatible with DOUT/CLK.
 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
*/

#include "HX711.h"  //You must have this library in your arduino library folder
#include <stdio.h>
#include <string.h>
#include <EEPROM.h>
const int LOADCELL_DOUT_PIN = 4; // Change to suit your board.
const int LOADCELL_SCK_PIN = 5; // Change to suit your board.
const int relayPin = 3;  // Change to suit your board.
const int singleButtonPin = 6; 
const int doubleButtonPin = 7; 
const int led = 8; 
HX711 scale;

//Change this calibration factor as per your load cell once it is found you many need to vary it in thousands
float calibration_factor = 2119; //-106600 worked for my 40Kg max scale setup 

//=============================================================================================
//                         SETUP
//=============================================================================================

float early_stop=0.0;
float single_amout=7.0;
float double_amout=14.0;
int puls_grind=100;
int wait_weight=800;
float grind_faktor = 2;
void setup() {
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(2119);
  scale.tare(); //Reset the scale to 0
  digitalWrite(relayPin, HIGH); // To make sure grinder doesn't start running during boot.
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  pinMode(led, OUTPUT);
  digitalWrite(led,LOW);
  EEPROM.get(40,early_stop);
  EEPROM.get(0,single_amout);
  EEPROM.get(20,double_amout);
  EEPROM.get(60,puls_grind);
  EEPROM.get(80,wait_weight);
  EEPROM.get(90,grind_faktor);  

}

//=============================================================================================
//                         LOOP
//=============================================================================================
int buttonsState=0;
int buttondState=0;




String readString; //main captured String 





float test = 0.00f;
void loop() {
  buttonsState = digitalRead(singleButtonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonsState == HIGH) {
         grind(single_amout);
  }
   buttondState = digitalRead(doubleButtonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttondState == HIGH) {
   
       grind(double_amout);
  }
  buttonsState=0;
  buttondState=0;

  if(Serial.available())
  {
    char c = Serial.read();  //gets one byte from serial buffer
    if (c == '*') {
      //do stuff
      

      if (readString.substring(0, 1)== "s" ) {
        single_amout=readString.substring(1).toFloat();
        EEPROM.get(0,test);
        if (test != single_amout){
        EEPROM.put(0,single_amout);}
      }
      if (readString.substring(0, 1)== "d" ) {
        double_amout=readString.substring(1).toFloat();
        EEPROM.get(20,test);
         if (test!= double_amout){
        EEPROM.put(20,double_amout);
         }
      }

      if (readString.substring(0, 1)== "e" ) {
        
        early_stop=readString.substring(1).toFloat();
        EEPROM.get(40,test);
         if (test!= early_stop){
        EEPROM.put(40,early_stop);
         }
      }
      if (readString.substring(0, 1)== "p" ) {
        
        puls_grind=readString.substring(1).toInt();
        EEPROM.get(60,test);
         if (test!= puls_grind){
        EEPROM.put(60,puls_grind);
         }
      }
      if (readString.substring(0, 1)== "w" ) {
        
        wait_weight=readString.substring(1).toInt();
        EEPROM.get(80,test);
         if (test!= wait_weight){
        EEPROM.put(80,wait_weight);
         }
      }
       if (readString.substring(0, 1)== "f" ) {
        
        grind_faktor=readString.substring(1).toInt();
        EEPROM.get(90,test);
         if (test!= grind_faktor){
        EEPROM.put(90,grind_faktor);
         }
      }
      if (readString.substring(0, 1)== "?" ){
      Serial.print("*");
      Serial.print("g");
      Serial.print(single_amout);
      Serial.print(";");
      Serial.print(double_amout);
      Serial.print(";");
      Serial.print(early_stop);
      Serial.print(";");
      Serial.print(puls_grind);
      Serial.print(";");
      Serial.print(wait_weight);
      Serial.print(";");
      Serial.print(grind_faktor);
      
      Serial.print("*");
      Serial.println();
      }
      
      readString="";
 
    }  
    else {     
      readString += c; //makes the string readString
    }


  }


  
}
void grind(float grind_amount ){
  digitalWrite(led, HIGH);
  scale.tare();
  float weight=0;
  digitalWrite(relayPin, LOW);
  unsigned long startMillis = millis();
  while (weight < grind_amount-early_stop) {
    
    weight=scale.get_units(10);
  }
 
 digitalWrite(relayPin, HIGH);
 unsigned long duration = millis()-startMillis;
 
 delay(wait_weight);
 weight=scale.get_units(20);
 float wpmil =  duration/weight;
 while(weight < grind_amount){
   int puls = round((grind_amount-weight)/grind_faktor *wpmil);
   if (puls < puls_grind ){
    puls= puls_grind;
   }
   digitalWrite(relayPin, LOW);
   delay(puls);
   digitalWrite(relayPin, HIGH);
   delay(wait_weight);
   weight=scale.get_units(20);
   wpmil = puls/weight;
}
digitalWrite(led,LOW);
 Serial.print("*r");
 Serial.print(weight);
 Serial.println("*");
}
