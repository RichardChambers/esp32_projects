
// ESP32 sketch. Must set Tools  -> Board to NodeMCU-32S

// int LED_BUILTIN = 2;   // this definition is not needed for NodeMCU-32S

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


int TestClientWrite(WiFiClient &clientWiFi)
{
  if (! clientWiFi.connected()) return 0;
  if (! clientWiFi.available()) return 0;

  int mySock = clientWiFi.fd();
  fd_set set;
  struct timeval tv = {0};  // zero out timeout as we are polling using select()

  FD_ZERO(&set);        // empties the set
  FD_SET(mySock, &set); // adds FD to the set

  return (select(mySock + 1, NULL, &set, NULL, &tv) > 0);
}

int TestClientRead(WiFiClient &clientWiFi)
{
  if (! clientWiFi.connected()) return 0;
  if (! clientWiFi.available()) return 0;
  
  int mySock = clientWiFi.fd();
  fd_set set;
  struct timeval tv = {0};  // zero out timeout as we are polling using select()

  FD_ZERO(&set);        // empties the set
  FD_SET(mySock, &set); // adds FD to the set

  return (select(mySock + 1, &set, NULL, NULL, &tv) > 0);
}


int readRequest (WiFiClient & client, char getRequestLine[126])
{
  int  iCount = 0;
  
  bool currentLineIsBlank = true;  // an http request ends with a blank line
  unsigned short getRequestLineIndex = 0;

  while (client.connected()) {
     if (client.available()) {
      int myc = client.read();
      if (myc >= 0) {
        char c = myc;
        iCount++;
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          break;
        }

        if (c == '\n') {
          if (strncmp (getRequestLine, "GET ", 4) == 0) getRequestLineIndex = 128;
          // you're starting a new line
          currentLineIsBlank = true;
          if (getRequestLineIndex != 128) getRequestLineIndex = 0;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
          if (getRequestLineIndex < 120) {
            getRequestLine[getRequestLineIndex++] = c;
          }
        }
      }
    }
  }

  Serial.println("");
  Serial.print ("readRequest() iCount = ");
  Serial.print (iCount);
  Serial.print ("  GET line reads ");
  Serial.println (getRequestLine);
  return iCount;
}

int writeResponseHeader (WiFiClient & client)
{
  int iStatus = 0;

  Serial.print ("writeResponseHeader() TestClientWrite() ");
  Serial.println (TestClientWrite(client));

  if (client.connected()) {
    iStatus = 1;
    // send a standard http response header
    client.print("HTTP/1.0 200 OK\r\n");
    client.print("Content-Type: text/html\r\n");
    client.print("Connection: close\r\n");  // the connection will be closed after completion of the response
//    client.print("Refresh: 5\r\n");  // refresh the page automatically every 5 sec
    client.print("\r\n");
  }

//  Serial.print("writeResponseHeader() iStatus = ");
//  Serial.println(iStatus);
  return iStatus;
}

int writeResponseHeader404 (WiFiClient & client)
{
  int iStatus = 0;
  
  Serial.print ("writeResponseHeader404() TestClientWrite() ");
  Serial.println (TestClientWrite(client));

  if (client.connected()) {
    iStatus = 1;
    // send a standard http response header
    client.print("HTTP/1.0 404 OK\r\n");
    client.print("Content-Type: text/html\r\n");
    client.print("Connection: close\r\n");  // the connection will be closed after completion of the response
//    client.print("Refresh: 5\r\n");  // refresh the page automatically every 5 sec
    client.print("\r\n");
  }

//  Serial.print("writeResponseHeader() iStatus = ");
//  Serial.println(iStatus);
  return iStatus;
}

int writeResponseBody (WiFiClient &client)
{
  int iStatus = 0;
  
  Serial.print ("writeResponseBody() TestClientWrite() ");
  Serial.println (TestClientWrite(client));

  if (client.connected()) {
    iStatus = 1;
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<body>");
    client.println("<h1>A Web Page</h1>");
    client.print("<p> ulLoopCount = ");
    client.print(ulLoopCount);
    client.println("</p>");
    client.println("</body>");
    client.println("</html>");
  }
  
  Serial.print("writeResponseBody() iStatus = ");
  Serial.println(iStatus);
  return iStatus;
}

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


