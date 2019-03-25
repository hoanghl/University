#include "ESP8266WiFi.h"

#define SCAN        0
#define THINGSPEAK  2
#define SSIDPASS    3
#define CONNCT      4
#define READCHAR    5


short read_level = 0;
char c = 0;
String s = "";
int fakeTimer = 1;

short state = 0;            // control states in loop()
int n;                      // hold position of Wifi.SSID[]

/******************* for THINGSPEAK connection ***************************/
String apiKey = "DUANLWTJNUMVT7FW";    //  Enter your Write API key from ThingSpeak

char     ssid[20], password[20];
const char* server = "api.thingspeak.com";

/**************************************************************************/
WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");
}

void loop() {
  switch (state){
    case SCAN:{
      Serial.println("scan start");
  
      // WiFi.scanNetworks will return the number of networks found
      n = WiFi.scanNetworks();
      Serial.println("scan done");
      if (n == 0) {
        Serial.println("no networks found");
      } else {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
          // Print SSID and RSSI for each network found
          Serial.print(i + 1);
          Serial.print(": ");
          Serial.print(WiFi.SSID(i));
          Serial.print(" (");
          Serial.print(WiFi.RSSI(i));
          Serial.print(")");
          Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
          delay(10);
        }
      }
      Serial.println("");
    
      state = SSIDPASS;
      break;
    }

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
  
            Serial.print("Enter SSID number: ");
            break;
          }
          case 1:{
            n = s.toInt() - 1;
            Serial.println(n+1);
            WiFi.SSID(n).toCharArray(ssid, WiFi.SSID(n).length() + 1);
              
            state = READCHAR; s = "";
            Serial.print("Enter Password: ");
            break;
          }
          case 2:{
            s.toCharArray(password, s.length()+1);
            Serial.println(s);

            
            state = CONNCT;
            WiFi.begin(ssid, password);
            break;
          }
        }
        break;
      }
  
      case CONNCT:{
        if (WiFi.status() != WL_CONNECTED){
          if (++fakeTimer == 50){
            fakeTimer = 0;
            Serial.print(".");
          }
        }
        else {
          Serial.println();
          Serial.println("WiFi connected");
          state = THINGSPEAK;
        }
        break;
      }
    

    case THINGSPEAK:{
      float h = (float)random(30, 90);
      float t = (float)random(20, 35);
        
      if (isnan(h) || isnan(t)){
       Serial.println("Failed to read from DHT sensor!");
       return;
      }
    
      if (client.connect(server,80)){  //  "184.106.153.149" or api.thingspeak.com
      String postStr = apiKey;
      postStr +="&field1=";
      postStr += String(t);
      postStr +="&field2=";
      postStr += String(h);
      postStr += "\r\n\r\n";
     
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);
     
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.print(" degrees Celcius, Humidity: ");
      Serial.print(h);
      Serial.println("%. Send to Thingspeak.");
        }
        client.stop();
     
        Serial.println("Waiting...");
      
      // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
      delay(10000);

      break;
    }
  }

    delay(10);
}
