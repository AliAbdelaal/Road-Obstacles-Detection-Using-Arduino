#ifndef READER_H_
#define READER_H_
#define DEBUG
#include "Arduino.h"
#include "acce.h"
#include "gyro.h"
#include "ultrasonic.h"
#include "skm53_gps.h"

//change these to change the sensitivty
//note that heigher number mean low senstivity 
//this represent the angular change
#define GYRO_SENS 50
//maximume change allowed in acce
//mainly because of unstable cars
#define ACCE_TOLERANCE 45
//in cm
#define ULTASONIC_MIN_DIST 10
#define ULTASONIC_MAX_DIST 100
//time to wait for the car to pass the event
// in seconds
#define MAX_WAIT_TIME 3

//important states
#define MANHOLE 1
#define BUTHOLE 2
#define IDLE 0

typedef struct{

	int but_or_man ;
	float longitude ;
	float latitude  ;

}event_info;

//initialize the sensors and calibrate the sensors
void init_and_calibrate(uint8_t ultrasonic_echo, uint8_t ultrasonic_trig, uint8_t gps_rx, uint8_t gps_tx);

//return one of the ultrasonic states
int ultrasonic_check();

//return the struct, check but_or_man for the event type
event_info event_check();


//initialize the lcd module 
void LCD_init(uint8_t RS, uint8_t EN, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7);



#endif