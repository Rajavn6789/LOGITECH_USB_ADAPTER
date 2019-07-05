//Logitech Driving Force Shifter USB Adapter
//By Armandoiglesias 2018
//Based on Jason Duncan functionreturnfunction Project
//Video tutorial https://www.youtube.com/watch?v=dLpWEu8kCec
//Use Arduino Leonardo
//Install Joystick Library
//Attribution-NonCommercial-NoDerivatives 4.0 International


#include <Joystick.h>

#define TOTAL_BUTTONS 7

  // Create the Joystick
  Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_GAMEPAD,
  TOTAL_BUTTONS, 0,      // Button Count, Hat Switch Count
  true, true, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering

  // H-shifter mode analog axis thresholds
  #define X_AXIS_12_ZONE         400
  #define X_AXIS_56_ZONE         650
  #define Y_AXIS_TOP_ZONE        800
  #define Y_AXIS_BOTTOM_ZONE     300

  const int ledPin = 11;
  const int xAxisPin = A0;
  const int yAxisPin = A2;
  const int reverseGearPin = 2;

  void setup() {
    pinMode(xAxisPin, INPUT_PULLUP);   // X axis
    pinMode(yAxisPin, INPUT_PULLUP);   // Y axis
    pinMode(reverseGearPin, INPUT); // Reverse

    // Initialize Joystick Library
    Joystick.begin();
  }

  void loop() {
    int x = analogRead(0);                 // X axis
    int y = analogRead(2);                 // Y axis
    int isreverse = digitalRead(2);        // Reverse

    if(isreverse){
      putGear(7);
    } else
    {
      releaseGear(7);
      if(x < X_AXIS_12_ZONE)                  // Shifter on the left?
      {
        if(y > Y_AXIS_TOP_ZONE) putGear(1);
        if(y < Y_AXIS_BOTTOM_ZONE) putGear(2);
      }
      else if(x > X_AXIS_56_ZONE)             // Shifter on the right?
      {
        if(y > Y_AXIS_TOP_ZONE) putGear(5);
        if(y < Y_AXIS_BOTTOM_ZONE) putGear(6);
      }
      else                               // Shifter is in the middle
      {  
        if(y > Y_AXIS_TOP_ZONE) putGear(3);
        if(y < Y_AXIS_BOTTOM_ZONE) putGear(4);
      }
    }
  }

  void putGear(int gearno){
    releaseGearsExceptCurrent(gearno);
    Joystick.setButton(gearno - 1, HIGH);
  }

   void releaseGear(int gearno){
    Joystick.setButton(gearno - 1, LOW);    
  }

  void releaseGearsExceptCurrent(int gearno){
     for(int i = 0; i <= TOTAL_BUTTONS ; i++ ){
      if (i == gearno - 1) continue;
      Joystick.setButton(i, LOW);  
    }  
  }
