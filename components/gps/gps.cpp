#include "gps.h"

TinyGPSPlus gps;

HardwareSerial GPSSerial(2);
void gps_setup()
{
    printf("Starting Serial 2 for GPS\n");
    GPSSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
    printf("Serial 2 started at 9600 baud\n");
}

// Get GPS data function
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
            printf("Latitude: %f, Longitude: %f\n", vars->lat, vars->lon);
        }
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}
