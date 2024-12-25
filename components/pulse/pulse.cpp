#include "pulse.h"

TwoWire I2C_1 = TwoWire(1);  // Create a second I2C bus (I2C bus 1)
MAX30105 particleSensor;
static const char *TAG = "HeartRate";


uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data
                          //
int32_t bufferLength; //data length
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

byte readLED = 26; //Blinks with each data read


int dis_t = 0;

void pulse_setup() {
  ESP_LOGI(TAG, "Initializing pulse sensor...\n");
  I2C_1.begin(18, 19);
  pinMode(readLED, OUTPUT);

  // Initialize sensor
  if (!particleSensor.begin(I2C_1, 100000)) {
    ESP_LOGI(TAG, "MAX30102 was not found. Please check wiring/power. ");
    while (1);
  }
  ESP_LOGI(TAG, "Place your index finger on the sensor with steady pressure.");

  byte ledBrightness = 60;
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411;
  int adcRange = 4096;

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with default settings
}

void get_pulse(void *pvParameters) {
  pulse_vars *vars = (pulse_vars *)pvParameters;
  bufferLength = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps

  //read the first 100 samples, and determine the signal range
  for (byte i = 0 ; i < bufferLength ; i++)
  {
    while (particleSensor.available() == false) //do we have new data?
      particleSensor.check(); //Check the sensor for new data

    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample(); //We're finished with this sample so move to next sample

  }

  //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

  //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
  while (1)
  {
    //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
    for (byte i = 25; i < 100; i++)
    {
      redBuffer[i - 25] = redBuffer[i];
      irBuffer[i - 25] = irBuffer[i];
    }

    //take 25 sets of samples before calculating the heart rate.
    for (byte i = 75; i < 100; i++)
    {
      while (particleSensor.available() == false) //do we have new data?
        particleSensor.check(); //Check the sensor for new data

      digitalWrite(readLED, !digitalRead(readLED)); //Blink onboard LED with every data read

      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample(); //We're finished with this sample so move to next sample

      //send samples and calculation result to terminal program through UART


    }
    printf(" Avg BPM=%d\n", vars->beatAvg);
    printf(" SPO2=%d\n", vars->SPO2);

    //After gathering 25 new samples recalculate HR and SP02
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
    if (heartRate != -999)
      vars->beatAvg = heartRate;
    if (spo2 != -999)
      vars->SPO2 = spo2;
    vTaskDelay(1000 / portTICK_PERIOD_MS); //Take 1000 samples per second
  }
}
