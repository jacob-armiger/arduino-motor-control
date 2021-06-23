#pragma once
#include "Arduino.h"

class Pin {
  public:
  /*CONSTRUCTOR*/
  Pin() { state = LOW; freq_time = 0; prog_time = millis(); };

  /*FUNCTION DECLARATIONS*/
  // To be used within the interrupt
  void measure_PWM();
  void pin_setup(int);

  /*VARIABLES*/
  // Pin number on board
  int pin_num;
  
  // High/low state of pin
  volatile byte state;

  // PWM measurment
  unsigned long freq_time;
  // Total program time elapsed
  unsigned long prog_time;

};
