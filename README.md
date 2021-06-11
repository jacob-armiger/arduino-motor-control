# arduino-motor-control
An Arduino script that logs controller transmissions

### Hardware
* Spektrum DX8 transciever
* Spektrum AR8010T
* Arduino Uno

### Use
The transciever controls an RC style Autonomous Service Vessel which has two small propeller motors.  
Each motor is connected to an interrupt pin, and then the interrupts are used to measure the PWM's from the reciever.

