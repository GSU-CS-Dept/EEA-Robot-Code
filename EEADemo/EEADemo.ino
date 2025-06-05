#include <avr/wdt.h>
#include "DeviceDriverSet_xxx0.h"
#include "ApplicationFunctionSet_xxx0.cpp"
#include "FastLED.h"
#include "MPU6050.h"
#include "MPU6050_getdata.h"

// LED setup
#define PIN_RBGLED 4
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

// battery setup
#define VOL_MEASURE_PIN A3
unsigned long vol_measure_time = 0;

// gyro, motors, servo, ultrasonic setup
MPU6050_getdata AppMPU6050getdata;
DeviceDriverSet_Motor AppMotor;
Application_xxx Application_SmartRobotCarxxx0;
DeviceDriverSet_Servo AppServo;
DeviceDriverSet_ULTRASONIC myUltrasonic;

void setup() {
  // initialize LED on main board
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, NUM_LEDS);
  FastLED.setBrightness(20);

  // init ultrasonic
  myUltrasonic.DeviceDriverSet_ULTRASONIC_Init();

  // init gyro
  AppMPU6050getdata.MPU6050_dveInit();
  delay(1000);
  AppMPU6050getdata.MPU6050_calibration();

  // init battery read
  voltageInit();

  // start and initialize motor
  AppMotor.DeviceDriverSet_Motor_Init();

  // wait 1 seconds
  delay(1000);

  // move car back and forth to verify movement
  int motorSpeed = 50; // change this value to speed up or slow down car, min 0, max of 255
  for (Application_SmartRobotCarxxx0.Motion_Control = 0; Application_SmartRobotCarxxx0.Motion_Control < 9; Application_SmartRobotCarxxx0.Motion_Control = Application_SmartRobotCarxxx0.Motion_Control + 1)
  {
    ApplicationFunctionSet_SmartRobotCarMotionControl(Application_SmartRobotCarxxx0.Motion_Control /*direction*/, motorSpeed /*speed*/);
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

  // change color of LED
  myColor();

}

void loop() {
  // measure voltage
  voltageMeasure();

  // print yaw
  static float Yaw;
  AppMPU6050getdata.MPU6050_dveGetEulerAngles(&Yaw);
  Serial.print("Yaw:");
  Serial.println(Yaw);

  // check ultrasonic
  myUltrasonic.DeviceDriverSet_ULTRASONIC_Test();
  delay(500);
}

// LED functions
uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
{
  return (((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
}

void myColor()
{
  int r = 255, g = 0, b = 0;
  for (int i = 0; i < 255; i++)
  {
    FastLED.showColor(Color(r, g, b));
    r -= 1;
    g += 1;
    delay(10);
  }
  r = 0, g = 0, b = 255;

  for (int i = 0; i < 255; i++)
  {
    FastLED.showColor(Color(r, g, b));
    r += 1;
    b -= 1;
    delay(10);
  }
  r = 0, g = 255, b = 0;

  for (int i = 0; i < 255; i++)
  {
    FastLED.showColor(Color(r, g, b));
    g -= 1;
    b += 1;
    delay(10);
  }
  r = 0, g = 0, b = 0;
}

// Voltage functions below
void voltageInit()
{
  pinMode(VOL_MEASURE_PIN, INPUT);
}

void voltageMeasure()
{
  if (millis() - vol_measure_time > 1000) //Measured every 1000 milliseconds
  {
    vol_measure_time = millis();
    float voltage = (analogRead(VOL_MEASURE_PIN) * 5 ) * ((10 + 1.5) / 1.5) / 1024; //Read voltage value
    //float voltage = (analogRead(VOL_MEASURE_PIN) * 0.0375);
    voltage = voltage + (voltage * 0.08);
    Serial.print("Current voltage value : ");
    Serial.println(voltage);
    float diff = 5 - voltage;
    if (diff > 1.0)
      Serial.println("Voltage is low, should be 5v");
    else if (diff < -1.0)
      Serial.println("Voltage is high, should be 5v");
    else
      Serial.println("Voltage is fine");
  }
}
