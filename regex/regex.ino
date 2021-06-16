#include <Regexp.h>

void regex_parse( const char * ); // good practice to add function prototype to head
                                  // of file if called prior to definition.

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
    Serial.print( "Test 1: ");
    regex_parse( "asdfasg" );
    
    Serial.print( "Test 2: " );
    regex_parse( "asdfasdf=42" );
    test = false;
  }
}

void regex_parse( const char *tg ) {          // Added input to allow for multiple tests per loop
  // Allocate storage for expected string
  char buf[100];
  
  // Object communicates with library
  MatchState ms;
  // What to parse
  //ms.Target ("Testing: answer=42");
  ms.Target ( tg );                           // pass variable rather than static string for
                                              // ease of testing

  // Match is the expression to parse
  char result = ms.Match("(%a+)=(%d+)", 0);   // this string is looking for a string, then an equal sign
                                              // then a number

  if(result = REGEXP_MATCHED) {
    if( ms.level < 1 ) {                      // this looks like a glitch in the library
      Serial.println( "False positive" );     // if ms.level is zero, it looks like the library thought
      return;                                 // that it found a match, but it didnt really
    }                                         // It matches on the string but failed to complete the parse
    // Captures are the amount of substrings  // The library should really fail on this exception but it doens't
    Serial.print("Captures: ");               // that's why we need to add this check to safety proof
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
