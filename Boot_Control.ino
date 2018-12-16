
// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);




int analogPin0 = 1;
int analogPin1 = 0;
int wasser=0;
int luft=0;
int up=1;
int alarm_suspend = 0;
int alarmgesetzt =0;


unsigned long time2 = millis(); //suspend timer
unsigned long time3 = millis(); // NMEA ausgabe



const byte interruptPin = 0;
const byte wasserLED =3;
const byte luftLED =4;
const int buzzer =  5;



byte CRC = 0;
boolean data_end = false; // Here we will keep track of EOT (End Of Transmission).
String WasserTemperatur="$IIMTW,";
String LuftTemperatur="$IIMTA,";
String XDR="$IIXDR,";
String ende=",C*";
String NMEA="";
float MTW=23.5;
float MTA=-10.3;

unsigned int StringLength = NMEA.length();



void setup() {
 

  
 pinMode(interruptPin, INPUT);
 pinMode(wasserLED, OUTPUT);
 pinMode(luftLED, OUTPUT);
 digitalWrite(interruptPin, HIGH);
 digitalWrite(wasserLED, HIGH);
 digitalWrite(luftLED, HIGH);
 

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
  Serial.begin(9600);
  Serial.println ("warmup");
//delay(60000);
 digitalWrite(wasserLED, LOW);
 digitalWrite(luftLED, LOW);

 Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();

  
  
}

 


void loop() {
 
 wasser = analogRead(analogPin0); 
 luft = analogRead(analogPin1); 
 up = digitalRead (interruptPin);



// NMEA Ausgabe Basteln alle 1s
if( millis()-time3 > 1000)   
  {

 
  sensors.requestTemperatures();
  
  
  float MTA = sensors.getTempCByIndex(0);
  float MTW = sensors.getTempCByIndex(1);

    
 time3 = millis(); 
 NMEA=WasserTemperatur+MTW+ende;
 StringLength = NMEA.length();  
 NMEA_Ausgabe();
 NMEA=LuftTemperatur+MTA+ende;
 StringLength = NMEA.length();  
 NMEA_Ausgabe();

 NMEA=XDR+","+wasser+",,Wasser Alarm*";
 StringLength = NMEA.length();  
 NMEA_Ausgabe();

 NMEA=XDR+","+luft+",,Luft Alarm*";
 StringLength = NMEA.length();  
 NMEA_Ausgabe();

//Wasser Alarm 
if (wasser < 950) {
  up = digitalRead (interruptPin);
  //if (up==0) alarm_suspend = 1;
  
  digitalWrite(wasserLED, HIGH);
  
     if (alarm_suspend == 1) { 
      noTone(buzzer);
      }
     else {
      
        tone(buzzer,1200);
      }
    }
else if (luft > 180)  {
  up = digitalRead (interruptPin);
  //if (up==0) alarm_suspend = 1;

  digitalWrite(luftLED, HIGH);
  
     if (alarm_suspend == 1) { 
      noTone(buzzer);
      }
     else {
      tone(buzzer,2000);
     }
   
} 

// Luft Alarm

else {
  digitalWrite(wasserLED, LOW);
  digitalWrite(luftLED, LOW);
  noTone(buzzer);
  
}



 
}




 
// Alarmsuspendierung nach 60s zurücksetzen
if(alarm_suspend == 1 && millis()-time2 > 60000)   
  {
 time2 = millis();   
 alarm_suspend = 0;
 
}
}





void NMEA_Ausgabe() {
  for (byte x = 1; x<(StringLength-1); x++){ // XOR every character in between '$' and '*'
     
      CRC = CRC ^ NMEA[x] ;
      //Serial.println(NMEA[x]);
    }
  
  if(CRC > 0){
    Serial.print (NMEA);
    Serial.println(CRC,HEX); // print calculated CS in HEX format.
    CRC = 0; // reset CRC variable
    
      }
  
}
  





