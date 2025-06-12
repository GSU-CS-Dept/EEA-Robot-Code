#include <avr/wdt.h>
#include "DeviceDriverSet_xxx0.h"
#include "ApplicationFunctionSet_xxx0.cpp"

// Robot Directions
#define FORWARD 0
#define BACKWARD 1
#define LEFT 2    
#define RIGHT 3   
#define LEFTFORWARD 4
#define LEFTBACKWARD 5
#define RIGHTFORWARD 6
#define RIGHTBACKWARD 7
#define STOP 8

// Motor setup
DeviceDriverSet_Motor AppMotor;
Application_xxx Application_SmartRobotCarxxx0;

void setup() {
  // initialize communication on main board
  Serial.begin(9600);

  // start and initialize motor
  AppMotor.DeviceDriverSet_Motor_Init();

  // wait 1 seconds
  delay(1000);

  // set movement speed (150 @ minimum, movements get less precise the slower it goes, 255 is optimal)
  uint8_t motorSpeed = 255; // change this value to speed up or slow down car, min 0, max of 255

  // move forward
  ApplicationFunctionSet_SmartRobotCarMotionControl(FORWARD /*direction*/, motorSpeed /*speed*/);
  delay(2500); // wait
  
  // turn 
  ApplicationFunctionSet_SmartRobotCarMotionControl(RIGHT /*direction*/, motorSpeed /*speed*/);
  delay(3000); // wait

  // move forward
  ApplicationFunctionSet_SmartRobotCarMotionControl(FORWARD /*direction*/, motorSpeed /*speed*/);
  delay(2500); // wait

  // turn 
  ApplicationFunctionSet_SmartRobotCarMotionControl(RIGHT /*direction*/, motorSpeed /*speed*/);
  delay(3000); // wait

  // move forward
  ApplicationFunctionSet_SmartRobotCarMotionControl(FORWARD /*direction*/, motorSpeed /*speed*/);
  delay(2500); // wait

  // turn 
  ApplicationFunctionSet_SmartRobotCarMotionControl(RIGHT /*direction*/, motorSpeed /*speed*/);
  delay(3000); // wait

  // move forward
  ApplicationFunctionSet_SmartRobotCarMotionControl(FORWARD /*direction*/, motorSpeed /*speed*/);
  delay(2500); // wait
  
  // stop smart car
  ApplicationFunctionSet_SmartRobotCarMotionControl(STOP /*direction*/, motorSpeed /*speed*/);

}

void loop() {
 // stuff to do "forever"
 
}
