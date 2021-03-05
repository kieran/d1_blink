# Control the LED on an esp8266 dev board (d1 mini)

Basically just me learning about ESPs

[![Demo](https://i.imgur.com/0me7qhU.png)](https://imgur.com/a/sBJVHM4)

## Endpoints
- [turn the LED on](http://esp8266.local/on)
- [turn the LED off](http://esp8266.local/off)
- [toggle the LED](http://esp8266.local/toggle)

## How to do this on your computer

Uses the [PlatformIO](https://platformio.org) VS Code plugin for building / interfacing with the mini

You'll need an ESP8266 D1 Mini - I bought [these](https://www.amazon.ca/gp/product/B07WWFND3B/) but I think any D1 Mini would probably work.

You'll need to edit the network SSID and password at the top of `src/main.cpp` to match your network
