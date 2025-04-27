/*
   Sketch: RemoteXY_Pan_Tilt.ino
   Author: Don Potbury March 2025
   Description: Control the pan and tilt functions on a camera gimbal.

   This sketch uses RemoteXY for the user interface.  Instructions below:
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.13 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.15.01 or later version;
     - for iOS 1.12.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG    

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__WIFI

#include <ESP8266WiFi.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "TheMakersSpace"
#define REMOTEXY_WIFI_PASSWORD "Godisthemastermaker"
#define REMOTEXY_SERVER_PORT 6377


#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 55 bytes
  { 255,2,0,0,0,48,0,19,0,0,0,80,97,110,95,84,105,108,116,0,
  31,1,106,200,1,1,2,0,5,23,130,60,60,32,2,26,31,129,3,4,
  98,23,64,17,80,97,110,32,38,32,84,105,108,116,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int8_t pantilt_x; // from -100 to 100
  int8_t pantilt_y; // from -100 to 100

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////


// ESP8266 Pins to GPIO numbers
#define D0  16 // HIGH at boot, used to wake up from deep sleep
#define D1   5
#define D2   4
#define D3   0 // Connected to FLASH button, boot fails if pulled LOW
#define D4   2 // HIGH at boot, boot fails if pulled LOW
#define D5  14
#define D6  12
#define D7  13
#define D8  15 // Required for boot, boot fails if pulled HIGH


// The stepper motors.
uint8_t pan[4] = {D1, D2, D3, D4};
uint8_t tilt[4] = {D5, D6, D7, D8};

// states to increment a stepper motor 1 tick
uint8_t stepper_states [4][4] =
{
   {HIGH, LOW , LOW , LOW }
  ,{LOW , HIGH, LOW , LOW }
  ,{LOW , LOW , HIGH, LOW }
  ,{LOW , LOW , LOW , HIGH}
};
int speed    = 2500;


void setup() 
{
  RemoteXY_Init (); 
  Serial.begin(9600);

  // Set the pins controling the motors to OUTPUT
  for(int i=0; i<4; i++)
  {
    pinMode(pan[i],OUTPUT);
    pinMode(tilt[i],OUTPUT);
  }
}

void loop() 
{ 
  RemoteXY_Handler ();

  if (RemoteXY.pantilt_x < -30)
  {
    Serial.println("Pan Left");
    for (int row=0; row<4; row++)
    {
      digitalWrite(pan[0], stepper_states[row][0]);
      digitalWrite(pan[1], stepper_states[row][1]);
      digitalWrite(pan[2], stepper_states[row][2]);
      digitalWrite(pan[3], stepper_states[row][3]);
      delayMicroseconds (speed);
    }      
  }
  else if (RemoteXY.pantilt_x > 30)
  {
    Serial.println("Pan Right");
    for (int row=0; row<4; row++)
    {
      digitalWrite(pan[3], stepper_states[row][0]);
      digitalWrite(pan[2], stepper_states[row][1]);
      digitalWrite(pan[1], stepper_states[row][2]);
      digitalWrite(pan[0], stepper_states[row][3]);
      delayMicroseconds (speed);
    } 
  }

  if (RemoteXY.pantilt_y < -30)
  {
      Serial.println("Tilt Up");
    for (int row=0; row<4; row++)
    {
      digitalWrite(tilt[3], stepper_states[row][0]);
      digitalWrite(tilt[2], stepper_states[row][1]);
      digitalWrite(tilt[1], stepper_states[row][2]);
      digitalWrite(tilt[0], stepper_states[row][3]);
      delayMicroseconds (speed);
    }
  }
  else if (RemoteXY.pantilt_y > 30)
  {
    Serial.println("Tilt Down");
    for (int row=0; row<4; row++)
    {
      digitalWrite(tilt[0], stepper_states[row][0]);
      digitalWrite(tilt[1], stepper_states[row][1]);
      digitalWrite(tilt[2], stepper_states[row][2]);
      digitalWrite(tilt[3], stepper_states[row][3]);
      delayMicroseconds (speed);
    } 
  }  
}