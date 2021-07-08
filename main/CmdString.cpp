#include "CmdString.h"
#include <Regexp.h> // Regular expressions

/*MEMBER FUNCTIONS*/
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
  MatchState *ms = new MatchState;
  // "Target" passes string you want to parse to ms object
  ms->Target (cmd_string);

  // "result" is the regular expression to look for in the target.            
  char result = ms->Match(regex, 0);
  
  switch (result) {
    case REGEXP_MATCHED:
      // A "Capture" is a regular expression that's been detected
      // `ms.level` indexes captures
      
      Serial.print("Matched on: ");
      // `GetMatch` finds the index of the match in `buf` and returns matched string
      Serial.println(ms->GetMatch(buf));

      // Detect the number of valid arguments by checking if last 3 captures are numbers
      arg_num = 0;
      if(isdigit(*ms->GetCapture(buf, 5))) {
        arg_num = 3;
      } else if(isdigit(*ms->GetCapture(buf, 4))) {
        arg_num = 2;
      } else if(isdigit(*ms->GetCapture(buf, 3))) {
        arg_num = 1;
      }      

      // Store matched values
      // Cycle through the number of arguments and set to `args`
      for(int i=0; i<arg_num ;i++) {
        args[i] = (ms->GetCapture(buf, 3+i));
      }
      // Set `func` to second capture
      func = (ms->GetCapture(buf, 2));
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
  delete( ms );
}
