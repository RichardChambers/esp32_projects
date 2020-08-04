
// ESP32 sketch. Must set Tools  -> Board to NodeMCU-32S


#include <WiFi.h>

static unsigned long ulLoopCount = 0;

class StateClass {
private:
  static unsigned long ulIdCount;
  
  unsigned long ulState;
  unsigned long ulId;
  unsigned long ulNamedId;
  
public:
  StateClass (unsigned long state = 0, unsigned long namedid = 0) : ulState(state), ulId (ulIdCount++), ulNamedId(namedid) { }
  ~StateClass () {}
  
  unsigned long CheckState (void) { return ulState; }
  unsigned long GetId (void) { return ulId; }
  unsigned long AssignNamedId (unsigned long id) {
       unsigned long ulId = ulNamedId;
       ulNamedId = id;
       return ulId;
    }
  unsigned long GetNamedId (void) { return ulNamedId; }
  unsigned long NextState (unsigned long ulNext) {
       unsigned long ulSt = ulState;
       ulState = ulNext;
       return ulSt;
    }

  StateClass & operator = (const StateClass &rhs) {
      ulState = rhs.ulState;
      // ulId = rhs.ulId;     // we want each StateClass object with a unique auto id
      ulNamedId = rhs.ulNamedId;
      return *this;
    }

  bool operator == (const StateClass & rhs)
    {
      return ulNamedId == rhs.ulNamedId && ulState == rhs.ulState;
    }
};

unsigned long StateClass::ulIdCount = 0;


class DelayClass {
private:
  static  unsigned long ulIdCount;
  
  bool          bDelayStart;   // indicates if a delay has been started or not.
  unsigned long ulDuration;    // duration to delay in milliseconds
  unsigned long ulMilliStart;  // start of delay from millis() function
  unsigned long ulState;       // state in finite state machine
  unsigned long ulId;          // identifier generated for this DelayClass object
  unsigned long ulNamedId;     // named identifier assigned to this DelayClass object

  StateClass myState;

  DelayClass *nextInChain;

public:
  DelayClass (unsigned long ulDur = 0, unsigned long ulSt = 0, unsigned long ulId = 0) : bDelayStart(false), ulDuration(ulDur), ulMilliStart(millis()), ulState(ulSt), ulId(ulIdCount++), ulNamedId(ulId), myState(ulSt, ulId), nextInChain(NULL) {
    }
  ~DelayClass () {}
  unsigned long StartDelay (unsigned long ulDur = 0, unsigned long ulSt = 0, unsigned long ulId = 0) {
       unsigned long ulDurOld = ulDuration;
       if (ulDur != 0) ulDuration = ulDur;
       if (ulSt != 0) ulState = ulSt;
       if (ulId != 0) ulNamedId = ulId;
       ulMilliStart = millis();
       bDelayStart = true;
       return ulDurOld;
    }
  bool CheckDelayDone (void) {
       unsigned long ulMilli = millis() - ulMilliStart;
       return (bDelayStart && (ulMilli >= ulDuration));
    }
  bool CheckDelayStart (void) { return bDelayStart; }
  bool ClearDelayStart (void) {
      bool bDone = CheckDelayDone();
      bDelayStart = false;
      return bDone;
    }
  unsigned long CheckDelayState (void) { return ulState; }
  unsigned long GetId (void) { return ulId; }
  unsigned long AssignNamedId (unsigned long id) {
       unsigned long ulId = ulNamedId;
       ulNamedId = id;
       return ulId;
    }
  unsigned long GetNamedId (void) { return ulNamedId; }
  unsigned long NextDelayState (unsigned long ulNext) {
       unsigned long ulSt = ulState;
       ulState = ulNext;
       myState.NextState(ulNext);
       return ulSt;
    }

    DelayClass & operator = (const DelayClass & rhs) {
      bDelayStart = rhs.bDelayStart;
      ulDuration = rhs.ulDuration;
      ulMilliStart = rhs.ulMilliStart;
      ulState = rhs.ulState;
      ulNamedId = rhs.ulNamedId;
      myState = rhs.myState;
      
      return *this;
    }
};

unsigned long DelayClass::ulIdCount = 0;



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

int myPin = LED_BUILTIN; // 4;
int myPin_4 = 4;
int myPin_13 = 5;

static DelayClass DelayPin;
static int        DelayPinTiming[] = {1000, 500, 1000, 500};
static DelayClass DelayPin_4;
static int        DelayPin_4Timing[] = {500, 1000, 500, 1000};
static DelayClass DelayPin_13;
static int        DelayPin_13Timing[] = {1000, 500, 500, 1000};

static DelayClass DelaySerial1(500, 0, 71);
static DelayClass DelaySerial2(750, 0, 72);
static DelayClass DelaySerial3(250, 0, 73);

void setup() {
  // put your setup code here, to run once:

  DelayPin = DelayClass(1000, 0);
  pinMode (myPin, OUTPUT);
  digitalWrite(myPin, LOW);    // initialize the pin to LOW to turn off LED

  pinMode (myPin_4, OUTPUT);
  digitalWrite(myPin_4, LOW);    // initialize the pin to LOW to turn off LED

  pinMode (myPin_13, OUTPUT);
  digitalWrite(myPin_13, LOW);    // initialize the pin to LOW to turn off LED

#if 1
  // serial monitor at 9600 rather than 115200
  // running serial at 115200 seems to interfere with WiFi
  Serial.begin(9600);
#else
  // serial monitor at 115200 rather than 9600
  Serial.begin(115200);
#endif
}

void loop() {
  // put your main code here, to run repeatedly:
  
  ulLoopCount++;    // increment this counter each time we perform loop().

  TaskDelayPin (DelayPin, DelayPinTiming, myPin);
  TaskDelayPin (DelayPin_4, DelayPin_4Timing, myPin_4);
  TaskDelayPin (DelayPin_13, DelayPin_13Timing, myPin_13);

//  TaskDelaySerial (DelaySerial1);
//  TaskDelaySerial (DelaySerial2);
//  TaskDelaySerial (DelaySerial3);
  
}
