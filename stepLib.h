#ifndef _stepLib_h_
#define _stepLib_h_

#include "Arduino.h";

// define our stepper class
class stepMotor {
   public:
      stepMotor(byte stepPin, byte dirPin); // our stepper object with variables stepPin and dirPin
      void step(signed int stepFreq); // our stepping function which takes as an input our stepping frequency
   private:
     unsigned long _time; // current time
     unsigned long _lastStepTime; // time at which we last stepped
     unsigned long _stepPeriod; // time between a half period - this is the same as our delay(X) of part 1
     byte _stepPin;
     byte _dirPin;
     boolean _stepCycle; // defines if we are on the HIGH or LOW side of our step cycle
     boolean _dir; // keeps track of which direction we're moving
};


#endif
