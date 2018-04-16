### Neonate (Newborn Child) Real Time Temperature Control Incubator
*Features a Raspberry Pi, Temperature Sensors, a Heater and a Fan.*  
*Low budget alternative to conventional incubators suitable for the developing world.*

**Find our promotional video on YouTube:** http://www.youtube.com/watch?v=8vy_HCOcaGk

**You can also find a demo of the device here:** 

[facebook_icon]: https://raw.githubusercontent.com/croaljack0/IncubatePi/master/Media/Marketing%20Images/facebook_icon.png
[facebook_url]: https://www.facebook.com/IncuPi/
[instagram_icon]: https://raw.githubusercontent.com/croaljack0/IncubatePi/master/Media/Marketing%20Images/instagram_icon.png
[instagram_url]: https://www.instagram.com/incupi_project
[youtube_icon]: https://raw.githubusercontent.com/croaljack0/IncubatePi/master/Media/Marketing%20Images/youtube_icon.png
[youtube_url]: https://www.youtube.com/channel/UCyrm2wJSCseUDFAKD9X7mjQ
Click these icons to follow us on  [![alt text][facebook_icon]][facebook_url] &nbsp; [![alt text][instagram_icon]][instagram_url] &nbsp; [![alt text][youtube_icon]][youtube_url]

View our project on Hackaday: https://hackaday.io/project/125376-incupi

## Problem

Premature or ill neonates often require automated temperature monitoring and control because they are unable to properly regulate their own body temperature. This is conventionally provided using expensive incubator systems. In the developing world, where standards of health care are poor and funding is very limited, an incubator solution using inexpensive and widely accessible technology is required.

## Solution

IncuPi provides a neonate temperature monitoring solution using a Raspberry Pi as the central control unit. Multiple temperature sensors provide information about the temperature of the neonate and the incubator ambient temperature. Embedded PID control software on the Raspberry Pi is then used to control a heater and fan system. This provides the appropriate heat control to ensure a neonate body temperature of 37 degrees Celsius.

