/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/
#include "EspMQTTClient.h"

EspMQTTClient client(
  "MeinNetz",
  "MeinNetzistsehrstark235689",
  "192.168.1.29",  // MQTT Broker server ip
  "boyi",   // Can be omitted if not needed
  "wB9fApv5nvoV4J4kvq5L",   // Can be omitted if not needed
  "grinder"      // Client name that uniquely identify your device
);






float early_stop;
float single_amout;
float double_amout;
int puls_grind;
int wait_weight;
float grind_faktor;



void onConnectionEstablished() {

  client.subscribe("coffe/grinder/getstate", [] (const String &payload)  {
    if (payload.toInt()  == 1){
      Serial.println();
      Serial.println("*?*");
      client.publish("coffe/grinder/getstate","0");
    }
  });
  client.subscribe("coffe/grinder/single_dose", [] (const String &payload)  {
    single_amout=payload.toFloat()  ;
  });
  client.subscribe("coffe/grinder/double_dose", [] (const String &payload)  {
    double_amout=payload.toFloat()   ; 
  });
  client.subscribe("coffe/grinder/early_stop", [] (const String &payload)  {
    early_stop=payload.toFloat();  
  });
  client.subscribe("coffe/grinder/puls_grind", [] (const String &payload)  {
    puls_grind=payload.toInt();  
  });
  client.subscribe("coffe/grinder/wait_weight", [] (const String &payload)  {
    wait_weight=payload.toInt();  
  });
   client.subscribe("coffe/grinder/grind_faktor", [] (const String &payload)  {
    grind_faktor=payload.toInt();  
  });
     client.subscribe("coffe/grinder/set_grind_faktor", [] (const String &payload)  {
    if (payload.toInt()  == 1){
      Serial.println();
      Serial.print("*f");
      Serial.print(grind_faktor);
      Serial.println("*");
      client.publish("coffe/grinder/set_grind_faktor","0");
    }
  });
   client.subscribe("coffe/grinder/set_wait_weight", [] (const String &payload)  {
    if (payload.toInt()  == 1){
      Serial.println();
      Serial.print("*w");
      Serial.print(wait_weight);
      Serial.println("*");
      client.publish("coffe/grinder/set_wait_weight","0");
    }
  });
   client.subscribe("coffe/grinder/set_puls_grind", [] (const String &payload)  {
    if (payload.toInt()  == 1){
      Serial.println();
      Serial.print("*p");
      Serial.print(puls_grind);
      Serial.println("*");
      client.publish("coffe/grinder/set_puls_grind","0");
    }
  });
  client.subscribe("coffe/grinder/set_single_dose", [] (const String &payload)  {
    if (payload.toInt()  == 1){
      Serial.println();
      Serial.print("*s");
      Serial.print(single_amout);
      Serial.println("*");
      client.publish("coffe/grinder/set_single_dose","0");
    }
  });
  client.subscribe("coffe/grinder/set_double_dose", [] (const String &payload)  {
    if (payload.toInt()  == 1){
      Serial.println();
      Serial.print("*d");
      Serial.print(double_amout);
      Serial.println("*");
      client.publish("coffe/grinder/set_double_dose","0");
    }
  });
  client.subscribe("coffe/grinder/set_early_stop", [] (const String &payload)  {
    if (payload.toInt() == 1){
      Serial.println();
      Serial.print("*e");
      Serial.print(early_stop);
      Serial.println("*");
      client.publish("coffe/grinder/set_early_stop","0");
    }
  });
}


void setup()
{
  // Debug console
  
  Serial.begin(9600);
  client.enableHTTPWebUpdater();
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  //timer.setInterval(1000L, myTimerEvent);
}
String readString; //main captured String 
float grind=0.0f;
void loop()
{
  client.loop();


    if(Serial.available())
  {
 
    char c = Serial.read();  //gets one byte from serial buffer
    if (c == '*') {
      //do stuff
      

      if (readString.substring(0, 1)== "g" ) {
        int ind1 = readString.indexOf(';');
        client.publish("coffe/grinder/single_dose",readString.substring(1,ind1) );
        int ind2 = readString.indexOf(';',ind1+1);
        client.publish("coffe/grinder/double_dose",readString.substring(ind1+1,ind2));
        int ind3 = readString.indexOf(';',ind2+1);
        client.publish("coffe/grinder/early_stop",readString.substring(ind2+1,ind3) );
        int ind4 = readString.indexOf(';',ind3+1);
        client.publish("coffe/grinder/puls_grind",readString.substring(ind3+1,ind4) );
        int ind5 = readString.indexOf(';',ind4+1);
        client.publish("coffe/grinder/wait_weight",readString.substring(ind4+1,ind5) );
        int ind6 = readString.indexOf(';',ind5+1);
        client.publish("coffe/grinder/grind_faktor",readString.substring(ind5+1,ind6) );
      }
      if (readString.substring(0, 1)== "r" ) {
        grind=readString.substring(1).toFloat();
        String myString = "";     // empty string
        myString.concat(grind);
        client.publish("coffe/grinder/lastgrind",myString);
      }

      
     readString="";
 
    }  
    else {     
      readString += c; //makes the string readString
    }


  }

}
