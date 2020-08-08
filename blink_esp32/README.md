Blink LEDs in different rhythms using the DelayState library to specify delays for pin HIGH, to turn on the LED, and LOW, to turn off the LED, along with a state
machine to know how to transition to a new state when the delay for the current state expires.

The source code expects three LEDs for blinking: two LEDs on the breadboard connected to pins on the ESP32 and the third
LED is the builtin LED on pin 2.

In addition, the source code expects a momentary contact button which is used as a user interface.
