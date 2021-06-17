#include <Regexp.h>

// FUNCTION PROTOTYPES
void regex_parse(const char *);

// Serial port input buffer
char cmd_string[50];
int index = 0;

// MAIN
void setup() {
  Serial.begin(9600);
}
void loop() {
  char letter;
  bool cmd_end = false;

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

  // If full input is obtained, parse the command
  if(cmd_end) {
    // Parse input
    regex_parse(cmd_string);

    // Reset c_string buffer and index
    memset(&cmd_string, 0, 50);
    index = 0;
  }
}


// FUNCTION DEFINITIONS
void regex_parse(const char *target) {
  // Allocate storage for expected string
  char buf[100];
  
  // Object communicates with Regexp library
  MatchState ms;
  // "Target" passes string you want to parse to ms object
  ms.Target (target);

  // "result" is the regular expression to look for in the target.
  //                   (string)=(float)
  char result = ms.Match("(%a+)=(%d+)", 0);

  switch (result) {
    case REGEXP_MATCHED:
      // Captures are the amount of substrings
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
}
