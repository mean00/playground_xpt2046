/***************************************************
 Battery Tester 2
 Base ino, just a rempoline to real code
 ***************************************************/

// includes all we need here, so that the cmake magic will find and link them

#include <Wire.h>
#include "SPI.h"

extern void mySetup();
extern void myLoop();

void setup() 
{
    mySetup();
}


void loop(void) 
{
    myLoop();
}

// EOF
