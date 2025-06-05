#include <avr/wdt.h>
#include "DeviceDriverSet_xxx0.h"
#include "ApplicationFunctionSet_xxx0.cpp"
#include "FastLED.h"

// LED setup
#define PIN_RBGLED 4
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

// motors and servo setup
DeviceDriverSet_Motor AppMotor;
Application_xxx Application_SmartRobotCarxxx0;
DeviceDriverSet_Servo AppServo;

void setup() {
  // initialize LED on main board
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, NUM_LEDS);
  FastLED.setBrightness(20);
  
  // start and initialize motor
  AppMotor.DeviceDriverSet_Motor_Init();
  
  // wait 1 seconds
  delay(1000);

  // move in all directions
  for (Application_SmartRobotCarxxx0.Motion_Control = 0; Application_SmartRobotCarxxx0.Motion_Control < 9; Application_SmartRobotCarxxx0.Motion_Control = Application_SmartRobotCarxxx0.Motion_Control + 1)
  {
    ApplicationFunctionSet_SmartRobotCarMotionControl(Application_SmartRobotCarxxx0.Motion_Control /*direction*/, 200 /*speed*/);
    delay(1000); // wait 1 second
  }

  // initialize servo motor for camera
  AppServo.DeviceDriverSet_Servo_Init(90);

  // spin servo around in 10 degree increments
  for (int i = 0; i <= 180; i += 10)
  {
    AppServo.DeviceDriverSet_Servo_control(i);
  }

  // spin back in 10 degree increments
  for (int i = 180; i >= 90; i -= 10)
  {
    AppServo.DeviceDriverSet_Servo_control(i);
  }
}

uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
{
  return (((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
}

void myColor()
{
  int r=255,g=0,b=0;
    for(int i=0;i<255;i++)
    {
      FastLED.showColor(Color(r, g, b));
      r-=1;
      g+=1;
      delay(10);
    }
    r=0,g=0,b=255;
    
    for(int i=0;i<255;i++)
    {
      FastLED.showColor(Color(r, g, b));
      r+=1;
      b-=1;
      delay(10);
    }
    r=0,g=255,b=0;

    for(int i=0;i<255;i++)
    {
      FastLED.showColor(Color(r, g, b));
      g-=1;
      b+=1;
      delay(10);
    }
    r=0,g=0,b=0;
}

void loop() {
  // change color of LED
  myColor();
}
