# LTC2945_power_monitor
LTC2945 2.5 to 80V power monitor esp8266
The main purpose of this project is to measure voltage and current from solar power panels and send that data to ThingSpeak.
You can measure up to 80 dc volts with the LTC2945 power monitor.  It uses I2C for communication and is able to use 3.3V for easy use with microcontrollers like the esp8266 used in this project.  This makes it ideal for using with 12 or 24 volt solar panels, etc.
Not only measuring voltage, you could use an appropriate shunt resistor or external shunt to measure DC amps.
You will have to input your shunt resistor ohm value(if measuring amps), network ssid, password and the ThingSpeak ApiKey into the code.


