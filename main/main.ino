#include <Regexp.h> // Regular expressions
#include <SPI.h>
#include <SD.h> // SD card

class CmdString {
  public:
  /*CONSTRUCTOR*/
  CmdString() { index = 0; cmd_end = false; };
              
  /*FUNCTION DECLARATIONS*/
  // Read and store Serial input
  void get_serial_input();
  // Check to see if done reading Serial input
  bool check_complete();
  // Parse Serial input
  void parse_cmd();

  private:
  // Reset c_string buffer and index
  void reset() {memset(&cmd_string, 0, 50); index = 0; cmd_end = false;}
  
  /*VARIABLES*/
  char cmd_string[50];
  char letter;
  int index = 0;
  bool cmd_end = false;
  
};

class Pin {
  public:
  /*CONSTRUCTOR*/
  Pin() { state = LOW; freq_time = 0; prog_time = millis(); };

  /*FUNCTION DECLARATIONS*/
  // To be used within the interrupt
  void measure_PWM();

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

// Create Objects
CmdString cmd;
Pin pin_2,pin_3;
File myFile;
String dataString;

// MAIN
void setup() 
{
  Serial.begin(9600);
  
  // Set pin numbers
  pin_2.pin_num = 2;
  pin_3.pin_num = 3;
  
  // Initialize the signalPins as inputs:
  pinMode(pin_2.pin_num, INPUT);
  pinMode(pin_3.pin_num, INPUT);
  
  // Attach an interrupt to the ISR for pin 2
  attachInterrupt(digitalPinToInterrupt(pin_2.pin_num),ISR_2,CHANGE);
  
  // Attach an interrupt to the ISR for pin 3
  attachInterrupt(digitalPinToInterrupt(pin_3.pin_num),ISR_3,CHANGE);

  // Initialize SD card to write to
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
}
void loop() 
{
  cmd.get_serial_input();
  
  if(cmd.check_complete()) {
    cmd.parse_cmd();
  }

  // Initialize string to empty
  dataString = "";
  // Create String with interrupt-updated values
  dataString += String(pin_2.freq_time);
  dataString += ",";
  dataString += String(pin_3.freq_time);
  
  // Open file on SD. Only one file can be opened at a time!
  myFile = SD.open("test.txt", FILE_WRITE);
  if(myFile) {
    // If file opens, write data
    myFile.println(dataString);
    Serial.println(dataString);
    // Close file
    myFile.close();
  } else {
    Serial.println("Error: could not open test.txt");
  }
  
//  Serial.print("Thrust: ");
//  Serial.println(pin_2.freq_time);
//  
//  Serial.print("Aux1: ");
//  Serial.println(pin_3.freq_time);

  // Loop 10 times every second
  delay(100);
}


void ISR_2() {
  pin_2.measure_PWM();
}

void ISR_3() {
  pin_3.measure_PWM();
}

// FUNCTION MEMBER DEFINITIONS
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

void CmdString::get_serial_input() {
  // Check for Serial port input
  if(Serial.available() > 0) {
    // Read one byte from Serial port
    letter = Serial.read();
    
    // Assign c_string at index to `letter` and increment index
    cmd_string[index] = letter;
    ++index;

    // Detect delimiter 'new line', which is the end of input
    if(letter == '\n') {
      cmd_end = true;
    }
  }
}

bool CmdString::check_complete() {
  // Return true if end of serial input has been reached
  return cmd_end;
}

void CmdString::parse_cmd() {
  // Allocate storage for expected string
  char buf[100];
  
  // Object communicates with Regexp library
  MatchState ms;
  // "Target" passes string you want to parse to ms object
  ms.Target (cmd_string);

  // "result" is the regular expression to look for in the target.            
  char result = ms.Match("(%a+)=(%d+)", 0);
                     /*(string)=(float)*/
  switch (result) {
    case REGEXP_MATCHED:
      // "Captures" are the amount of substrings
      Serial.print("Captures: ");
      // ms.level indexes captures
      Serial.println(ms.level);

      Serial.print ("Matched on: ");
      // GetMatch traverses to index of the match and returns matched string
      Serial.println (ms.GetMatch(buf));
      break;
      
    case REGEXP_NOMATCH:
      Serial.println("No Match");
      break;
  
    default:
      Serial.print ("Regexp error: ");
      Serial.println (result, DEC);
      break;
  }
  // Command has been parsed and buffers can be reset
  reset();
}