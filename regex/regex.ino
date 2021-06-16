#include <Regexp.h>

bool test = true;

// FUNCTION PROTOTYPES
void regex_parse(const char *);


// MAIN
void setup() {
  Serial.begin(9600);
  Serial.println();
}
void loop() {
  // TODO: Get characters from Serial Port until end of string
  //  char letter;
  //  Serial.println("Enter your name: ");
  //  while(Serial.available() > 0) {
  //    letter
  //  }
  
  String test_string = "b=12";

  // If full message is obtained, parse the command
  if(test) {
    regex_parse(test_string.c_str());
    test = false;
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
