

int analogPin0 = 1;
int analogPin1 = 0;
int wasser=0;
int luft=0;
int up=1;
int alarm_suspend = 0;
const int buzzer =  5;     

unsigned long time2 = millis(); //suspend timer
unsigned long time3 = millis(); // NMEA ausgabe



const byte interruptPin = 2;
const byte wasserLED =3;
const byte luftLED =4;


const byte buff_size = 80; // buffer size must be a constant variable
char buffer[buff_size];

byte CRC = 0;
boolean data_end = false; // Here we will keep track of EOT (End Of Transmission).
String WasserTemperatur="$IIMTW,";
String LuftTemperatur="$IIMTA,";
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
  Serial.begin(9600);
  Serial.print ("warmup");
//delay(60000);
 digitalWrite(wasserLED, LOW);
 digitalWrite(luftLED, LOW);


 
}

void loop() {
 
 wasser = analogRead(analogPin0); 
 luft = analogRead(analogPin1); 
 up = digitalRead (interruptPin);
Serial.print (" Wasser: ");
Serial.print (wasser);
Serial.print ("   Luft: ");
Serial.println (luft);


// NMEA Ausgabe Basteln alle 1s
if( millis()-time3 > 1000)   
  {
 time3 = millis(); 
 NMEA=WasserTemperatur+MTW+ende;
 StringLength = NMEA.length();  
 NMEA_Ausgabe();
 NMEA=LuftTemperatur+MTA+ende;
 StringLength = NMEA.length();  
 NMEA_Ausgabe();
}



//Wasser Alarm 
if (wasser < 950) {
  up = digitalRead (interruptPin);
  //if (up==0) alarm_suspend = 1;
  
  digitalWrite(wasserLED, HIGH);
  
     if (alarm_suspend == 1) { 
      noTone(buzzer);
      }
     else {
      alarm1();
     }
    }
else {
  digitalWrite(wasserLED, LOW);
  noTone(buzzer);
}

// Luft Alarm
if (luft > 80)  {
  up = digitalRead (interruptPin);
  //if (up==0) alarm_suspend = 1;

  digitalWrite(luftLED, HIGH);
  
     if (alarm_suspend == 1) { 
      noTone(buzzer);
      }
     else {
      alarm2();
     }
   
}
else {
  digitalWrite(luftLED, LOW);
  noTone(buzzer);
}
 
// Alarmsuspendierung nach 60s zurücksetzen
if(alarm_suspend == 1 && millis()-time2 > 60000)   
  {
 time2 = millis();   
 alarm_suspend = 0;
}
}

void alarm1() {
 tone(buzzer,1000);
 delay (20);
 tone(buzzer,1200);
 delay (20);
 
} 
void alarm2() {
  tone(buzzer,2000);
 delay (20);
 tone(buzzer,2200);
 delay (20);
 
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
  



