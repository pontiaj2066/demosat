//Defintions
#include <TimeLib.h>

//LED Circuit
const int SB_LED = 13;
int photoresistor = 0;
int photoresistor_pin = A4;

//Sound Circuit Interior
const int BUZZER = 12;
int sound_int_gate;
int sound_int_env;

const int sound_int_gate_pin = 11;
const int sound_int_env_pin = A0;

//External Sensors
int sound_ext_gate;
int sound_ext_env;

const int sound_ext_gate_pin = 10;
const int sound_ext_env_pin = A1; 

const int amb_light_pin = A5;
float amb_light;

int uv_light;
const int uv_light_pin = A2;

const int led_ext = 3;

//Time Keeper
uint32_t timeStamp = 0;
int timer = 0;

// t1 & t2
unsigned long time1;
unsigned long time2;

bool micRecording = false;
unsigned long micTimer = 0;
//------------------------------------------------------------------------------------

void setup() {
//setup code to run once:
  
//baud rate
  Serial.begin(9600);

// setup for circuit elements
  pinMode(BUZZER, OUTPUT); //Buzzer
  pinMode(sound_int_gate, OUTPUT); //Interal sound, Gate
  pinMode(sound_int_env, OUTPUT); //Internal sound, Envelope
  pinMode(sound_ext_gate, OUTPUT); //External sound, Gate
  pinMode(sound_ext_env, OUTPUT); //External sound, Envelope
  pinMode(led_ext, OUTPUT); //Arduino on
  pinMode(SB_LED, OUTPUT); //Superbright LED
  pinMode(photoresistor_pin, INPUT); //Photoresistor
  pinMode(amb_light_pin, INPUT); //Ambient Light Sensor
  pinMode(uv_light, OUTPUT); //UV Light Sensor

//turn on Arduino LED
  digitalWrite(led_ext, HIGH); //Leave on while power is on

//Print column headers
  Serial.println("Time        | Gate Int | Env Int | Photoresistor | Gate Ext | Env Ext | Amb Light | UV Light");
}

//------------------------------------------------------------------------------------

void loop() {
  
// put your main code here, to run repeatedly:



// Log the time
     //timeStamp = millis() / 1000; //3600000;
     char timeString[100];
     sprintf(timeString, "%d:%02d:%02d.%03d | ", hour(),minute()%60,second()%60,millis()%1000);
     Serial.print(timeString);

     

//Internal Sound Circuit
     tone(BUZZER,1000);
     
     if (millis()- micTimer >= 10000) {
      micTimer = millis();
      micRecording = !micRecording;
     }
    // micRecording = true;
     if (micRecording){
        sound_int_gate = digitalRead(11);
        sound_int_env = analogRead(A0);
        char soundString[100];
        sprintf(soundString,"%d        | %04d   ", sound_int_gate, sound_int_env);
        Serial.print(soundString);
     } else {
        Serial.print("0        | 0      ");
     }

//Internal LED Circuit
photoresistor = analogRead(A4);
Serial.print(" | ");
Serial.print(analogRead(A4));

//External Sound Detector
sound_int_gate = digitalRead(10); 
sound_int_env = analogRead(A1);
char externalString[100];
sprintf(externalString, "           | %d        | %04d   ", digitalRead(10),analogRead(A1));
Serial.print(externalString);


//Ambient Light Sensor
float amb_light = analogRead(A5);
char ambString[100];
sprintf(ambString," | %04d",(analogRead(A5)));
Serial.print(ambString);
//Serial.print(amb_light);//, 4%3);

//UV Light Sensor
Serial.print("      | ");
uv_light = analogRead(A2);
Serial.print(uv_light);



Serial.println();

if (millis() >= 10800000) exit(0);
}
