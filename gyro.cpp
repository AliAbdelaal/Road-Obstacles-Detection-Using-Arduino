#include "gyro.h"
#include "Arduino.h"
#include <Wire.h>

//registers from the datasheet
#define L3GD20_Address 0x6b //the device address includes the mode (read)
#define CTRL_REG1 0x20        //the control register to set the power mode 
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_z_H 0x2D
// unit constants 
#define GYRO_SENSITIVITY_2000DPS (0.070F)
#define SENSORS_DPS_TO_RADS      (0.017453293F)          /**< Degrees/s to rad/s multiplier */

static void write8(byte reg, byte value);

//initialize the sensor and set the mode of the gyro
void GYRO_init()
{
  //push range to 2000dps
  Wire.begin();
  //clear the reg 
  write8(CTRL_REG1, 0x00);
  //normal mode and enable the axis 
  write8(CTRL_REG1, 0x0F);
  //clear the reg
  write8(CTRL_REG4, 0x00);
  //define the senstivity to 2000DPS
  write8(CTRL_REG4, 0x20);
}

//get the readings of the sensor 
gyro_reads GYRO_read()
{
	gyro_reads myReads ;
  //start the transmission, by calling the address of the gyroscope 
	Wire.beginTransmission((byte)L3GD20_Address);
	//ask the gyro for the address of x_low reg and telling it we will be asking for 
  //more than one byte 
  Wire.write(OUT_X_L|0x80);
	Wire.endTransmission();
  //ask the gyro for 6 bytes which are the x(low/high), y(low/high), z(low/high)
	Wire.requestFrom((byte)L3GD20_Address, (byte)6);
	//read the values 
	uint8_t xlo = Wire.read();
	uint8_t xhi = Wire.read();
	uint8_t ylo = Wire.read();
	uint8_t yhi = Wire.read();
	uint8_t zlo = Wire.read();
	uint8_t zhi = Wire.read();
	//concat the values from two bytes into on 16bit var
	myReads.x = (int16_t)(xlo | (xhi << 8));
	myReads.y = (int16_t)(ylo | (yhi << 8));
	myReads.z = (int16_t)(zlo | (zhi << 8));
	//compensate vaues, for the sake of units
	myReads.x *= GYRO_SENSITIVITY_2000DPS ;
	myReads.y *= GYRO_SENSITIVITY_2000DPS ;
	myReads.z *= GYRO_SENSITIVITY_2000DPS ;
	return myReads;
}

// static AKA private function to write data to the register 
static void write8(byte reg, byte value)
{
  /*
    begins the transmission, send the address and the data
  */
  Wire.beginTransmission(L3GD20_Address);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)value);
  Wire.endTransmission();
}