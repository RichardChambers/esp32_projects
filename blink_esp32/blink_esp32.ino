
// ESP32 sketch. Must set Tools  -> Board to NodeMCU-32S

// Project to test multi-threading using pseudo-threads.
//
// Blink multiple LEDs at different rates and/or output to Serial Monitor
// printed lines at different rates. Also demonstrate handling button presses
// from a momentary contact switch and debouncing such a switch to eliminate
// spurious signals from mechanical parts making and breaking electrical contact
// as they move in response to the button press.

// This sketch requires the DelayClass and the StateClass functionality
// in order to interleave the pin states and state delays for multiple pins
// in order to do the blinking of multiple LEDs at different rates.
// These classes are also used in the button press detection logic in order
// to count number of presses in a time duration as well as to specify the
// amount of time used for debouncing by ignoring electrical state changes in
// the pin to which the momentary contact switch is wired.
//
// The actual pin assignments are below. If you are using different pins
// on the ESP32 then you will need to change the pin numbers assigned to the
// variables for the pins.

// This sketch allows for two different demonstrations of using DelayClass
// and StateClass objects to interleave multiple pseudo-threads within the
// execution of a single real thread.
//
// One demonstration is with multiple LEDs and the second is multiple print
// threads to Serial and the Serial Monitor display.

#define MULTIPLE_LEDS
//#define MULTIPLE_SERIAL

#include "DelayState.h"

static unsigned long ulLoopCount = 0;

#if defined(MULTIPLE_LEDS)

// The function TaskDelayPin() is the finite state machine for blinking
// an LED. Each LED has its own DelayClass, its own list of pin state
// timings (the length of time a pin stays at LOW or HIGH), and the
// ESP32 pin number to be used with the function digitalWrite().
int TaskDelayPin (DelayClass &delayPin, int delayPinTiming[], int xPin)
{
  // The following state machine flashes the designated LED
  // by turning it on for some duration then turning it off
  // for a second duration.
  switch (delayPin.CheckDelayState()) {
    case 0:
     digitalWrite(xPin, HIGH);
     delayPin.StartDelay (delayPinTiming[0], 1);
     break;
    case 1:
     if (delayPin.CheckDelayDone()) {
      digitalWrite(xPin, LOW);
      delayPin.StartDelay (delayPinTiming[1], 2);
     }
     break;
    case 2:
     if (delayPin.CheckDelayDone()) {
       digitalWrite(xPin, HIGH);
       delayPin.StartDelay (delayPinTiming[2], 3);
     }
     break;
    case 3:
     if (delayPin.CheckDelayDone()) {
      digitalWrite(xPin, LOW);
      delayPin.StartDelay (delayPinTiming[3], 4);
     }
     break;
    case 4:
     if (delayPin.CheckDelayDone()) {
       delayPin.NextDelayState(0);
     }
     break;
    default:
     delayPin.NextDelayState(0);
     break;
  }
}
#endif

#if defined(MULTIPLE_SERIAL)
// The function TaskDelaySerial() is the finite state machine for
// interleaving multiple Serial output streams.
int TaskDelaySerial (DelayClass &delaySerial)
{
  // The following state machine flashes the designated LED
  // by turning it on for some duration then turning it off
  // for a second duration.
  switch (delaySerial.CheckDelayState()) {
    case 0:
     Serial.print (". ");  Serial.println (delaySerial.GetId());
     if (delaySerial.GetNamedId() == 71) delaySerial.StartDelay (750, 1);
     if (delaySerial.GetNamedId() == 72) delaySerial.StartDelay (500, 1);
     if (delaySerial.GetNamedId() == 73) delaySerial.StartDelay (1000, 1);
     break;
    case 1:
      if (delaySerial.CheckDelayDone()) {
      Serial.print("\n  case 1 "); Serial.println (delaySerial.GetId());
      delaySerial.StartDelay (1000, 2);
     }
     break;
    case 2:
      if (delaySerial.CheckDelayDone()) {
      Serial.print("\n  case 2 ");  Serial.println (delaySerial.GetId());
      delaySerial.NextDelayState (0);
     }
     break;
    default:
     delaySerial.NextDelayState(0);
     break;
  }
}
#endif

// ============================================================


#if defined(MULTIPLE_LEDS)

// pin 2 is same as D2 which is also linked to builtin LED

int myPin_1 = LED_BUILTIN;   // pin #2, D2, goes to builtin LED on the ESP32 development board
int myPin_2 = 4;             // second LED driven by pin #4, D4
int myPin_3 = 22;            // third LED driven by pin #22, D22
int myPin_button = 23;       // momentary contact switch attached to pin #23, D23

static DelayClass DelayPin_1;
static int        DelayPin_1Timing[] = {1000, 500, 1000, 500};

static DelayClass DelayPin_2;
static int        DelayPin_2Timing[] = {500, 1000, 500, 1000};

static DelayClass DelayPin_3;
static int        DelayPin_3Timing[] = {1000, 500, 500, 1000};


// Button press delay for the counting logic for number of
// button presses within a given period of time. We use this
// delay for the period of time to count button presses. This
// may be used as a primitive user interface to allow changes
// in program behavior when the user enters some specified number
// of button presses during the time period.
//
// Since the button is a mechanical device we need to perform what
// is called debouncing or removing spurious signals caused by the
// mechanical parts making and breaking electrical contact as they
// move. By commenting out the define TURN_ON_DEBOUNCE below, you
// can modify the behavior of this program to not perform debouncing
// so that you can compare the effect of debouncing versus not debouncing
// a mechanical button.

#define TURN_ON_DEBOUNCE

