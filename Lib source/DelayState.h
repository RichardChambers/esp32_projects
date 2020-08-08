

#if ! defined(INCLUDE_DELAYSTATE_LIB_H)

#define INCLUDE_DELAYSTATE_LIB_H

#include "Arduino.h"

class StateClass {
private:
  static unsigned long ulIdCount;
  
  unsigned long ulState;
  unsigned long ulId;
  unsigned long ulNamedId;
  
public:
  unsigned long ulCounter;
  
  StateClass (unsigned long state = 0, unsigned long namedid = 0) :
        ulState(state), ulId (ulIdCount++), ulNamedId(namedid), ulCounter(0)
     {
     }
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

class DelayClass {
private:
  static  unsigned long ulIdCount;
  
  bool          bDelayStart;   // indicates if a delay has been started or not.
  unsigned long ulDuration;    // duration to delay in milliseconds
  unsigned long ulMilliStart;  // start of delay from millis() function
  unsigned long ulId;          // identifier generated for this DelayClass object
  unsigned long ulNamedId;     // named identifier assigned to this DelayClass object

  StateClass myState;          // state in finite state machine

  DelayClass *nextInChain;     // allows single linked list of delay objects

public:
  unsigned long ulCounter;
 
  DelayClass (unsigned long ulDur = 0, unsigned long ulSt = 0, unsigned long ulId = 0) :
    bDelayStart(false), ulDuration(ulDur), ulMilliStart(millis()), ulId(ulIdCount++),
    ulNamedId(ulId), myState(ulSt, ulId), nextInChain(NULL), ulCounter(0)
    {
    }
  ~DelayClass () {}
  unsigned long StartDelay (unsigned long ulDur = 0, unsigned long ulSt = 0, unsigned long ulId = 0) {
       unsigned long ulDurOld = ulDuration;
       if (ulDur != 0) ulDuration = ulDur;
       if (ulSt != 0) myState.NextState(ulSt);
       if (ulId != 0) ulNamedId = ulId;
       ulMilliStart = millis();
       bDelayStart = true;
       return ulDurOld;
    }
  unsigned long StopDelay (void) {
       unsigned long ulMilli = millis();
       ulDuration = ulMilli - ulMilliStart;
       return ulDuration;
  }
  bool CheckDelayDone (void) {
       unsigned long ulMilli = millis() - ulMilliStart;
       return (bDelayStart && (ulMilli >= ulDuration));
    }
  bool CheckDelayStart (void) { return bDelayStart; }
  bool ClearDelayStart (void) {
      bool bDone = bDelayStart;
      bDelayStart = false;
      return bDone;
    }
  unsigned long CheckDelayState (void) { return myState.CheckState(); }
  unsigned long GetId (void) { return ulId; }
  unsigned long AssignNamedId (unsigned long id) {
       unsigned long ulId = ulNamedId;
       ulNamedId = id;
       return ulId;
    }
  unsigned long GetNamedId (void) { return ulNamedId; }
  unsigned long NextDelayState (unsigned long ulNext) {
       return myState.NextState(ulNext);
    }

    DelayClass * SetNextInChain (DelayClass * dObj)
    {
      DelayClass * oldObj = nextInChain;
      nextInChain = dObj;
      return oldObj;
    }

    DelayClass * NextInChain (void) { return nextInChain; }
    
    DelayClass & operator = (const DelayClass & rhs) {
      bDelayStart = rhs.bDelayStart;
      ulDuration = rhs.ulDuration;
      ulMilliStart = rhs.ulMilliStart;
      ulNamedId = rhs.ulNamedId;
      myState = rhs.myState;
      nextInChain = rhs.nextInChain;
      
      return *this;
    }

    StateClass  GetStateClass (void) { return myState; }
};

#endif
