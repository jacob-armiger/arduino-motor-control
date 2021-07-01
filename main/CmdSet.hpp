#pragma once
#include "Arduino.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>
#include <stdarg.h>

struct cmdSet {
  char cmd_name[ 10 ];
  int (*f)(int,...);
  int argIn;
};

int DigitalRead( int pin );
int AnalogRead( int pin );
//int testFunc(int i);
//int verify_cmd_num( char *cmd, int cmds_rec );
int mSetPinMode( int argc, ... );
int AnalogWrite( int argc, ... );
int DigitalWrite( int argc, ... );
