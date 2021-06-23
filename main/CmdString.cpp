#include "CmdString.h"
#include <Regexp.h> // Regular expressions


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

void CmdString::parse_cmd(const char *regex) {
  // Allocate storage for expected string
  char buf[100];
  
  // Object communicates with Regexp library
  MatchState ms;
  // "Target" passes string you want to parse to ms object
  ms.Target (cmd_string);

  // "result" is the regular expression to look for in the target.            
  char result = ms.Match(regex, 0);
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
