#include "acce.h"
#include "Arduino.h"
#include <Wire.h>

//the address and registers from the datasheet
#define LSM303DLHC_ADDRESS (0x32>>1)
#define CTRL_REG1_A 0x20
#define OUT_X_L_A 0x28
#define OUT_X_H_A 0x29
#define OUT_Y_L_A 0x2A
#define OUT_Y_H_A 0x2B
#define OUT_Z_L_A 0x2C
#define OUT_Z_H_A 0x2D
#define SENSORS_GRAVITY_EARTH (9.80665F)              /**< Earth's gravity in m/s^2 */
#define SENSORS_GRAVITY_STANDARD (SENSORS_GRAVITY_EARTH)

static float _lsm303Accel_MG_LSB     = 0.001F;   // 1, 2, 4 or 12 mg per lsb
static void write8(byte reg, byte value);

//initialize the acce itself in the normal mode
void ACCELEROMETER_init()
{
  //initialize the wire (i2c)
	Wire.begin();

  // put 1001 0111 in the ctrl_reg1_a
  //normal mode 
  //enable x,y,z for reading
	write8(CTRL_REG1_A, 0x97);	
}

//read access from acce
acce_reads ACCELEROMETER_read()
{
  //variable to hold the values readed
	acce_reads myReads ;
  //initiate a call to the acc
	Wire.beginTransmission((byte)LSM303DLHC_ADDRESS);
  //ask for the three sensors readings
  //note the address called 0x80 binary(1000 0000) OR with the x_low address
  //because we need to read more than one byte  
	Wire.write(OUT_X_L_A|0x80);
  //end the transmission
	Wire.endTransmission();
  //request the 6 bytes from the acce, which are the x(low/high), y(low/high), z(low/high)
	Wire.requestFrom((byte)LSM303DLHC_ADDRESS,(byte)6);
  //wait for the data to be available 
	while(!Wire.available());

	//read the 6 bytes from the sensor 
	uint8_t xlo = Wire.read();
	uint8_t xhi = Wire.read();
	uint8_t ylo = Wire.read();
	uint8_t yhi = Wire.read();
	uint8_t zlo = Wire.read();
	uint8_t zhi = Wire.read();
	// concate the low and high bytes together in one variable 
	myReads.x = (int16_t)(xlo | (xhi << 8)) ;
	myReads.y = (int16_t)(ylo | (yhi << 8)) ;
	myReads.z = (int16_t)(zlo | (zhi << 8)) ;
  // unit transformation (for the sake of units)
	myReads.x *= _lsm303Accel_MG_LSB * SENSORS_GRAVITY_STANDARD ;
	myReads.y *= _lsm303Accel_MG_LSB * SENSORS_GRAVITY_STANDARD ;
	myReads.z *= _lsm303Accel_MG_LSB * SENSORS_GRAVITY_STANDARD ;
	return myReads;
}

// static AKA private function to write data to the register 
static void write8(byte reg, byte value)
{
  /*
    begins the transmission, send the address and the data
  */
  Wire.beginTransmission(LSM303DLHC_ADDRESS);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)value);
  Wire.endTransmission();
}
