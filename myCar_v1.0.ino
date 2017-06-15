//the libs 
#define DEBUG
#include "reader.h"
#include "skm53_gps.h"


//initializations
void setup()
{
	//initialize the acce
	LCD_init(2,3,4,5,6,7);

	//initialize the serial for debugging
	Serial.begin(9600);
	//initialize the GPS and the ultrasonic 
	init_and_calibrate(8,9,10,11);
	
}


//the loop
void loop()
{
	//update the info datastructure with new values
	event_info info = event_check();

	//get the ultrasonic read
	int ultrasonic = ultrasonic_check();
	
	// debugging the values 
	Serial.print("ultrasonic = ");
	Serial.println(ultrasonic);
	Serial.print("but_or_man = ");
	if(info.but_or_man == MANHOLE || info.but_or_man == BUTHOLE)
	{
		char str_log[10],str_lat[10] ;
		SKM53_GPS_location(char * str_log, char* str_lat);
		////str log and str lat are now available
		//GSM code goes here ..
		//http_send(str_log, str_lat, 
	}
	else if(info.but_or_man == IDLE)
		Serial.println("IDLE");
	Serial.println("\n\n*****************************\n\n");
	Serial.print("longitude = ");
	Serial.println(info.longitude);
	Serial.print("latitude = ");
	Serial.println(info.latitude);
	delay(100);
	
}
