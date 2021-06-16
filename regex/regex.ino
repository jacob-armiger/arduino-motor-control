#include <Regexp.h>

bool test = true;

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

  // If full message is obtained, parse the command
  if(test) {
    regex_parse();
    test = false;
  }
}

void regex_parse() {
  // Allocate storage for expected string
  char buf[100];
  
  // Object communicates with library
  MatchState ms;
  // What to parse
  //ms.Target ("Testing: answer=42");
  ms.Target ("fhausdfhs");

  // Match is the expression to parse
  char result = ms.Match("(%a+)=(%d+)", 0);

  if(result = REGEXP_MATCHED) {
    // Captures are the amount of substrings
    Serial.print("Captures: ");
    // ms.level indexes captures
    Serial.println(ms.level);

    Serial.print ("Matched on: ");
    // GetMatch traverses to index of the match and returns matched string
    Serial.println (ms.GetMatch(buf));
    
  } else if (result == REGEXP_NOMATCH) {
    Serial.println("No Match");
  } else {
    // Errors defined in Regexp.h
    Serial.println("ERROR");
  }
}
