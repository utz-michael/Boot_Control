

int analogPin0 = 1;
int analogPin1 = 0;
int wasser=0;
int luft=0;
int up=1;
int alarm_suspend = 0;
const int buzzer =  5;     
unsigned long time = millis();
unsigned long time2 = millis();


const byte interruptPinUp = 2;
const byte interruptPinDown =3;

//int fastforwardUp = 0;
//int fastforwardDown = 0;

void setup() {

  
 pinMode(interruptPinUp, INPUT);
 pinMode(interruptPinDown, INPUT);
 digitalWrite(interruptPinUp, HIGH);
 digitalWrite(interruptPinDown, HIGH);
 

  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);

//delay(60000);
}

void loop() {
 
 wasser = analogRead(analogPin0); 
 luft = analogRead(analogPin1); 
 up = digitalRead (interruptPinUp);

//Wasser Alarm 
if (wasser < 950) {
  up = digitalRead (interruptPinUp);
  if (up==0) alarm_suspend = 1;
  //Alarm
     if (alarm_suspend == 1) { 
      noTone(buzzer);
      }
     else {
      alarm1();
     }
    }
else noTone(buzzer);

// Luft Alarm
if (luft > 80)  {
  up = digitalRead (interruptPinUp);
  if (up==0) alarm_suspend = 1;
  //Alarm 
     if (alarm_suspend == 1) { 
      noTone(buzzer);
      }
     else {
      alarm2();
     }
   
}
else noTone(buzzer);






if (wasser < 950 || luft > 80) 
{ //nichts machen während ses Alarmes
  }

else {


// display alle sekunde refresh
 if(millis()-time > 1000)     //Has one second passed?
  {
   time = millis();           //and reset time.
   //nix machen 
 }
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



