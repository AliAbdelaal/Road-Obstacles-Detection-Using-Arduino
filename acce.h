#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "Arduino.h"


// struct to hold the 3 axis data as float numbers 
typedef struct{
	float x ;
	float y ;
	float z ;
}acce_reads;

// initialization function
void ACCELEROMETER_init();

//get readings from the acc
acce_reads ACCELEROMETER_read();


#endif