#include "Pin.h"
#include <Regexp.h> // Regular expressions

void Pin::pin_setup(int physical_pin) {
  // Set pin numbers
  pin_num = physical_pin;
  
  // Initialize the signalPins as inputs:
  pinMode(pin_num, INPUT);
}

void Pin::measure_PWM() {
  noInterrupts();
  // Read whether current signal is high/low
  state = digitalRead(pin_num);
  
  if(state == HIGH) {
    // Measure the time at which state goes to high
    prog_time = micros();
  } else if(state == LOW) {
    // Use the time at which state goes low to get total time state was high
    freq_time = micros() - prog_time;
  }
  interrupts();
}
