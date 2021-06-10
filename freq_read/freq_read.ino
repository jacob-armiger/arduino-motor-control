// Print out freq to serial monitor

const int signalPin_2 = 2;     // the number of the interrupt pin
const int signalPin_3 = 3;

volatile byte state_2 = LOW;    //high,low state of pin 2
volatile byte state_3 = LOW;    //high,low state of pin 3

// PWM measurment
unsigned long freq_time = 0;
// Total program time elapsed
unsigned long prog_time = 0;

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
  // Nothing
  }

void ISR_2() {
  // Read whether current signal is high/low
  state_2 = digitalRead(signalPin_2);
  // Track time elapsed at every interrupt call
  
  if(state_2 == HIGH) {
    prog_time = micros();
  } else if(state_2 == LOW) {
    freq_time = micros() - prog_time;
    Serial.print("Thrust: ");
    Serial.println(freq_time);
  }
}

void ISR_3() {
  // Read whether current signal is high/low
  state_3 = digitalRead(signalPin_3);
  // Track time elapsed at every interrupt call
  
  if(state_3 == HIGH) {
    prog_time = micros();
  } else if(state_3 == LOW) {
    freq_time = micros() - prog_time;
    Serial.print("Aux: ");
    Serial.println(freq_time);
  }
}
