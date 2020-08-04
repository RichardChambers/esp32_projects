This is shared library source that is used by various projects. These library files provide some piece of functionality that is helpful.

DelayState

The DelayState library provides a way to mimic multi-tasking or multi-threading with the Arduino and ESP32 in the absence of an
actual RTOS. An example of using this library is in the blink_esp32 project which blinks three different LEDs to different rhythms
usng different delays for each LED along with a state machine to know when to set an output pin driving the LED either HIGH or LOW.
