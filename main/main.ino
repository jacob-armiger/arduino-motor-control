#include "CmdString.h"
#include "Pin.h"
#include <Regexp.h> // Regular expressions
#include <SPI.h>
#include <SD.h> // SD card


// Create Objects
CmdString cmd;
Pin pin_2,pin_3;
File myFile;
String dataString;

// MAIN
void setup() 
{        
  // ms.Target( "[N]10~[T]1234.123~[C]aRead,10,30,40a0," );
  // char result = ms.Match("%[N%](%d+)~%[T%]([%d.]+)~%[C%]([a-zA-Z]*),*(%d*),*(%d*),*(%d*)")
  // loop through cmdSet to look for third ms.level
  Serial.begin(9600);
  init_SD();
  
  // Set pin numbers
  pin_2.pin_setup(2);
  pin_3.pin_setup(3);
  
  // Attach an interrupt to the ISR for pin 2 and 3
  attachInterrupt(digitalPinToInterrupt(pin_2.pin_num),ISR_2,CHANGE);
  attachInterrupt(digitalPinToInterrupt(pin_3.pin_num),ISR_3,CHANGE);

}
void loop() 
{
  // Regular expression used to parse
  char regex[] = "(%a+)=(%d+)";
  
  // Read and store char from Serial port
  cmd.get_serial_input();

  // If full command from Serial port has been read, parse it
  if(cmd.check_complete()) {
    cmd.parse_cmd(regex);
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
    //Serial.println(dataString);
    // Close file
    myFile.close();
  } else {
    Serial.println("Error: could not open test.txt");
  }

  // Loop 10 times every second
  delay(100);
}

// INTERRUPT SERVICE ROUTINES
void ISR_2() {
  pin_2.measure_PWM();
}

void ISR_3() {
  pin_3.measure_PWM();
}

// SD CARD INITIALIZATION
void init_SD () {
  // Delays are included so that printing isn't skipped
  delay(1000);
  // Initialize SD card to write to
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  delay(1000);
}
