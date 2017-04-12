#ifndef SKM53_GPS_H_
#define SKM53_GPS_H_

#include "Arduino.h"

// a structure to hold the long and lat read as float 
// so that we can access them in one time
typedef struct{
	float longitude;
	float latitude;
}location ;

//initialize the gps module to pins rx and tx 
void SKM53_GPS_init(int RX, int TX);

//update the location strings long and lat
void SKM53_GPS_location(char * longitude, char* latitude);

//get the location struct that contains float long and lat
location SKM53_GPS_getLocation();


#endif