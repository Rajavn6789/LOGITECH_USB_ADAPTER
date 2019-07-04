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
  Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
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

  void setup() {
    pinMode(A0, INPUT_PULLUP);   // X axis
    pinMode(A2, INPUT_PULLUP);   // Y axis
    pinMode(2, INPUT); // Reverse

    // Initialize Joystick Library
    Joystick.begin();
  }

  void loop() {
    int x = analogRead(0);                 // X axis
    int y = analogRead(2);                 // Y axis

    int _isreverse = digitalRead(2);

    if( _isreverse == 1 ){
      putGear(7);
    } else
    {
      if(x < X_AXIS_12_ZONE)                  // Shifter on the left?
      {
        if(y > Y_AXIS_TOP_ZONE) putGear(1);       // 1st gear
        if(y < Y_AXIS_BOTTOM_ZONE) putGear(2);       // 2nd gear
      }
      else if(x > X_AXIS_56_ZONE)             // Shifter on the right?
      {
        if(y > Y_AXIS_TOP_ZONE) putGear(5);       // 5th gear
        if(y < Y_AXIS_BOTTOM_ZONE) putGear(6);       // 6th gear
      }
      else                               // Shifter is in the middle
      {
        if(y > HS_YAXIS_135) putGear(3);       // 3rd gear
        if(y < HS_YAXIS_246) putGear(4);       // 4th gear
      }
    }
    delay(50);
  }

  void putGear(gearno){
    // Clear all gears
    for(int i = 0; i <= TOTAL_BUTTONS ; i++ )
    Joystick.setButton(i, LOW);

    // Put gear
    Joystick.setButton(gearno - 1, HIGH);

    delay(50);
  }
