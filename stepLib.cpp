#include "Arduino.h"
#include "stepLib.h"


// DAN digital write direct (copied from online)
inline void digitalWriteDirect(int pin, boolean val){
  if(val) g_APinDescription[pin].pPort -> PIO_SODR = g_APinDescription[pin].ulPin;
  else    g_APinDescription[pin].pPort -> PIO_CODR = g_APinDescription[pin].ulPin;
}

inline int digitalReadDirect(int pin){
  return !!(g_APinDescription[pin].pPort -> PIO_PDSR & g_APinDescription[pin].ulPin);
}


// used for declaring our motor and initializing it
stepMotor::stepMotor(byte stepPin, byte dirPin) {

   _stepPin = stepPin;
   _dirPin = dirPin;

   // define our digital pins as output
   pinMode(_stepPin, OUTPUT);
   pinMode(_dirPin, OUTPUT);

   // initialize our digital pins to LOW
   digitalWriteDirect(_stepPin, LOW);
   digitalWriteDirect(_dirPin, LOW);

   _stepCycle = false; // this keeps track of which end of the step cycle we are on: high or low
   _dir = false;
}


// function responsible for driving our digital pins high/low at the proper frequency
// input is the stepping frequency
void stepMotor::step(signed int stepFreq) {

   _time = micros(); // get the current time in microseconds

  if (stepFreq != 0) { // we don't want to divide by 0 when joystick is at center
     _stepPeriod = 1000000 / abs(stepFreq); // get our step period (in micro-seconds) from the user given step frequency; we lose a bit of accuracy here since we've defined _stepPeriod as an unsigned long instead of a float, but that's ok...
  }

   // set our direction pin properly, we will only set this once for every requested direction change to save on CPU time
   if (_dir == false && stepFreq < 0) { // we were previously moving backwards, and are now requesting to move forward
     digitalWriteDirect(_dirPin, HIGH);
     _dir = true;
   } else if (_dir == true && stepFreq > 0) { // we were previously moving forward, and are now requestion to move backward
     digitalWriteDirect(_dirPin, LOW);
     _dir = false;
   }

   // if the proper amount of time has passed and we are requesting a speed, let's go ahead and proceed to the next half of our step cycle
   if (stepFreq != 0 && _time >= _lastStepTime + _stepPeriod) {
      digitalWriteDirect(_stepPin, _stepCycle == true); // a compact way of writing either HIGH/LOW to our step pin based on where we are on our step cycle

      _stepCycle = !_stepCycle; // this simply flips our Boolean // DAN: wtf, why are we doing this in two steps, this is stupid.

      _lastStepTime = _time; // update the time we last stepped
   }

}
