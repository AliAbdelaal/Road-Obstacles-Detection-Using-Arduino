#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "Arduino.h"


//initialize the ultrasonic pins [echo and trig]
void ULTRASONIC_init(unsigned char echo, unsigned char trig);

//return the reading from the ultrasonic or -1 if error occured
float ULTRASONIC_read();



#endif