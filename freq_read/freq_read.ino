// Print out freq to serial monitor

const int signalPin_2 = 2;     // the number of the interrupt pin
const int signalPin_3 = 3;

volatile byte state_2 = LOW;    //high,low state of pin 2
volatile byte state_3 = LOW;    //high,low state of pin 3

// PWM measurment
unsigned long freq_time = 0;
// Total program time elapsed
unsigned long prog_time = 0;

// PWM measurment
unsigned long freq_time_3 = 0;
// Total program time elapsed
unsigned long prog_time_3 = 0;

void setup() {
  // Set buad
  Serial.begin(9600);
  
  // Initialize the signalPins as inputs:
  pinMode(signalPin_2, INPUT);
  pinMode(signalPin_3, INPUT);
  
  // Attach an interrupt to the ISR for pin 2
  attachInterrupt(digitalPinToInterrupt(signalPin_2),ISR_2,CHANGE);
  
  // Attach an interrupt to the ISR for pin 3
  attachInterrupt(digitalPinToInterrupt(signalPin_3),ISR_3,CHANGE);
}

void loop() {
    Serial.print("thrust: ");
    Serial.println(freq_time);
    Serial.print("aux: ");
    Serial.println(freq_time_3);

    // Loop 10 times every second
    delay(100);
}

void ISR_2() {
  noInterrupts();
  // Read whether current signal is high/low
  state_2 = digitalRead(signalPin_2);
  
  if(state_2 == HIGH) {
    // Measure the time at which state goes to high
    prog_time = micros();
  } else if(state_2 == LOW) {
    // Use the time at which state goes low to get total time state was high
    freq_time = micros() - prog_time;
  }
  interrupts();
}

void ISR_3() {
  noInterrupts();
  // Read whether current signal is high/low
  state_3 = digitalRead(signalPin_3);
  
  if(state_3 == HIGH) {
    // Measure the time at which state goes to high
    prog_time_3 = micros();
  } else if(state_3 == LOW) {
    // Use the time at which state goes low to get total time state was high
    freq_time_3 = micros() - prog_time_3;
  }
  interrupts();
}
