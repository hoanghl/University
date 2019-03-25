#define READCHAR  1
#define SSIDPASS  0
#define HTMLPOST  2

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

short state;
char ssid[20], password[20];
short read_level = 0;     // 0: read SSID ; 1: read Password
String s;
char c;


ESP8266WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(115200);
  state = SSIDPASS ;
}

void loop() {
  switch(state){
    case READCHAR:{
      if (Serial.available() > 0){
        c = Serial.read();

        if (c == '#'){
          state = SSIDPASS;
          read_level++;
        }
        else {
          s += c;
        }
      }
      break;
    }

    case SSIDPASS:{
      switch(read_level){
        case 0:{
          state = READCHAR; s = "";

          Serial.print("Enter SSID: ");
          break;
        }
        case 1:{
          Serial.println(s); 
          s.toCharArray(ssid, s.length());
          ssid[s.length()] = 0;

          state = READCHAR; s = "";
          Serial.print("Enter Password: ");
          break;
        }
        case 2:{
          Serial.println(s);
          s.toCharArray(password, s.length());
          password[s.length()] = 0;

          state = HTMLPOST;          
          WiFi.mode(WIFI_STA);
          WiFiMulti.addAP(ssid, password);
          break;
        }
      }
      break;
    }

    case HTMLPOST:{
      Serial.println("In HTML POST");
      // wait for WiFi connection
      if ((WiFiMulti.run() == WL_CONNECTED)) {
    
        HTTPClient http;
    
        Serial.print("[HTTP] begin...\n");
        http.begin("http://jsonplaceholder.typicode.com/users"); //HTTP
    
        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();
    
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    
          // file found at server
          if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            Serial.println(payload);
          }
        } 
        else 
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());        
    
        http.end();
      }
  
      delay(10000);
      break;
    }
  }
  delay(10);
}
