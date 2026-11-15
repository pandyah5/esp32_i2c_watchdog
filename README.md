# Summary

A very short demo code where one OLED display monitors the i2c health of the other OLED display. Since ESP32 has two i2c controller both OLED displays run on different i2c controllers.

This is required because of two reasons:

- They both share the same address (0x3C) and hence cannot share the same bus
- When one bus goes down the other display needs to be up in order to report the error that is detected.

The ESP32 runs the logic to check for the health over both i2c controllers, the OLEDs are acting as a slave/peripheral device.

To experiment with it, you could ground the SCL/SDA of one OLED display and notice how the other OLED shows the corresponding error code (err 5)

# About the files
> esp32_i2c_monitor.ino

This is just a normal i2c monitor, it cannot re-init the device if it goes down. For example, when you disconnect an OLED's VCC and reconnect it, you will need to press EN or re-flash the device because it won't come up even after re-plugging the VCC. That is why this file is called monitor. It does not automatically recover the device.

> esp32_i2c_watchdog_recover.ino

In this file if you disconnect and reconnect the VCC of a OLED, the microprocessor will re-init the slave/peripheral device once the bus is up and you don't have to manually reset the ESP32.
This is more aligned to what a watchdog should do.

You could also attach an oscilloscope to monitor the clock signal (SCL).

# Components

- ESP32
- 2x OLED modules (over i2c)
