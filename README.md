# esp32_projects
various projects with an ESP32 microcontroller using various sensors and breadboards

I have purchased from Amazon.com two ESP32 microcontrollers.

MELIFE 2 Pack ESP32 ESP-32S Development Board 2.4GHz Dual-Mode WiFi + Bluetooth Dual Cores Microcontroller Processor Integrated with ESP32s Antenna RF AMP Filter AP STA for Arduino IDE

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

If it can't get bluetooth working?
Using Node32s as the Board selection. To reference GPIO pins in code use just the number, for example "digitalWrite(13, HIGH)" sets GPIO13 high.
The built-in LED to GPIO2.

I am using these devices with the Arduino IDE on a Microsoft Surface Pro 2 with Windows 10.
