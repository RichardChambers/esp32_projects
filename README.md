# esp32_projects
various projects with an ESP32 microcontroller using various sensors and breadboards

I have purchased from Amazon.com two ESP32S development boards with the ESP32 microcontroller.

MELIFE 2 Pack ESP32 ESP-32S Development Board 2.4GHz Dual-Mode WiFi + Bluetooth Dual Cores Microcontroller Processor Integrated with ESP32s Antenna RF AMP Filter AP STA for Arduino IDE

**Helpful websites**

The Internet of Things with ESP32 containing links to a variety of technical articles http://esp32.net/

**Product Description**

The product description says:
 - 2 PACK 2.4GHz Dual Mode WiFi + Bluetooth Development Board
 - Ultra-Low power consumption, works perfectly with the Arduino IDE
 - Support LWIP protocol, Freertos
 - SupportThree Modes: AP, STA, and AP+STA
 - ESP32 is a safe, reliable, and scalable to a variety of applications

Development Board:
 - https://github.com/espressif/arduino-esp32
 - https://github.com/nodemcu/nodemcu-firmware/tree/dev-esp32
 - https://github.com/Nicholas3388/LuaNode

See also https://www.arduino.cc/en/Tutorial/HomePage with various tutorials about Arduino IDE.

If it can't get bluetooth working?

Using Node32s as the Board selection. To reference GPIO pins in code use just the number, for example "digitalWrite(13, HIGH)" sets GPIO13 high.
The built-in LED to GPIO2.

My development environment was:
 - the Arduino IDE 1.8.13 on a Microsoft Surface Pro 2 with Windows 10
 - using a standard USB to micro-USB cable from a Samsung cell phone
   - powers the ESP32
   - provides serial cable connectivity for downloading programs and the IDE Serial Monitor

The Arduino IDE shows the following settings:
 - Board: "NodeMCU-32S"
 - Upload Speed: "921600"
 - Flash Frequency: "80MHz"
 
 Get Board Info shows "Unknown board".
 
 My development environment currently:
  - the Arduino IDE 2.0 on a Dell laptop with Windows 10

**Adding support for ESP32 in Arduino IDE 2**

I installed Arduino IDE 2.0 and it worked fine out of the box with an Arduino UNO R3. I then used the tutorial at https://randomnerdtutorials.com/installing-esp32-arduino-ide-2-0/ to install the Espresif ESP32 libraries at `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`.

I then ran into a problem with my ESP32 not automatically creating a COM port when it was plugged in. I then found this posting, https://www.esp32.com/viewtopic.php?t=5841 which mentioned that additional device drives needed to be installed.

> 2) The USB connection is not handled by the ESP32, as an ESP32 only has serial lines (rx and tx), no USB. A dev board typically includes some sort of USB to serial converter chip (sometimes refered to as FTDI). This chip communicates with your computer, resulting in the creation of a virtual COM port. If you have a driver for that chip instaled, otherwise it won't work. Without a driver the chip probably turns up as unsupported or unrecognised device in your hardware list. Although the ESP32 module may be the same, you might have a dev board with a different USB to serial converter than other devices you might have tested on your computer. So if one dev board works and another one doesn't, that does not necessarily mean the board is defective. You might just need to install a driver for the USB to serial chip on your dev board.

I first installed the CH340 USB device driver https://sparks.gogo.co.nz/ch340.html which did not help

I then installed the CP210x USB to UART Bridge VCP Drivers from https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers and that worked. I now can access a plugged in Arduino UNO R3 from one COM port and an ESP32 from another COM port.

To compile I target "NodeMCU32S".

**Adding support for ESP8266 in Arduino IDE 2**

I have an ESP8266 with WiFi for other experiments. It uses the same COM port as the ESP32. I did have to modify the File -> Preferences -> Additional libraries which now looks like:

`http://arduino.esp8266.com/stable/package_esp8266com_index.json, https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`

To compile I target "Generic ESP8266 Module".

**Various articles about the ESP32 and using it with sensors**

ESP32: DHT11/22 Humidity Temperature Sensor Interfacing Example https://circuits4you.com/2019/01/25/esp32-dht11-22-humidity-temperature-sensor-interfacing-example/


**RTOS for the ESP32 Development Board**

FreeRTOS https://www.freertos.org/index.html

Espressif ESP-IDF Programming Guide - Section on FreeRTOS https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html

ESP32: Introduction to FreeRTOS https://icircuit.net/esp32-introduction-freertos/1297

Mongoose OS https://mongoose-os.com/

ARM Mbed OS https://os.mbed.com/
