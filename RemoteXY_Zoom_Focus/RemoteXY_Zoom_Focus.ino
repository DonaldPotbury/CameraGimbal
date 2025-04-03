/*
   This script is the Zoom & Focus module of the camera gimble.
   
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
#include <Servo.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "TMOBILE-0F51"
#define REMOTEXY_WIFI_PASSWORD "dispose.triage.scorch.corndog!"
#define REMOTEXY_SERVER_PORT 6377
#define REMOTEXY_ACCESS_PASSWORD "1"


#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 91 bytes
  { 255,4,0,0,0,84,0,19,0,0,0,122,111,111,109,95,102,111,99,117,
  115,0,31,1,106,200,1,1,6,0,129,27,134,46,17,64,17,70,111,99,
  117,115,0,1,8,154,24,24,0,2,31,0,1,71,153,24,24,0,2,31,
  0,1,39,39,24,24,0,2,31,0,1,39,73,24,24,0,2,31,0,129,
  25,19,47,18,64,17,90,111,111,109,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t focus_left; // =1 if button pressed, else =0
  uint8_t focus_right; // =1 if button pressed, else =0
  uint8_t zoom_in; // =1 if button pressed, else =0
  uint8_t zoom_out; // =1 if button pressed, else =0

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

// The elements in this array are the pins on the ESP8266
uint8_t motor[4] = {D5, D6, D7, D8};

// states to increment the stepper 1 tick
uint8_t stepper_states [4][4] =
{
   {HIGH, LOW , LOW , LOW }
  ,{LOW , HIGH, LOW , LOW }
  ,{LOW , LOW , HIGH, LOW }
  ,{LOW , LOW , LOW , HIGH}
};

int speed    = 2500;

Servo zoom_servo;
int pos = 90;

void setup() 
{
  RemoteXY_Init (); 
  Serial.begin(9600);
  Serial.println();

  pinMode(D4, OUTPUT);
  zoom_servo.attach(D4);
  zoom_servo.write(pos);

  for(int i=0; i<4; i++)
  {
    pinMode(motor[i],OUTPUT);
  }  
}

void loop() 
{ 
  RemoteXY_Handler ();
  Serial.print(RemoteXY.focus_left);
  Serial.print(", ");
  Serial.println(RemoteXY.focus_right); 

  if (RemoteXY.focus_left == 1 && RemoteXY.focus_right == 0)
  {
    for (int row=0; row<4; row++)
    {
      digitalWrite(motor[0], stepper_states[row][0]);
      digitalWrite(motor[1], stepper_states[row][1]);
      digitalWrite(motor[2], stepper_states[row][2]);
      digitalWrite(motor[3], stepper_states[row][3]);
      delayMicroseconds (speed);
    } 
  }
  else if (RemoteXY.focus_left == 0 && RemoteXY.focus_right == 1)
  {
    for (int row=0; row<4; row++)
    {
      digitalWrite(motor[3], stepper_states[row][0]);
      digitalWrite(motor[2], stepper_states[row][1]);
      digitalWrite(motor[1], stepper_states[row][2]);
      digitalWrite(motor[0], stepper_states[row][3]);
      delayMicroseconds (speed);
    } 
  }

  if (RemoteXY.zoom_in == 1 && RemoteXY.zoom_out == 0)
  {
    pos = 0;
    zoom_servo.write(pos);
  }
  else if (RemoteXY.zoom_in == 0 && RemoteXY.zoom_out == 1)
  {
    pos = 180;
    zoom_servo.write(pos);
  }
  else
  {
    pos = 90;
    zoom_servo.write(pos);
  }
}