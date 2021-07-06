#include "CmdString.h"
#include "Pin.h"
#include "CmdSet.hpp"
#include <Regexp.h> // Regular expressions
#include <SPI.h>
#include <SD.h> // SD card

// Create Objects
CmdString cmd;
Pin pin_2,pin_3;
File myFile;
String dataString;

const int set_size = 5;
struct cmdSet commands[set_size] = {
    { "aRead\0", AnalogRead, 1 },
    { "dRead\0", DigitalRead, 1 },
    { "pinMode\0", mSetPinMode, 2 },
    { "dWrite\0", DigitalWrite, 2 },
    { "aWrite\0", AnalogWrite, 2 },
};

// MAIN
void setup() 
{        
  // ms.Target( "[N]10~[T]1234.123~[C]aRead,10,30,40a0," );
  // char result = ms.Match("%[N%](%d+)~%[T%]([%d.]+)~%[C%]([a-zA-Z]*),*(%d*),*(%d*),*(%d*)")
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
  char regex[] = "%[N%](%d+)~%[T%]([%d.]+)~%[C%]([a-zA-Z]*),*(%d*),*(%d*),*(%d*)";
  
  // Read and store char from Serial port
  cmd.get_serial_input();

  // If full command from Serial port has been read, parse it
  if(cmd.check_complete()) {
    // Parse and store needed values
    cmd.parse_cmd(regex);

    // Search `cmd_set` for values stored from parse
    for(int i = 0; i<set_size; i++) {
      Serial.print("Searching...");

      // If function name matches w/ a function from `cmd_set`, continue
      if(strcmp(cmd.func,commands[i].cmd_name) == 0) {
        // Function has been found in cmdSet
        Serial.print("Func found...");

        // If amount of arguments given match the number required, continue
        if(cmd.arg_num == commands[i].argIn) {
          // Arguments must be converted from String to int
          Serial.println("Correct args...");

          // If 1 or 2 arguements are required
          if(commands[i].argIn == 1) {
            // Function call
            int pin = cmd.args[0].toInt();
            int o = commands[i].f(pin);
            Serial.print("Output: ");
            Serial.println(o);
          } else if(commands[i].argIn == 2) {
            // Function call
            int pin = cmd.args[0].toInt();
            int var = cmd.args[1].toInt();
            Serial.print("Output: ");
            Serial.println(commands[i].f(2,pin,var));
          }
          
        } else {
          Serial.println("Invalid number of arguments.");
        }
        //memset(&cmd.args, 0, 3);
        break;
      } else {
        // Function not found
      }
    }
    
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
