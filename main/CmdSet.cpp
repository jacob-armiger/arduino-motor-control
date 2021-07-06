#include "CmdSet.hpp"
#include <Regexp.h> // Regular expressions

#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>
#include <stdarg.h>
#define VERBOSE 1


int mprintf( const char *format, ... )
{
  char buf[ 256 ];
  va_list args;
  va_start( args, format );
  vsprintf( buf, format, args );
  Serial.print( buf );
  va_end( args );
}

int DigitalRead( int pin, ... ) {
  #if VERBOSE
    mprintf("DigitalRead: %d:%d\n", pin, digitalRead( pin ) );
  #endif
  return digitalRead( pin );
}

int AnalogRead( int pin, ... ) {
  #if VERBOSE
    mprintf("AnalogRead: %d:%d\n", pin, analogRead( pin ) );
  #endif
  return analogRead( pin );
}

int mSetPinMode( int argc, ... ) {
  if( argc != 2 ) return -1;

  va_list valist;

  va_start( valist, argc );

  int holder[2];
  for( int i = 0; i < argc; i++ ) {
    holder[i] = va_arg( valist, int );
  }

  if( holder[ 1 ] == 1 ) {
    #if VERBOSE
      mprintf("mSetPinMode: pin %d to INPUT\n", holder[ 0 ] );
    #endif
    pinMode( holder[ 0 ], INPUT );
  } else if( holder[ 1 ] == 0 ) {
    #if VERBOSE
      mprintf("mSetPinMode: pin %d to OUTPUT\n", holder[ 0 ] );
    #endif
    pinMode( holder[ 0 ], OUTPUT );
  }

  return 1;
}

int AnalogWrite( int argc, ... ) {
  if( argc != 2 ) return -1;

  va_list valist;

  va_start( valist, argc );

  int holder[2];
  for( int i = 0; i < argc; i++ ) {
    holder[i] = va_arg( valist, int );
  }

  if( holder[ 1 ] == 1 ) {
    #if VERBOSE
      mprintf("AnalogWrite: pin %d to %d\n", holder[ 0 ], holder[ 1 ] );
    #endif
    analogWrite( holder[ 0 ], holder[ 1 ] );
  } else if( holder[ 1 ] == 0 ) {
    #if VERBOSE
      mprintf("AnalogWrite: pin %d to %d\n", holder[ 0 ], holder[ 1 ] );
    #endif
    analogWrite( holder[ 0 ], holder[ 1 ] );
  }

  return 1;
}

int DigitalWrite( int argc, ... ) {
  if( argc != 2 ) return -1;

  va_list valist;

  va_start( valist, argc );

  int holder[2];
  for( int i = 0; i < argc; i++ ) {
    holder[i] = va_arg( valist, int );
  }

  if( holder[ 1 ] == 1 ) {
    #if VERBOSE
      mprintf("DigitalWrite: pin %d to %d\n", holder[ 0 ], holder[ 1 ] );
    #endif
    digitalWrite( holder[ 0 ], holder[ 1 ] );
  } else if( holder[ 1 ] == 0 ) {
    #if VERBOSE
      mprintf("DigitalWrite: pin %d to %d\n", holder[ 0 ], holder[ 1 ] );
    #endif
    digitalWrite( holder[ 0 ], holder[ 1 ] );
  }

  return 1;
}
