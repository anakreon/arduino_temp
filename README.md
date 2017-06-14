# arduino_temp

A simple program for Arduino platform.

Arduino code - in the loop:
- Reads raw temperature data from the temperature sensor.
- Prints out the value to the 4 digit, 7 segment display.
- Prints a formatted json value to the serial port.
- Checks for a button press, if pressed, prints as json to the serial port - to enable communication with the host system


Backend node.js:
- connects to a mysql server
- reads data from the serial port and:
  - records the measured temperature to mysql
  - triggers a predefined action (if the button was pressed)
