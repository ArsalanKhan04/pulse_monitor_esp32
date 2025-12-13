pulse component

Purpose

Handles MAX30105 initialization, continuous FIFO sampling and computation of heart rate (BPM) and SpO2. Provides a FreeRTOS task get_pulse that updates a shared pulse_vars struct.

Key files

- pulse.h — defines pulse_vars and declares pulse_setup() and get_pulse().
- pulse.cpp — configures I2C on TwoWire(1) (pins 18=SDA, 19=SCL), initializes the sensor and continuously reads data into ring buffers.
- Uses maxim_heart_rate_and_oxygen_saturation(...) (spo2_algorithm) to compute BPM and SpO2 from buffers.

Runtime behavior

- pulse_setup() initializes I2C and configures MAX30105 (led brightness, sample average, mode, sample rate, pulse width and ADC range).
- get_pulse() fills 100-sample buffers, computes initial HR/SpO2, then in a loop shifts the buffer and collects 25 new samples, recalculating HR/SpO2 every second.
- Results are written into the provided pulse_vars struct (beatAvg, SPO2, irValue).

Hardware/pin notes

- I2C (TwoWire(1)) uses SDA=18 and SCL=19 in this project.
- readLED pin 26 blinks on each sample read for visual feedback.

Integration

Create the get_pulse task with a pointer to a pulse_vars instance (main.cpp). Other tasks read the pulse_vars values to display or send over network.