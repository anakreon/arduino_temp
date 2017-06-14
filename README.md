# arduino_temp

A simple program for Arduino platform.

In the loop:
- Reads raw temperature data from the temperature sensor.
- Prints out the value to the 4 digit, 7 segment display.
- Prints a formatted json value to the serial port.
- Checks for a button press, if pressed, prints as json to the serial port - to enable communication with the host system
