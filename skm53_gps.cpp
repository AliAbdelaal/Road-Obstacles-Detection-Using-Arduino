#include "Arduino.h"
#include "skm53_gps.h"
#include <SoftwareSerial.h>
#include <string.h>
#include <stdlib.h>
#define TIMEOUT 1000

//a global pointer to the gps serial channel
SoftwareSerial* gps ;

/*
    init the gps by setting the communication channel 
    the buad rate which is 9600 by default in the datasheet
    and send the command to initiate the gps 
*/
void SKM53_GPS_init(int RX, int TX)
{
    gps = new SoftwareSerial(RX, TX);
    gps->begin(9600);
    gps->println("$PSRF103,01,00,01,01*24");
    delay(1000);
}

/*
    this function take two pointers to strings in which it will update
    them to the readed value of long and lat, the default will be 0.00
    if no read is available.
*/
void SKM53_GPS_location(char * longitude, char* latitude)
{
    //variables to be used
    String line ;
    char * token = NULL;
    int flag = 0, index = 0 ;
    bool data = false;

    //loop till time is out which is 1 sec
    for (unsigned long start = millis(); millis() - start < TIMEOUT;)
    {
        //as long as a gps data is available
        while (gps->available())
        {
            // read the data from gps 'the whole line'
            line = gps->readStringUntil('\n');

            //let's set the flag to -1 as a sign of no
            flag = - 1 ;
            index = 0  ;

            //tokenzier the string, splitting it into different words based on the ',' character
            token = strtok(line.c_str(),",");

            //as long as there's token available 
            while(token!=NULL)
            {
                //we are looking for the $GPGGA which will contain the Long and Lat
                if(strcmp(token,"$GPGGA")==0)
                {
                    //set the flat to 1, indicates yes !!
                    flag = 1 ;
                }

                /* 
                    the index number 2 will have the latitude in the line 
                    having GPGGA so the index will only increases if the 
                    flag is set as we will see down below
                */ 
                if(index == 2)
                {
                    memcpy(latitude, token, 9);
                    latitude[9] = '\0';

                }
                /*
                    the same as tha above, but here after reading the
                    long, we don't need to continue 
                */
                if(index == 4)
                {
                    memcpy(longitude, token, 9);
                    longitude[9] = '\0' ;
                    data=true;
                    break ;
                }
                // take the next token
                token = strtok(NULL,",");

                //check if we are in the right row of data, increase the index
                index = flag==1? index+1 : index ;

            }
            // clear the token buffer 
            strtok("","");            
        }
    }

    // if no data was available we set it to zero
    if(data==false)
    {
        longitude = "0.00";
        latitude = "0.00";
    }
        //debugging 
        #ifdef DEBUG 
        Serial.print("***data_from_gps_driver : longitude =  ");
        Serial.println(longitude);
        Serial.print("***data_from_gps_driver : latitude  = ");
        Serial.println(latitude);
        #endif
}

//  get location as a structure have the long and lat as floats
location SKM53_GPS_getLocation()
{
    //make sure the module is running 
    gps->begin(9600);
    gps->println("$PSRF103,01,00,01,01*24");
    delay(1000);

    //read the data as strings first then transfer it to floats
	location myLocation ;
	char log[10], lat[10];
	SKM53_GPS_location(log, lat);
	String log_str(log);
	String lat_str(lat);
	myLocation.longitude = log_str.toFloat();
	myLocation.latitude  = lat_str.toFloat();
	return myLocation ;
}