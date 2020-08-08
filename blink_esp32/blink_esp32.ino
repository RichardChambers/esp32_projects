
// ESP32 sketch. Must set Tools  -> Board to NodeMCU-32S

#include "DelayState.h"

static unsigned long ulLoopCount = 0;


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


// ============================================================

#define MYPIN_13
#define MYPIN_6

// pin 2 is same as D2 which is also linked to builtin LED
// pin 4 is same as D4
// pin 22 is same as D22
// pin 23 is same as D23

int myPin = LED_BUILTIN; // 4;
int myPin_4 = 4;
#if defined(MYPIN_13)
int myPin_13 = 22;
#endif
#if defined(MYPIN_6)
int myPin_6 = 23;
#endif

static DelayClass DelayPin;
static int        DelayPinTiming[] = {1000, 500, 1000, 500};
static DelayClass DelayPin_4;
static int        DelayPin_4Timing[] = {500, 1000, 500, 1000};
static DelayClass DelayPin_13;
static int        DelayPin_13Timing[] = {1000, 500, 500, 1000};

static DelayClass DelayPin_6 (5000, 0);

static DelayClass DelaySerial1(500, 0, 71);
static DelayClass DelaySerial2(750, 0, 72);
static DelayClass DelaySerial3(250, 0, 73);

static int iReadSave = LOW;


void setup() {
  // put your setup code here, to run once:
  
#if 1
  // serial monitor at 9600 rather than 115200
  // running serial at 115200 seems to interfere with WiFi
  Serial.begin(9600);
#else
  // serial monitor at 115200 rather than 9600
  Serial.begin(115200);
#endif

  delay(500);    // give Serial to initialize and setup.
  
  Serial.println("begin setup()");

#if defined(MYPIN_13)
  Serial.print ("   MYPIN_13 defined myPin_13 = ");
  Serial.println (myPin_13);
#endif

  DelayPin = DelayClass(1000, 0);
  
  pinMode (myPin, OUTPUT);
  digitalWrite(myPin, LOW);    // initialize the pin to LOW to turn off LED

  pinMode (myPin_4, OUTPUT);
  digitalWrite(myPin_4, LOW);    // initialize the pin to LOW to turn off LED

#if defined(MYPIN_13)
  pinMode (myPin_13, OUTPUT);
  digitalWrite(myPin_13, LOW);    // initialize the pin to LOW to turn off LED
#endif

#if defined (MYPIN_6)
  Serial.print ("   MYPIN_6 defined myPin_6 = ");
  Serial.println (myPin_6);

  pinMode (myPin_6, INPUT);
 
  Serial.print (" pin myPin_6 iReadSave ");
  Serial.println (iReadSave);
  iReadSave = digitalRead (myPin_6);
  Serial.print (" pin myPin_6 read ");
  Serial.println (iReadSave);
#endif
}

void loop() {
  // put your main code here, to run repeatedly:
  
  ulLoopCount++;    // increment this counter each time we perform loop().

  TaskDelayPin (DelayPin, DelayPinTiming, myPin);
  TaskDelayPin (DelayPin_4, DelayPin_4Timing, myPin_4);
#if defined(MYPIN_13)
  TaskDelayPin (DelayPin_13, DelayPin_13Timing, myPin_13);
#endif

#if defined (MYPIN_6)
  int  iRead = digitalRead (myPin_6);

  if (iRead != iReadSave) {
    if (iRead == HIGH) {
      if (! DelayPin_6.CheckDelayStart()) {
        DelayPin_6.ulCounter = 1;
        DelayPin_6.StartDelay(5000);
        Serial.print("  start button press count ");
        Serial.println (DelayPin_6.ulCounter);
      } else if ( ! DelayPin_6.CheckDelayDone()) {
        DelayPin_6.ulCounter++;
      }
    }
    iReadSave = iRead;
    Serial.print (" pin myPin_6 read ");
    Serial.println (iRead);
  } 
  if (DelayPin_6.CheckDelayDone()) {
      Serial.print (" button pushed count = ");
      Serial.println (DelayPin_6.ulCounter);
      DelayPin_6.ClearDelayStart();
  }
#endif

//  TaskDelaySerial (DelaySerial1);
//  TaskDelaySerial (DelaySerial2);
//  TaskDelaySerial (DelaySerial3);
  
}