int TaskClient (DelayClass & clientDelay, WiFiServer & serverWiFi, WiFiClient & clientWiFi)
{
    switch (clientDelay.CheckDelayState()) {
    case 0:
      clientWiFi = serverWiFi.available();
      if (clientWiFi) {
        Serial.println("new client");
        clientDelay.NextDelayState(1);
      }
      break;
    case 1:
      if (clientWiFi.connected()) {
        char getRequestLine[126] = {0};
        int iLen = readRequest (clientWiFi, getRequestLine);
        if (iLen > 20) clientDelay.NextDelayState(3); else clientDelay.NextDelayState(5);
      }
      break;
    case 3:
      if (clientDelay.CheckDelayStart() && ! clientDelay.CheckDelayDone()) {
        break;
      }
      if (clientWiFi.connected()) {
          writeResponseHeader (clientWiFi);
          clientDelay.ClearDelayStart();
          clientDelay.NextDelayState(4);
      }
      break;
    case 4:
      if (clientDelay.CheckDelayStart() && ! clientDelay.CheckDelayDone()) {
        break;
      }
      if (clientWiFi.connected()) {
          writeResponseBody (clientWiFi);
          clientDelay.StartDelay (100, 5);
      }
      break;
    case 5:
      if (clientDelay.CheckDelayStart() && ! clientDelay.CheckDelayDone()) {
        break;
      }
      if (clientDelay.CheckDelayDone()) {
         // close the connection:
         close(clientWiFi.fd());
         clientWiFi.stop();
         Serial.println("client disonnected");
         clientDelay.NextDelayState(0);
      }
      break;
    default:
      clientDelay.NextDelayState(0);
      break;
  }

  return 0;
}
// ============================================================

// Following information needed for a successful connect to an
// Access Point in order to available for other devices to connect
// to our server.
// We need to know the identifier of the Access Point we are to use
// along with the password for the Access Point.
const char *ssid = "Pinkey Blue";
const char *password = "R1chardEla1me";

WiFiServer  server(80);   // our server will listen on port 80 for connection requests.
WiFiClient  myclient;     // connection request handler for servicing the request

int myPin = LED_BUILTIN;

static DelayClass DelayPin;
static int        DelayPinTiming[] = {1000, 500, 1000, 500};

static DelayClass DelayClient;

static DelayClass DelaySerial1(500, 0, 71);
static DelayClass DelaySerial2(750, 0, 72);
static DelayClass DelaySerial3(250, 0, 73);

void setup() {
  // put your setup code here, to run once:

  DelayPin = DelayClass(1000, 0);
  pinMode (myPin, OUTPUT);
  digitalWrite(myPin, LOW);    // initialize the pin to LOW to turn off LED

#if 1
  // serial monitor at 9600 rather than 115200
  // running serial at 115200 seems to interfere with WiFi
  Serial.begin(9600);
#else
  // serial monitor at 115200 rather than 9600
  Serial.begin(115200);
#endif

  // set up our WiFi server connection to the Access Point
  // so that we can obtain an IP address from the AP and
  // then start our server listening for connection requests.
  WiFi.begin (ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("WiFi Connected, IP Address ");
  Serial.println(WiFi.localIP());
  
  server.begin();
  DelayClient = DelayClass(0, 0);  // initialize our client delay and state
}

void loop() {
  // put your main code here, to run repeatedly:
  
  ulLoopCount++;    // increment this counter each time we perform loop().

  TaskClient (DelayClient, server, myclient);

  TaskDelayPin (DelayPin, DelayPinTiming, myPin);

//  TaskDelaySerial (DelaySerial1);
//  TaskDelaySerial (DelaySerial2);
//  TaskDelaySerial (DelaySerial3);
  
}
