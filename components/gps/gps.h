#ifndef GPS_H
#define GPS_H

#include <TinyGPS++.h>

#define RXD2 16
#define TXD2 17
#define GPS_BAUD 9600

struct gps_vars
{
    float lat;
    float lon;
};

void gps_setup();
void get_gps_data(void *pvParameters);


#endif
