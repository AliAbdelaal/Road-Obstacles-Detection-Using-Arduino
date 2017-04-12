#include "ultrasonic.h"
#include "Arduino.h"


//static variable to save the trig and echo pins through the life of this cpp file
static unsigned char echo, trig;


/*
	init the pins of trig and echo and set the global variables 
	so that we can reference them in the read function
*/
void ULTRASONIC_init(unsigned char local_echo, unsigned char local_trig)
{
	pinMode(local_echo, INPUT);
	pinMode(local_trig, OUTPUT);
	echo = local_echo;
	trig = local_trig;
}

/*
	read the distance from the ultrasonic
	by sending a 10us pulse then read the width of the returned pulse
	divide it by 58 (from the datasheet of the ultrasonic) will give
	the distance
*/
float ULTRASONIC_read()
{
	float distance = -1;
	int dur  ;
  	digitalWrite(trig,HIGH);
  	delayMicroseconds(10);
  	digitalWrite(trig,LOW);
  	dur=pulseIn(echo,HIGH);
  	distance= dur / 58.0 ;
 	if (distance>0 && distance<200)
 	{
 		return distance ;
 	}
 	else 
 		return -1.0 ;

}