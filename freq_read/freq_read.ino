// Print out freq to serial monitor

class Pin {
  public:
  /*--CONSTRUCTOR--*/
  Pin() { state = LOW; freq_time = 0; prog_time = millis(); };

  /*--FUNCTION DECLARATIONS--*/
  // To be used within the interrupt
  void measure_PWM();

  /*--VARIABLES--*/
  // Pin number on board
  int pin_num;
  
  // High/low state of pin
  volatile byte state;

  // PWM measurment
  unsigned long freq_time;
  // Total program time elapsed
  unsigned long prog_time;

};

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


// Globally create pin objects
Pin pin_2,pin_3;

void setup() {
  // Set pin numbers
  pin_2.pin_num = 2;
  pin_3.pin_num = 3;
  
  // Set buad
  Serial.begin(9600);
  
  // Initialize the signalPins as inputs:
  pinMode(pin_2.pin_num, INPUT);
  pinMode(pin_3.pin_num, INPUT);
  
  // Attach an interrupt to the ISR for pin 2
  attachInterrupt(digitalPinToInterrupt(pin_2.pin_num),ISR_2,CHANGE);
  
  // Attach an interrupt to the ISR for pin 3
  attachInterrupt(digitalPinToInterrupt(pin_3.pin_num),ISR_3,CHANGE);
}

void loop() {
    Serial.print("Thrust: ");
    Serial.println(pin_2.freq_time);
    
    Serial.print("Aux1: ");
    Serial.println(pin_3.freq_time);

    // Loop 10 times every second
    delay(100);
}

void ISR_2() {
  pin_2.measure_PWM();
}

void ISR_3() {
  pin_3.measure_PWM();
}