**SEE THE WIKI DOCS [HERE](https://github.com/croaljack0/IncubatePi/wiki) FOR DETAILED HARDWARE AND SOFTWARE IMPLEMENTATION**

![alt text](https://raw.githubusercontent.com/croaljack0/IncubatePi/master/Media/Marketing%20Images/incupi_block_diagram.png)

## Real-time evaluation

#### 1. Responsiveness of the application
A fast temperature response was required to ensure fast-heating of the incubator in emergency situations. The final device was successful in achieving this. The PID temperature response times are shown below:

![alt text](https://raw.githubusercontent.com/croaljack0/IncubatePi/master/Media/Marketing%20Images/temperature_response.jpg)

The full data for these results can be found [here](./Experimental%20Data).

#### 2. Permitted latencies
The latency between the temperature reading and the incubator temperature change has been tested to be no longer than one minute for temperature changes of one degree Celsius.

#### 3. Sampling rate
The ambient temperature of the incubator is sampled and processed every 10 seconds by the PID control software. The neonate temperature is sampled every minute because this temperature does not vary significantly over short periods of time. This ensures sufficient data for the temperature control, modifying the heater/fan power every 5 seconds.

#### 4. Bus protocol
The Raspberry Pi communicates with the DS18B20 temperature sensors using the RPI's 1-wire interface. This uses a single wire to provide data, and power. This reduces wiring complexity and allows for multiple temperature devices connected in parallel without a chip select line for each, thus improving the flexibility and scalability.

A standard PWM signal is used to control the fan.

#### 5. How many channels
Multiple temperature sensor channels can be connected to the Raspberry Pi using the 1-wire interface. The default for this is 2: one for neonate temperature and one for the incubator ambient temperature. This is passed to the GPIO and PWM outputs to control the heating element's MOSFET and the fan respectively.

#### 6. Low level implementation: kernel or user space?
The main application runs in user space because it provides security and portability. The timing requirements of the system were not severe enough to warrant custom kernel routines. Some of the external interfaces (eg. 1-wire) are implemented using loadable kernel modules.

#### 7. Data flow from hardware to GUI to output
The Raspberry Pi sends data to a connected monitor using 

#### 8. Buffering of data
The temperature data is buffered over a maximum of 5 minutes to aid in the accurate PID control of the slow-changing temperature values.

#### 9. Buffering impacting on the real-time performance
The buffering of the temperature values is a low performance activity, and introduces very little latency within the application.

#### 10. Post-processing of data, latencies and processor load
The data is processed real-time as the temperature information is detected. This is post-processed in by the custom PID controller to provide the values for the heater and fan elements. This introduces a maximum latency of 5 ms, allowing for low-latency temperature changes.

#### 11. Number of threads and load be distribution
The main software is executed in one thread, and child threads are created to handle the temperature measurements and other external interrupts. The number of child threads created can be varied depending on the number of temperature sensors connected, to aid in the scalability of the application.

#### 12. GUI implementation
The GUI is implemented using QT, and is displayed on a connected monitor. The refresh rate is equal to the temperature sensor sample rate: refresh every second. The allowed latency is 0.5 seconds. The temperature information is displayed real-time, along with the PID temperature controller data.

#### 13. Class structure and unit tests
The software features a main class and 4 other classes as follows:

![UML diagram](https://raw.githubusercontent.com/croaljack0/IncubatePi/master/Media/Device%20Images/UML_diagram.jpg)

Unit tests are included for each class, and can be executed using the command `make test` within the software directory.

**SEE THE WIKI DOCS FOR DETAILS ON SOFTWARE [HERE](https://github.com/croaljack0/IncubatePi/wiki/Software)**

#### 14. Team structure
The team members and roles are as follows:

* **Jack Croal** - Hardware Developer
* **Chris Brown** - Software Developer
* **Cameron Houston** - GUI Implementation

These roles are flexible, with every team member cooperating on each section of the project to improve the common understanding within the team. Three times weekly scrum meetings are carried out to establish roles and tasks.

#### 15. Project time allocation
This project was completed over 4 months.

* **January**: Research and initial experimental development
* **February**: Hardware manufacturing and software implementation
* **March**: Finalising of software and incubator experimental tests
* **April**: Completion of project with release and media engagement

#### 16. Version control
The git version control software was chosen due to its wide-scale use and support, and its ability to merge between branches and create releases. The software releases can be found in the [releases section](https://github.com/croaljack0/IncubatePi/releases) of this GitHub, labeled A.B where A is the major release number and B is the minor release number.

#### 17. Release strategy and publication
[facebook_icon]: https://raw.githubusercontent.com/croaljack0/IncubatePi/master/Media/Marketing%20Images/facebook_icon.png
[facebook_url]: https://www.facebook.com/IncuPi/
[instagram_icon]: https://raw.githubusercontent.com/croaljack0/IncubatePi/master/Media/Marketing%20Images/instagram_icon.png
[instagram_url]: https://www.instagram.com/incupi_project
[youtube_icon]: https://raw.githubusercontent.com/croaljack0/IncubatePi/master/Media/Marketing%20Images/youtube_icon.png
[youtube_url]: https://www.youtube.com/channel/UCyrm2wJSCseUDFAKD9X7mjQ
Click these icons to follow us on  [![alt text][facebook_icon]][facebook_url] &nbsp; [![alt text][instagram_icon]][instagram_url] &nbsp; [![alt text][youtube_icon]][youtube_url]

The project can also be found on Hackaday here: https://hackaday.io/project/125376-incupi

The software and hardware schematics are released here on GitHub. Please feel free to download, under the MIT license included.

#### 18. Success of the application
The application is evaluated on its temperature response time based on the neonate's temperature changes. The experimental results are included [here](./Experimental%20Data). The application is successful in holding a steady temperature within the incubator and changes the temperature accordingly with low latency and useful graphical output.

## University of Glasgow
Developed as part of Real Time Embedded Programming Project at the University of Glasgow.

Course convener:    Dr Bernd Porr
