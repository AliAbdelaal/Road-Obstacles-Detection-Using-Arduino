#include "reader.h"
#include <LiquidCrystal.h>

//the calebrated value (default) for acce_x_axis
static float acce_x_calibrated = 0 ;

//a reference to the lcd object 
LiquidCrystal *lcd ;

//initialize the sensors and calibrate the sensors
void init_and_calibrate(uint8_t ultrasonic_echo, uint8_t ultrasonic_trig, uint8_t gps_rx, uint8_t gps_tx)
{
	//init the ultrasonic for the specified pins
	ULTRASONIC_init(ultrasonic_echo, ultrasonic_trig);
	//init the gps 
	SKM53_GPS_init(gps_rx, gps_tx);
	//init the gyro
	GYRO_init();
	//init the acc
	ACCELEROMETER_init();
	//calibrating the sensor and get the average readings for acc
	float acce_av = 0 ;
	for(uint8_t i=0;i<3;i++)
		acce_av += ACCELEROMETER_read().x ;
	acce_av /= 3 ;
	//set this av as the calibrated value 
	acce_x_calibrated = acce_av ;
	#ifdef DEBUG
		Serial.print("acce calibrated at : ");
		Serial.println(acce_x_calibrated);
	#endif

}

//initialize the lcd module 
void LCD_init(uint8_t RS, uint8_t EN, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7)
{
	lcd = new LiquidCrystal(RS, EN, D4, D5, D6, D7);
	lcd->begin(16,2);
	lcd->print("Welcome ... !");
}

//a function to display a msg on the lcd 
void LCD_display(String msg)
{
	lcd->clear();
	lcd->print(msg);
}


//return one of the ultrasonic states
int ultrasonic_check()
{
	float dist = ULTRASONIC_read();
	#ifdef DEBUG
		Serial.print("distance = ");
		Serial.println(dist);
	#endif
	if(dist == -1 )
		{
			LCD_display("Clear!");
			return IDLE ;
		}
	else if(dist > ULTASONIC_MAX_DIST )
		{
			LCD_display("BUTHOLE !!");
			return BUTHOLE ;
		}
	else if(dist < ULTASONIC_MIN_DIST)
		{
			LCD_display("MANHOLE !!");
			return MANHOLE ;
		} 
	else{
			LCD_display("Clear...");
			return IDLE ;

		}
}

//return the struct containing the current state either IDLE or manhole or buthole 
event_info event_check()
{
	//local variables to be used 
	event_info info ;
	float acce_x_before, acce_x_after, acce_diff;
	int gyro_y ;
	//get the current gyro Y_axis read
	gyro_y = GYRO_read().y;
	//if the read is larger than the senstivity set by the user either +ve or -ve
	if(gyro_y> GYRO_SENS || gyro_y<(GYRO_SENS*-1))
	{
		#ifdef DEBUG
			Serial.println("change captured !");
		#endif
		//save the acce value
		//give sometime for the acce to read the new value 
		delay(500);
		acce_x_before = ACCELEROMETER_read().x ;
		//wait  for the car to settle down
		delay(MAX_WAIT_TIME*1000);
		//now check acce again
		acce_x_after = ACCELEROMETER_read().x ;
		//calculate the diff
		acce_diff = abs(acce_x_after - acce_x_before);
		#ifdef DEBUG
			Serial.print("acce before : ");
			Serial.println(acce_x_before);
			Serial.print("acce after : ");
			Serial.println(acce_x_after);
			Serial.print("acce diff : ");
			Serial.println(acce_diff);
		#endif
		//if the diff is larger than the tolerance 
		if(acce_diff > ACCE_TOLERANCE)
		{
			//the car flipped (manhole or buthole)
			//manhole 
			if(acce_x_after > acce_x_before)
				info.but_or_man = BUTHOLE ;
			//buthole 
			else
				info.but_or_man = MANHOLE ;
			//update GPS 
			location loc = SKM53_GPS_getLocation();
			info.longitude = loc.longitude;
			info.latitude  = loc.latitude ;
		}		
		else
		{
			//the car is on a bridge or in a tunnel
			//update the calibrated value
			#ifdef DEBUG
			Serial.println("car stabled !");
			if(acce_x_after<acce_x_before)
				Serial.println("bridge is suspected!");
			if(acce_x_after>acce_x_before)
				Serial.println("tunnel is suspected!");
			Serial.print("acce recalibrated to : ");
			Serial.println(acce_x_calibrated);
			#endif

			acce_x_calibrated = acce_x_after ;
			info.but_or_man = IDLE ;
		}

	}
	else
		info.but_or_man = IDLE ; 
	return info ;
}


