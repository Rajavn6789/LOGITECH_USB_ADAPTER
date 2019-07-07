//Logitech Driving Force Shifter USB Adapter
//By Armandoiglesias 2018
//Based on Jason Duncan functionreturnfunction Project
//Video tutorial https://www.youtube.com/watch?v=dLpWEu8kCec
//Use Arduino Leonardo
//Install Joystick Library
//Attribution-NonCommercial-NoDerivatives 4.0 International
// Removed all sequential logic, since https://www.thrustmaster.com/en_US/products/ferrari-racing-wheel-red-legend-edition has inbuilt sequential gears

#include <Joystick.h>
#include <LedControl.h>

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

  // Constants
  const int xAxisPin = A0;
  const int yAxisPin = A1;
  const int reverseGearPin = 2;
  const int DIN = 12;
  const int CS =  11;
  const int CLK = 10;

  byte neutral[8]={0x66,0x76,0x76,0x7e,0x6e,0x6e,0x66,0x66};
  byte one[8]={0x10,0x30,0x10,0x10,0x10,0x10,0x10,0x38};
  byte two[8]={0x38,0x44,0x04,0x04,0x08,0x10,0x20,0x7C};
  byte three[8]={0x38,0x44,0x04,0x18,0x04,0x04,0x44,0x38};
  byte four[8]={0x04,0x0C,0x14,0x24,0x44,0x7E,0x04,0x04};
  byte five[8]={0x7C,0x40,0x40,0x78,0x04,0x04,0x44,0x38};
  byte six[8]={0x38,0x44,0x40,0x78,0x44,0x44,0x44,0x38};
  byte reverse[8]={0x7e,0x7e,0x62,0x7e,0x7c,0x66,0x66,0x66};

  LedControl lc=LedControl(DIN,CLK,CS,1);


  void setup() {
    pinMode(xAxisPin, INPUT_PULLUP);   // X axis
    pinMode(yAxisPin, INPUT_PULLUP);   // Y axis
    pinMode(reverseGearPin, INPUT); // Reverse

    // Initialize Joystick Library
    Joystick.begin();
    
    // Initialize LedControl Library
    lc.shutdown(0,false);       //The MAX72XX is in power-saving mode on startup
    lc.setIntensity(0,1);      // Set the brightness to maximum value
    lc.clearDisplay(0);         // and clear the display
  }

  void loop() {
    int x = analogRead(0);                 // X axis
    int y = analogRead(1);                 // Y axis
    int isreverse = digitalRead(2);        // Reverse

    if(isreverse){
      putGear(7);
    } else
    {
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
      else                                    // Shifter is in the middle
      {  
        if(y > Y_AXIS_TOP_ZONE) putGear(3);
        if(y < Y_AXIS_BOTTOM_ZONE) putGear(4);
      }
    }
  }

  void putGear(int gearno){
    releaseGearsExceptCurrent(gearno);
    displayGearInLEDMatrix(gearno);
    Joystick.setButton(gearno - 1, HIGH);
  }

   void releaseGearsExceptCurrent(int gearno){
     for(int i = 0; i <= TOTAL_BUTTONS ; i++ ){
      if (i == gearno - 1) continue;
      Joystick.setButton(i, LOW);  
    }  
  }

  void displayGearInLEDMatrix(int gearno){
    switch(gearno){
      case 1:
      printByte(one);  
      break;
    
      case 2:
      printByte(two);  
      break;
      
      case 3:
      printByte(three);  
      break;
      
      case 4:
      printByte(four);  
      break;
      
      case 5:
      printByte(five);  
      break;
      
      case 6:
      printByte(six);  
      break;
      
      case 7:
      printByte(reverse);  
      break;
      
      default:
      printByte(neutral); 
    }
  }

  void printByte(byte character []){
    int i = 0;
    for(i=0; i < 8; i++)
    lc.setRow(0,i,character[i]);
  }
