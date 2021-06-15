#include <SPI.h>
#include <SD.h>

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


// Globally create objects
Pin pin_2,pin_3;
File myFile;
String dataString;

bool SDCARD = false;
void setup() {
  // Set pin numbers
  pin_2.pin_num = 2;
  pin_3.pin_num = 3;
  
  // Set buad
  Serial.begin(9600);
  
  // Initialize the signalPins as inputs:
  pinMode(pin_2.pin_num, INPUT);
  pinMode(pin_3.pin_num, INPUT);
  pinMode( A0, INPUT );
  
  // Attach an interrupt to the ISR for pin 2
  attachInterrupt(digitalPinToInterrupt(pin_2.pin_num),ISR_2,CHANGE);
  
  // Attach an interrupt to the ISR for pin 3
  attachInterrupt(digitalPinToInterrupt(pin_3.pin_num),ISR_3,CHANGE);

  // Initialize SD card to write to
  //Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("SD Card initialization failed!");
    //while (1);
  }
  Serial.println("SD Card initialization done.");
  SDCARD = true;
  
}

void loop() {
  double dv = ( 5.0 * analogRead( A0 ) ) / 1024.0;
  double bv = dv * 5.7183702642;
  
  // Initialize string to empty
  dataString = "";
  // Create String with interrupt-updated values
  dataString += String(pin_2.freq_time);
  dataString += ",";
  dataString += String(pin_3.freq_time);
  dataString += ",";
  dataString += String( bv );
  
  // Open file on SD. Only one file can be opened at a time!
  if( SDCARD ) {
    myFile = SD.open("test.txt", FILE_WRITE);
    if(myFile) {
      // If file opens, write data
      myFile.println(dataString);
      
      // Close file
      myFile.close();
    } else {
      Serial.println("Error: could not open test.txt");
    }
  }
  Serial.println(dataString);
  
//  Serial.print("Thrust: ");
//  Serial.println(pin_2.freq_time);
//  
//  Serial.print("Aux1: ");
//  Serial.println(pin_3.freq_time);

  // Loop 10 times every second
  delay(50);
}

void ISR_2() {
  pin_2.measure_PWM();
}

void ISR_3() {
  pin_3.measure_PWM();
}
