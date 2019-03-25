#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

char ssid[20], password[20];
String s;

ESP8266WiFiMulti WiFiMulti;

void setup() {

  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  Serial.setTimeout(10000);
  Serial.print("Enter SSID = ");     s = Serial.readString(); Serial.println(s); s.toCharArray(ssid, s.length());     ssid[s.length()] = 0;
  Serial.flush();
  Serial.print("Enter password = "); s = Serial.readString(); Serial.println(s); s.toCharArray(password, s.length()); password[s.length()] = 0;

  
  
  

  
  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Home_BVD", "360486d4tdt");

  
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    USE_SERIAL.print("[HTTP] begin...\n");
    http.begin("http://jsonplaceholder.typicode.com/users"); //HTTP

    USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        USE_SERIAL.println(payload);
      }
    } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(10000);
}
