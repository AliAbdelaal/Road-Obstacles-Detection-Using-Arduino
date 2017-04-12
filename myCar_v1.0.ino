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
	if(info.but_or_man == MANHOLE)
		Serial.println("MANHOLE");
	else if(info.but_or_man == BUTHOLE)
		Serial.println("BUTHOLE");
	else if(info.but_or_man == IDLE)
		Serial.println("IDLE");
	Serial.println("\n\n*****************************\n\n");
	Serial.print("longitude = ");
	Serial.println(info.longitude);
	Serial.print("latitude = ");
	Serial.println(info.latitude);
	delay(100);
	
}
