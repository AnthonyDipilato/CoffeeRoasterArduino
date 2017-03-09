Coffee Roaster
Written By Anthony DiPilato
 
This project is the arduino code for the coffee roaster that I built.
You can see the build log and details for the project at [url]

The arduino acts as slave to the raspberry pi.
Inputs:
- RaspberyPi commands via usb 
- 3 thermocouples (MAX31855)
- 2 infrared flame sensors

Outputs: 
- 5 relays
- PWM pin for proportial gas valve.

Relays:
- Normally closed solenoid valve for gas line
- Ignitor (relay to 110V SSR)
- Cooling fan (relay to 110V SSR)
- Exhaust Fan (relay to 110V SSR)
- Drum Motor (relay to 110V SSR)


USE AT YOUR ON RISK.
I am posting this project for EDUCATIONAL USE ONLY.
This project involves electricity, propane, and fire.
I will not be held liable damages and/or injuries resulting from the use of this code or from copying this project.

Required libraries
Adafruit Max31855 library