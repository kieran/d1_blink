#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef NETWORK_SSID
#define NETWORK_SSID "wifi network"
#define NETWORK_PASS "password"
#endif

const char* ssid = NETWORK_SSID;
const char* password = NETWORK_PASS;

ESP8266WebServer server(80);

// reads LED state
bool led(void){
  return digitalRead(LED_BUILTIN) == LOW;
}

// writes LED state
bool led(bool state){
  digitalWrite(LED_BUILTIN, state ? LOW : HIGH);
  return led();
}

// LED state as a String
String s_led(void){
  return led() ? "ON" : "OFF";
}

// route: /
void handleRoot() {
  led(true);
  server.send(200, "text/plain", "hello from esp8266!");
  led(false);
}

// route: 404
void handleNotFound() {
  led(true);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  led(false);
}

void setup(void) {

  // set the internal LED as an output - not sure why?
  pinMode(LED_BUILTIN, OUTPUT);

  // turn the LED off (it defaults to on)
  led(false);

  // init logging interface over serial port
  Serial.begin(115200);

  // init the wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for the wifi connection
  Serial.println(""); // a vertical space
  while (WiFi.status() != WL_CONNECTED) {
    // print 1 dot every half second while we're trying to connect
    delay(500);
    Serial.print(".");
  }

  // wifi is connected - print debug info
  Serial.println(""); // a vertical space
  // really wish we could do these in one println each,
  // but it freaks out about mixing types ¯\_(ツ)_/¯
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // this advertises the device locally at "esp8266.local"
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started at http://esp8266.local");
  }

  //
  // define webserver routes
  //
  server.on("/", handleRoot);

  // looks like `[](){...}` is c++ speak for a lambda function
  server.on("/on", []() {
    led(true);
    server.send(200, "text/plain", "LED is now " + s_led());
  });

  server.on("/off", []() {
    led(false);
    server.send(200, "text/plain", "LED is now " + s_led());
  });

  server.on("/toggle", []() {
    // toggle the LED
    led(!led());
    // send a message to the browser including the new state
    server.send(200, "text/plain", "TOGGLE: LED is now " + s_led());
  });

  server.onNotFound(handleNotFound);

  //
  // Start the web server
  //
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
