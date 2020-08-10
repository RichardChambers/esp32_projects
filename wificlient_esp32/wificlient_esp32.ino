
// ESP32 sketch. Must set Tools  -> Board to NodeMCU-32S

// int LED_BUILTIN = 2;   // this definition is not needed for NodeMCU-32S

#include <WiFi.h>

// You will need to do Sketch -> Add file ...  to add the two files
// needed for the DelayClass and StateClass to this sketch. The
// two files are in the Lib source folder.
//  - DelayState.h
//  - DelayState.cpp
#include "DelayState.h"

static unsigned long ulLoopCount = 0;


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
const char *ssid = "";
const char *password = "";

WiFiServer  server(80);   // our server will listen on port 80 for connection requests.
WiFiClient  myclient;     // connection request handler for servicing the request

static DelayClass DelayClient;

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
  
}
