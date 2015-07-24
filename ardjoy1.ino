#include <Stepper.h>

#include "stepLib.h";
#include "smoothJoystick.h";

#define joystickPin 18 // analog pin A4 (27)

#define maxSpeed 20000 // speed measured in Hz
#define minSpeed 1 // speed measured in Hz

#define deadband 30 // deadband for consideration of 0 speed

// define our step pins
#define sliderStep 52 // 52

// define our direction pins
#define sliderDir 53 // 53

// instantiate out objects
stepMotor slider(sliderStep, sliderDir); // stepper motor
joystick joy(joystickPin, 512); // declare a new instance of a joystick on our joystickPin, let's initialize our buffer to center joystick value: 512


void setup() {

//DAN allows screen printing  
Serial.begin(115200); // begin serial output @ 115200 baud

}

void loop() {

  unsigned int smoothAnalogVal = joy.smoothRead(); // get our smoothed analog value
  signed int stepperSpeed = convertAnalogToSpeed(smoothAnalogVal); // convert our analog value to a stepper speed with direction
  // DAN
  Serial.println(smoothAnalogVal);
  Serial.println(stepperSpeed);
  slider.step(stepperSpeed*2); // request a step with our joystick driven speed

}


// this function takes as an input, an unsigned analog value and will convert it into a signed value which represents stepper speed
// the output is bounded by a min and max stepper speed
signed int convertAnalogToSpeed(unsigned int analogVal) {

  if (analogVal < 512 + deadband && analogVal > 512 - deadband) { // we are within deadband, return a 0 speed
    return 0;
  } else if (analogVal >= 512 + deadband) { // move in forward direction - return a positive speed
    return map(analogVal, 512 + deadband, 1024, minSpeed, maxSpeed); // interpolate our joystick value to the proper speed
  } else if (analogVal <= 512 - deadband) { // move in reverse direction - return a negative speed
    return -map(analogVal, 512 - deadband, 0, minSpeed, maxSpeed); // interpolate our joystick value to the proper speed
  }

}
