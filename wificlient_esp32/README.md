wificlient_esp32

An Arduino sketch for the ESP32 which contains an example of using WiFi to provide a simple
web server example along with the `DelayState` class to provide pseudo multi-threading with
a state machine to throttle the server output rate.

To use this source code you will need to specify the SSID and password of your WiFi router
which is used by the `WiFi` class to be allowed to connect to the WiFi LAN and obtain an IP address.
The web server created uses the standard port number of 80. This can be changed in the definition
of the `WiFiServer` variable.

    // Following information needed for a successful connect to an
    // Access Point in order to available for other devices to connect
    // to our server.
    // We need to know the identifier of the Access Point we are to use
    // along with the password for the Access Point.
    const char *ssid = "";
    const char *password = "";

    WiFiServer  server(80);   // our server will listen on port 80 for connection requests.
    WiFiClient  myclient;     // connection request handler for servicing the request

The sketch issues logs to the Serial monitor which is set to 9600 baud. Testing indicating problems
with higher baud rates for some reason.

The `DelayState` class is used to provide some degree of throttling to the web server output so that
the receiving client has the time to process the sent HTML header and the HTML document itself. So
the response is sent using a state machine along with delays so that the internal buffers of the `WiFi`
class are not filled causing transmission problems.

It may be there are other mechanisms that would be more flexible and more appropriate for sending a
document as a background task using a callback for completion. That is something to explore in the
future.

The function `writeResponseBody()` contains a hard coded example HTML document. This would be the place for
changes to send an HTML document of some kind.
