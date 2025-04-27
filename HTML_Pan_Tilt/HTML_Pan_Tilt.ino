/*
   Sketch: HTML_Pan_Tilt.ino
   Author: Don Potbury April 2025
   Description: Control the pan and tilt functions on a camera gimbal via a web page

   The code was derived from an example provided by: 
      Rui Santos & Sara Santos - Random Nerd Tutorials
      Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-web-server-outputs-momentary-switch/
      Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
      The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/


#include "ESP8266WiFi.h"
#include "ESPAsyncWebServer.h"
#include <ESPAsyncTCP.h>

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

//---------------------------------------------------------
// Web page related variables
//---------------------------------------------------------
// HTML web page
static const char *index_html PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
  <head>
    <title>ESP Pushbutton Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}
      .button {
        padding: 10px 20px;
        font-size: 24px;
        text-align: center;
        outline: none;
        color: #fff;
        background-color: #2f4468;
        border: none;
        border-radius: 5px;
        box-shadow: 0 6px #999;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }  
      .button:hover {background-color: #1f2e45}
      .button:active {
        background-color: #1f2e45;
        box-shadow: 0 4px #666;
        transform: translateY(2px);
      }
      .center {
        margin-left: auto;
        margin-right: auto;
      }
    </style>
  </head>
  <body>
    <h1>Camera Pan & Tilt Controller</h1>
    <table class="center" width="50%">
      <tr>
        <td colspan="2">
          <button class="button" onmousedown="toggleCheckbox('up');"  ontouchstart="toggleCheckbox('up');"  onmouseup="toggleCheckbox('off');" ontouchend="toggleCheckbox('off');">UP</button>
        </td>
      </tr>
      <tr>
        <td>
          <button class="button" onmousedown="toggleCheckbox('left');"  ontouchstart="toggleCheckbox('left');"  onmouseup="toggleCheckbox('off');" ontouchend="toggleCheckbox('off');">LEFT</button>
        </td>
        <td>
          <button class="button" onmousedown="toggleCheckbox('right');" ontouchstart="toggleCheckbox('right');" onmouseup="toggleCheckbox('off');" ontouchend="toggleCheckbox('off');">RIGHT</button>
        </td>
      </tr>
      <tr>
        <td colspan="2">
          <button class="button" onmousedown="toggleCheckbox('down');"  ontouchstart="toggleCheckbox('down');"  onmouseup="toggleCheckbox('off');" ontouchend="toggleCheckbox('off');">DOWN</button>
        </td>
      </tr>
    </table>

    <script>
    function toggleCheckbox(x) {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/" + x, true);
      xhr.send();
    }
    </script>
  </body>
</html>)rawliteral";

static const size_t html_length = strlen_P(index_html);


//---------------------------------------------------------
// Stepper motor related variables
//---------------------------------------------------------
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








void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String button_pressed = "none";

AsyncWebServer server(80);

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("ESP IP Address: http://");
  Serial.println(WiFi.localIP());
  
  // Set the pins controling the motors to OUTPUT
  for(int i=0; i<4; i++)
  {
    pinMode(pan[i],OUTPUT);
    pinMode(tilt[i],OUTPUT);
  }


  
  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", (uint8_t *)index_html, html_length);
  });

  // Receive an HTTP GET request
  server.on("/left", HTTP_GET, [] (AsyncWebServerRequest *request) {
    button_pressed = "left";
    request->send(200, "text/plain", "ok");
  });

  // Receive an HTTP GET request
  server.on("/up", HTTP_GET, [] (AsyncWebServerRequest *request) {
    button_pressed = "up";
    request->send(200, "text/plain", "ok");
  });

   // Receive an HTTP GET request
  server.on("/left", HTTP_GET, [] (AsyncWebServerRequest *request) {
    button_pressed = "left";
    request->send(200, "text/plain", "ok");
  }); 
  
  // Receive an HTTP GET request
  server.on("/right", HTTP_GET, [] (AsyncWebServerRequest *request) {
    button_pressed = "right";
    request->send(200, "text/plain", "ok");
  });

  // Receive an HTTP GET request
  server.on("/down", HTTP_GET, [] (AsyncWebServerRequest *request) {
    button_pressed = "down";
    request->send(200, "text/plain", "ok");
  });
  
  // Receive an HTTP GET request
  server.on("/off", HTTP_GET, [] (AsyncWebServerRequest *request) {
    Serial.println("OFF");
    button_pressed = "off";
    request->send(200, "text/plain", "ok");
  });
  
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  if (button_pressed == "up")
  {
    Serial.println("up");
    for (int row=0; row<4; row++)
    {
      digitalWrite(tilt[3], stepper_states[row][0]);
      digitalWrite(tilt[2], stepper_states[row][1]);
      digitalWrite(tilt[1], stepper_states[row][2]);
      digitalWrite(tilt[0], stepper_states[row][3]);
      delayMicroseconds (speed);
    }
  }
  else if (button_pressed == "left")
  {
    Serial.println("left");
    for (int row=0; row<4; row++)
    {
      digitalWrite(pan[0], stepper_states[row][0]);
      digitalWrite(pan[1], stepper_states[row][1]);
      digitalWrite(pan[2], stepper_states[row][2]);
      digitalWrite(pan[3], stepper_states[row][3]);
      delayMicroseconds (speed);
    } 
  }
  else if (button_pressed == "right")
  {
    Serial.println("right");
    for (int row=0; row<4; row++)
    {
      digitalWrite(pan[3], stepper_states[row][0]);
      digitalWrite(pan[2], stepper_states[row][1]);
      digitalWrite(pan[1], stepper_states[row][2]);
      digitalWrite(pan[0], stepper_states[row][3]);
      delayMicroseconds (speed);
    } 
  }
  else if (button_pressed == "down")
  {
    Serial.println("down");
    for (int row=0; row<4; row++)
    {
      digitalWrite(tilt[0], stepper_states[row][0]);
      digitalWrite(tilt[1], stepper_states[row][1]);
      digitalWrite(tilt[2], stepper_states[row][2]);
      digitalWrite(tilt[3], stepper_states[row][3]);
      delayMicroseconds (speed);
    } 
  }
  else
  {

  }
}
