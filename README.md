# Camera-Gimbal
 
# Description:
My church had a need for a device to remotely pan and tilt a hand held television camera.  Later the ability zoom and focus was added to the focus.  This repository describes the electronics and software used for the project.   RemoteXY was used for the GUI.

![alt text](<images/Camera Gimbal.jpeg>)

Figure 1.  Front view of the gimbal.

## Pan/Tilt:
### Hardware:
- (1) ESP8266 NodeMCU Development board.
- (3) 28BYJ-48 Stepper motors.
- (2) ULN2003A Stepper motor drivers.

### Circuit:
![alt text](<images/Pan and Tilt Board.jpeg>)

Figure 2. Pan/Tilt board.

![alt text](<images/Pan and Tilt Circuit.png>)

Figure 3. Pan/Tilt circuit.



## Zoom/Focus
### Hardware:
- (1) ESP8266 NodeMCU Development board.
- (1) 28BYJ-48 Stepper motors.
- (1) SG90 9g Micro Servo

### Circuit:
![alt text](<images/Zoom and Focus Board.jpeg>)

Figure 4. Zoom/Focus board

![alt text](<images/Zoom and Focus Circuit.png>)

Figure 5. Zoom/Focus circuit


## Power:
A single 5 volt 2 amp power supply was used to power the entire unit. It uses a 2.1mm barrel jack.  This plugs into the base of the pan/tilt unit.  Power to the zoom/focus unit is supplied by a short cable as shown in figure 6


![Camera Gimbal left side](assets/Camera%20Gimbal%20left%20side.jpeg)
Figure 6. External cable used to power the zoom/focus unit.


![Power Supply](assets/Power%20Supply.jpg)
Figure 7.  DC Power Supply


## Programming:
### Arduino IDE
#### Loading the RemoteXY Library
### RemoteXY
#### Downloading the App
