#pragma once
#include "Arduino.h"

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
  void parse_cmd(const char*);

  private:
  // Reset c_string buffer and index
  void reset() {memset(&cmd_string, 0, 50); index = 0; cmd_end = false;}
  
  /*VARIABLES*/
  char cmd_string[50];
  char letter;
  int index = 0;
  bool cmd_end = false;
  
};
