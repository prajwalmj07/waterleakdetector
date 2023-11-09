#include <ESP8266WiFi.h>
volatile byte flow_frequency,flow_frequency2; // Measures flow sensor pulses
// Calculated litres/hour
unsigned char flowsensor2 = 5;
float vol = 0.0,l_minute,l_minute2;
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long currentTime2;
unsigned long cloopTime2;
unsigned long cloopTime;
int relay = 4;
int rate,rate2;
int c=0;

void IRAM_ATTR flow() // Interrupt function
{
   flow_frequency++;
}

void IRAM_ATTR flow2(){
  flow_frequency2++;
  }
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "watleaks-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "aEFc6q2t26IiAahuHM2auWEbnC2aqAdaRIzAaBQp"
#define WIFI_SSID "username"
#define WIFI_PASSWORD "password"

void setup() {
  pinMode(relay,OUTPUT);
pinMode(flowsensor, INPUT);
 pinMode(flowsensor2, INPUT);
 digitalWrite(flowsensor2, HIGH);
   digitalWrite(flowsensor, HIGH); 
Serial.begin(9600);

attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); 
attachInterrupt(digitalPinToInterrupt(flowsensor2), flow2, RISING);
   currentTime = millis();
   currentTime2= millis();
   cloopTime = currentTime;
   cloopTime2=currentTime2;
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int n = 0;
int motor;

void loop() {
  // set value
  
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // update value


  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // get value 
  Serial.print("motor: ");
  Serial.println(Firebase.getInt("Motor"));
  motor=Firebase.getInt("Motor");
  if(motor==1){
    digitalWrite(relay,HIGH);
   }
  else{digitalWrite(relay,LOW);
  }
  




  currentTime= millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
    cloopTime = currentTime; // Updates cloopTime
    if(flow_frequency != 0){
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_minute = (flow_frequency / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0;
      Serial.print(l_minute, DEC); // Print litres/hour
      Serial.println(" L/Sec");
      vol= vol+(l_minute/60);
      Firebase.setInt("Variable/Value",vol);
      rate = (int)l_minute;
      Firebase.setInt("Variable/Flowrate1",rate);

}
else { 
      Serial.print("0");
      }}
      
      currentTime2 = millis();
   // Every second, calculate and print litres/hour
   if(currentTime2>=(cloopTime2 + 1000))
   {
    cloopTime2 = currentTime2; // Updates cloopTime
    if(flow_frequency2 != 0){
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_minute2 = (flow_frequency2  / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour    
      flow_frequency2 = 0; // Reset Counter
     Serial.print("flow sensor2=");
      Serial.print(l_minute2, DEC); // Print litres/hour
      Serial.println(" L/min");
      rate2 = (int)l_minute2;
      Firebase.setInt("Variable/Flowrate2",rate2);
     
    }
    else {
      Serial.println(" flow rate2= 0 ");
     
    }
      
      }


      if(rate/20!=rate2/20){
        Firebase.setInt("Value",1);
        }else Firebase.setInt("Value",0);

}
