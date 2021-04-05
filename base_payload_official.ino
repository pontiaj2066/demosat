/* Balloon Shield Rev. 1.0
   Test Code - Jesse Austin 11/20/14
   Updated 12/16/2014 - Chris Koehler for CCCE Workshop 2015
      -Modified from WRSG Arduino Workshop Code
   Updated 01/04/2017 for OpenLog   
   
   Sensor Code for:
   - Temperature x2
   - Humidity
   - Pressure
   - Accelerometer
   - 6 LED Visual Display */

// Definitions
#include <TimeLib.h>
#include <SPI.h>
#include <Wire.h>
#include "SparkFun_AS3935.h"
// Temperature Sensor #1    
    int temp1;
    float temp1Volt;
    float temp1C;
    float temp1F;
    
// Temperature Sensor #2   
    int temp2;
    float temp2Volt;
    float temp2C;
    float temp2F;
   
// Humidity Sensor    
    int humidity;
    float humidityVolt;
    float RH;
    
// Presure Sensor         
    int pressure;
    float pressureVolt;
    float psi;
    
// Accelerometer X  
    int accelX;
    float accelXVolt;
    float accelXG;
    
// Accelerometer Z
    int accelZ;
    float accelZVolt;
    float accelZG;
    
// Time keeper 
// The time stamp used when recording data points
    uint32_t timeStamp = 0;     

    int timer = 0;

// Lightning Detector
    #define INDOOR 0x12
    #define OUTDOOR 0xE
    #define LIGHTNING_INT 0x08
    #define DISTURBER_INT 0x04
    #define NOISE_INT 0x01
    SparkFun_AS3935 lightning;
// Interrupt pin for lightning detection
    const int lightningInt = 2;
    int spiCS = 10;
// This variable holds the number representing the lightning or non-lightning
// event issued by the lightning detector. 
    int intVal = 0;
    int noise = 2; // Value between 1-7 
    int disturber = 2; // Value between 1-10

    
void setup() {
// put your setup code here, to run once:

      Serial.begin(9600);

// setup the LED Visual Display
      pinMode(3, OUTPUT);  //Arduino on
      pinMode(4, OUTPUT);  //Internal Temp
      pinMode(5, OUTPUT);  //External Temp
      pinMode(6, OUTPUT);  //Humidity
      pinMode(7, OUTPUT);  //Pressure
      pinMode(9, OUTPUT);  //Accels
      pinMode(13, OUTPUT);

//Lightning setup code
// When lightning is detected the interrupt pin goes HIGH.
      pinMode(lightningInt, INPUT); 
      SPI.begin(); 
      lightning.setIndoorOutdoor(OUTDOOR);

// turn on Arduino LED
      digitalWrite(3, HIGH);  // Leave on while power is on 

// Print Column Headers
      Serial.println("Time        | Temp1F (I) | Temp2F (E) | Humidity | Pressure | AccX   | AccZ   | Disturb | Noise | Lightning");
}

void loop() { 
//   put your main code here, to run repeatedly:
    
//   Turn script running leds OFF at begining of loop
     digitalWrite(4, LOW);
     digitalWrite(5, LOW);
     digitalWrite(6, LOW);
     digitalWrite(7, LOW);
     digitalWrite(9, LOW);

     if(millis() > 10000000) exit(0);
     
     delay(500);  //Amount of time between samples (milliseconds)
     
// Log the time
     timeStamp = millis() / 1000; //3600000;
     char timeString[100];
     sprintf(timeString, "%d:%02d:%02d.%03d | ", hour(),minute()%60,second()%60,millis()%1000);
     Serial.print(timeString);

//Internal Temperature Sensor
     temp1 = analogRead(A0);
     temp1Volt = temp1*(5.0/1023);
     temp1C = (temp1Volt - 0.5)/(0.01);
     temp1F = (temp1C*(9.0/5.0) + 32);
         
     Serial.print(temp1F, 2);
     digitalWrite(5, HIGH);

//External Temperature Sensor 
     temp2 = analogRead(A1);
     temp2Volt = temp2*(5.0/1023);
     temp2C = (temp2Volt - 0.5)/(0.01);
     temp2F = (temp2C*(9.0/5.0) + 32);
     Serial.print("      | ");    
     Serial.print(temp2F, 2);
     digitalWrite(4, HIGH);

//Humidity Sensor
     humidity = analogRead(A2);
     humidityVolt = humidity*(5.0/1023);
     RH = (((humidityVolt/5.0)-0.16)/0.0062);
     Serial.print("      | ");    
     Serial.print(RH, 2);
     digitalWrite(6, HIGH);

//Pressure Sensor
     pressure = analogRead(A3);
     pressureVolt = pressure*(5.0/1023);
     psi = (pressureVolt-0.5)*(15.0/4.0); 
     Serial.print("    | ");    
     Serial.print(psi, 2);
     digitalWrite(7, HIGH);

//Acceleration
     accelX = analogRead(A4);
     accelXVolt = accelX*(5.0/1023);
     accelXG = (accelXVolt - (3.3/2))/(0.330);
     Serial.print("    | ");    
     Serial.print(accelXG > 0 ? " " : "");
     Serial.print(accelXG,3);

     accelZ = analogRead(A5);
     accelZVolt = accelZ*(5.0/1023);
     accelZG = (accelZVolt - (3.3/2))/(0.330);
     Serial.print(" | "); 
     Serial.print(accelZG > 0 ? " " : "");   
     Serial.print(accelZG,3);
     digitalWrite(9, HIGH);
     
//Lightning Detector
     bool noise = false, disturber = false;
     byte distance;
     if(digitalRead(lightningInt) == HIGH){
       intVal = lightning.readInterruptReg();
       if(intVal == NOISE_INT){
         noise = true;
       }
       else if(intVal == DISTURBER_INT){
         disturber = true; 
       }
       else if(intVal == LIGHTNING_INT){
         distance = lightning.distanceToStorm();
       } else {
         distance = 0;
       }
     }
    char lightningString[100];
    sprintf(lightningString, " | %d       | %d     | %d", disturber, noise, distance);
    Serial.print(lightningString);
     

    Serial.println();
   

}
