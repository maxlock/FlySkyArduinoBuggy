# FlySkyArduinoBuggy
Use a FlySky or Turnigy receiver connected to an rduino and motor shield to drive a two wheeled buggy.

This code builds uses the excellent [FlySky i-bus library](https://gitlab.com/timwilkinson/FlySkyIBus) by Tim Wilkinson.
Additionally I've used an [Adafruit V1 motorshield](https://learn.adafruit.com/adafruit-motor-shield/using-dc-motors) and 
a differential steering algorithm from [Stackexchange](https://electronics.stackexchange.com/revisions/293108/3)

Connect the motor sheild you your arduino, and motors to whichever channels you want. 1 and 2 give you more options to set frequency.
You'll need to connect the receiver to Gnd and 5v, as well as RX (pin 0) but as it's not brought out to a header you may have to be creative.

An outstanding issue is that once powered on, one of the motors will rotate until the TX and RX are communicating. Additionally there
is no failsafe so if the TX is powered off or goes out of range the buggy will continue at it's current rate.
