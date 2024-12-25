#include "gps.h"

TinyGPSPlus gps;

HardwareSerial GPSSerial(2);
void gps_setup()
{
    printf("Starting Serial 2 for GPS\n");
    GPSSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
    printf("Serial 2 started at 9600 baud\n");
}


void get_gps_data(void *pvParameters)
{
    gps_vars *vars = (gps_vars *)pvParameters;
    for (;;)
    {
        while (GPSSerial.available() > 0)
        {
            gps.encode(GPSSerial.read());
        }
        if (gps.location.isValid())
        {
            vars->lat = gps.location.lat();
            vars->lon = gps.location.lng();
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