// Use 100 milliseconds as the debounce duration. This assumes a normal button press
// will be less than 100 milliseconds.
// NOTE: You can deliberately hold the button down and wiggle it to generate suprious
//       pin state changes so this is not bullet proof however it does address the most
//       common case.
static DelayClass DelayPin_button (5000, 0);   // duration for counting number of button presses
static DelayClass DelayPin_debounce(100, 0);   // use a duration of 100 milliseconds for debouncing

static int iReadSave = LOW;    // saved button press state

#endif

#if defined(MULTIPLE_SERIAL)
// For each of the Serial Monitor delay classes we specify not only a
// starting delay and state, we also specify a unique identifier which
// is used in the TaskDelaySerial() function in the initial state 0
// to change the delay. This is just a demonstration of giving a
// DelayClass object a specific or named identifier.
static DelayClass DelaySerial1(500, 0, 71);
static DelayClass DelaySerial2(750, 0, 72);
static DelayClass DelaySerial3(250, 0, 73);
#endif


void setup() {
  // put your setup code here, to run once:
  // this function is used to do run once environment setup to
  // do those actions that only need to be done once when the
  // program starts up. These actions are such things as initialing
  // the Serial Monitor object, the WiFi object, or setting
  // pin modes to be input or output.
  
#if 1
  // serial monitor at 9600 rather than 115200
  // running serial at 115200 seems to interfere with WiFi
  Serial.begin(9600);
#else
  // serial monitor at 115200 rather than 9600
  Serial.begin(115200);
#endif

  delay(500);    // give Serial time to initialize and setup.
  
  Serial.println("begin setup()");

#if defined(TURN_ON_DEBOUNCE)
  Serial.println ("  debouncing for button press turned on.");
#else
  Serial.println ("  debouncing for button press turned off.");
#endif

#if defined(MULTIPLE_LEDS)
  // If doing the multiple LEDs demo of the interleaving of operations
  // on a single thread, set up the environment for turning the LEDs on
  // and off. Initially we turn all LEDs off.
  
  pinMode (myPin_1, OUTPUT);      // LED pin to drive the blinking of first LED 
  digitalWrite(myPin_1, LOW);     // initialize the pin to LOW to turn off LED

  pinMode (myPin_2, OUTPUT);      // LED pin to drive the blinking of second LED 
  digitalWrite(myPin_2, LOW);     // initialize the pin to LOW to turn off LED

  pinMode (myPin_3, OUTPUT);      // LED pin to drive the blinking of third LED 
  digitalWrite(myPin_3, LOW);     // initialize the pin to LOW to turn off LED

  pinMode (myPin_button, INPUT);      // button press pin to read the button state
  iReadSave = digitalRead (myPin_button);  // get the initial button press state
#endif

}

void loop() {
  // put your main code here, to run repeatedly:
  
  ulLoopCount++;    // increment this counter each time we perform loop().

#if defined(MULTIPLE_LEDS)
  // Perform the LED blink state machine for each of the LEDs
  TaskDelayPin (DelayPin_1, DelayPin_1Timing, myPin_1);
  TaskDelayPin (DelayPin_2, DelayPin_2Timing, myPin_2);
  TaskDelayPin (DelayPin_3, DelayPin_3Timing, myPin_3);

  // Button press state functionality to detect button press state
  // changes from a momentary contact switch type button. The pin
  // will be HIGH if the button is pressed and the pin will be LOW
  // when the button is released. We perform debouncing by ignoring
  // pin state changes for a short duration after we detect a pin
  // state change.

#if defined(TURN_ON_DEBOUNCE)
  if (! DelayPin_debounce.CheckDelayStart()) {  // in a debounce period?
#endif
    int  iRead = digitalRead (myPin_button);

    if (iRead != iReadSave) {            // has pin state changed?
#if defined(TURN_ON_DEBOUNCE)
      DelayPin_debounce.StartDelay();    // start debounce period, same delay
#endif
      if (iRead == HIGH) {
        if (! DelayPin_button.CheckDelayStart()) {
          // button has been pressed and we are not in a counting period.
          // start our count and start the timer for the counting duration.
          DelayPin_button.ulCounter = 1;
          DelayPin_button.StartDelay(5000);
          Serial.print("  start button press count ");
          Serial.println (DelayPin_button.ulCounter);
        } else if ( ! DelayPin_button.CheckDelayDone()) {
          // we are in a counting period so lets count this button press.
          DelayPin_button.ulCounter++;
        }
      }
      // save the current pin state so that we can use it to compare
      // next time in order to see pin state changes. remember that the
      // ESP32 is polling the pin a lot faster than our finger is moving
      // so a single physical button press with our finger will show a
      // HIGH pin state over multiple times through the loop() function.
      iReadSave = iRead;
      Serial.print (" pin myPin_button read ");
      Serial.println (iRead);
    }
#if defined(TURN_ON_DEBOUNCE)
  } else if (DelayPin_debounce.CheckDelayDone()) {
    // if our debounce period has expired then clear the delay indicator
    // so that the next time the button is pressed we can start over again.
    DelayPin_debounce.ClearDelayStart();
  }
#endif

  if (DelayPin_button.CheckDelayDone()) {
      // check to see if our period for counting button presses has
      // expired or not. If it has then print the count and then
      // clear the delay so that we can start the count all over again
      // the next time the button is pressed.
      Serial.print (" button pushed count = ");
      Serial.println (DelayPin_button.ulCounter);
      DelayPin_button.ClearDelayStart();
  }
#endif

#if defined(MULTIPLE_SERIAL)
  TaskDelaySerial (DelaySerial1);
  TaskDelaySerial (DelaySerial2);
  TaskDelaySerial (DelaySerial3);
#endif

}
