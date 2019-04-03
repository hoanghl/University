#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>


const char *ssid = "Home_BVD";  
const char *password = "360486d4tdt";
//Enter your wifi name and password here

WiFiClient client;

const char* ThingSpeakServer = "api.thingspeak.com";
const int ID = 584123;
//Enter your ThingSpeak Channel ID here

String writeAPIKey = "NBM8S01IAHD0GBJG";
//Enter your write API Key here

const int setTimeOut = 30*1000;
//Delay interval between sending data, 1000ms = 1s


bool ThingSpeakConnect(){
  if (client.connect(ThingSpeakServer,80))
    return true;
  else 
    return false;
}

bool WiFiInit(){

  long int t;
  
  WiFi.mode(WIFI_OFF);
  delay(1000);
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  
  t = millis();
  
  while (WiFi.status() != WL_CONNECTED) {
      if (millis() - t <= setTimeOut){
          delay(500);
          Serial.print(".");
      }
      else {
        Serial.print("Timed out, can't connect to Wifi\n");
        return false;
      }
  }
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      return true;
}

void SendThingSpeak(int Data){
  // if (Data > 100000)
  //   return;
  // client.printf("GET /update?api_key=%s&field3=%d",writeAPIKey.c_str(), Data);
  // Serial.printf("Sent data: %d\n", Data);


  // for testing only ----------- By Hoang Le, Mar 27 : 00:47 AM
    if (Data > 100000)
      return;

    if (client.connect(ThingSpeakServer,80)){
      String postStr = writeAPIKey;
      postStr +="&field3=";
      postStr += String(Data);
      postStr += "\r\n\r\n";
    
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);

      Serial.print("Sent to server: "); Serial.println(Data);
    }

    client.stop();

}
  //Put data read from sensor here 
  //the above code is just temporary random data for testing
