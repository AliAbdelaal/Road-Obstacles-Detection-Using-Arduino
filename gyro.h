#ifndef GYRO_H_
#define GYRO_H_

//struct to hold the data
typedef struct{
	int x ;
	int y ;
	int z ;

}gyro_reads;

// initialize the gyroscope sensor 
void GYRO_init();

//get the readings from the gyro
gyro_reads GYRO_read();


#endif