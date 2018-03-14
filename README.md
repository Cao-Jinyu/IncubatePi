![alt text](https://raw.githubusercontent.com/croaljack0/IncubatePi/master/Media/incupi_logo_2_lowres.png)

### Neonate (Newborn Child) Real Time Temperature Control Incubator
*Features a Rasberry Pi, Temperature Sensors, a Heater and a Fan.*
*Low budget alternative to conventional incubators suitable for the developing world.*

[facebook_icon]: https://raw.githubusercontent.com/croaljack0/IncubatePi/master/Media/facebook_icon.png
[facebook_url]: https://www.facebook.com/IncuPi/

[instagram_icon]: https://raw.githubusercontent.com/croaljack0/IncubatePi/master/Media/instagram_icon.png
[instagram_url]: https://www.instagram.com/incupi_project

Click these icons to follow us on  [![alt text][facebook_icon]][facebook_url] [![alt text][instagram_icon]][instagram_url]

## Problem

Premature or ill neonates often require automated temperature monitoring and control because they are unable to properly regulate their own body temperature. This is conventionally provided using expensive incubator systems. In the developing world, where standards of health care are poor and funding is very limited, an incubator solution using inexpensive and widely accessible technology is required.

## Solution

IncuPi provides a neonate temperature monitoring solution using a Raspberry Pi as the central control unit. Multiple temperature sensors provide information about the temperature of the neonate and the incubator ambient temperature. Embedded PID control software on the Raspberry Pi is then used to control a heater and fan system. This provides the appropriate heat control to ensure a neonate body temperature of 37 degrees Celsius.

![alt text](https://raw.githubusercontent.com/croaljack0/IncubatePi/master/Media/incupi_block_diagram.png)

## Wiki 

### Software
All required software is provided within the folder [Software](./Software). This can be compiled and executed on the RPI using the following command:
`cmake . && make && sudo ./TempControl`
The software features a main class and 4 other classes as follows (full documentation is avaliable in each of the respective header files):
* **GPIOWriter:** Configures an RPI GPIO pin so that it can be set high and low as required.
* **PWMChip:** Configures a PWM chip on a RPI so that its period and duty cycle can be specified and it can be enabled and disabled as required.
* **ReadTemp:** Reads the temperature from a DS18B20 using the RPI's one wire interface.
* **PID:** Encapsulates a general PID controller.

### Printed Circuit Boards
The device features three circuit boards. Two Temperature PCBs and a Power PCB as described below:
* **Temperature PCB:** This PCB contains only a temperature sensor. One of these is wrapped in elastic and attached to the neonates arm to measure its temperature. The other is placed in the incubator box to measure the ambient temperature. 
* **Power PCB:** This PCB connects the power supplies and RPI GPIO pins to the relevant devices (fan, heater and temp. sensors).

### Power 
Power is provided to the device using an old 200 W desktop computer power supply. The required voltages are 12 V and 5 V. These supplies are connected to the power PCB using Molex Mini-Fit Jr. connectors.

### Bill of Materials
All the neccessary components for this project are listed within the file [BOM.xlsx](./Circuit Design/BOM.xlsx). Links to purchase the required items are also provided. 

### Mechanical Assembly 
The fan and heater box are attached to the side of the main incubator box using standard screws. Two files are provided within the [Mechanical Design](./Mechanical Design) directory for use as masks for drilling both the heater box and the main incubator box.

## University of Glasgow

Developed as part of Real Time Embedded Programming Project at the University of Glasgow.

Course convener:    Dr Bernd Porr

## Team Members

**Jack Croal**
**Cameron Houston**
### Chris Brown
