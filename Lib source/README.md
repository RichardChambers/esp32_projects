This is shared library source that is used by various projects. These library files provide some piece of functionality that is helpful.

### Making a library in Arduino IDE

Here are directions for creating a library that can be imported with the Arduino IDE.  https://www.arduino.cc/en/Hacking/LibraryTutorial

Below is a block quote from the specifics of how to add a library composed of two files, Morse.h and Morse.cpp, to create a library called Morse.

 > And that's all you need (there's some other nice optional stuff, but we'll talk about that later). Let's see how you use the library.

 > First, make a Morse directory inside of the libraries sub-directory of your sketchbook directory. Copy or move the Morse.h and Morse.cpp files into that directory. Now launch the Arduino environment. If you open the *Sketch > Import Library* menu, you should see Morse inside. The library will be compiled with sketches that use it. If the library doesn't seem to build, make sure that the files really end in .cpp and .h (with no extra .pde or .txt extension, for example).

### List of library files and what they do

#### DelayState

The DelayState library provides a way to mimic multi-tasking or multi-threading with the Arduino and ESP32 in the absence of an
actual RTOS. An example of using this library is in the blink_esp32 project which blinks three different LEDs to different rhythms
usng different delays for each LED along with a state machine to know when to set an output pin driving the LED either HIGH or LOW.
