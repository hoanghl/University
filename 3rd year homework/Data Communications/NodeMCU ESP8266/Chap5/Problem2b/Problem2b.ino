#include <ESP8266WiFi.h>

#define READCHAR    0
#define SSIDPASS    1
#define CONNCT      2
#define THINGSPEAK  3

String apiKey = "DUANLWTJNUMVT7FW";    //  Enter your Write API key from ThingSpeak

char     ssid[20], password[20];
const char* server = "api.thingspeak.com";

WiFiClient client;

int fakeTimer = 1;
short state = SSIDPASS;
short read_level = 0;
char c = 0;
String s = "";

void setup(){
  Serial.begin(115200);
  delay(10);

  /*
  /////////// get SSID and password //////////
  String s;
  Serial.setTimeout(10000);
  Serial.print("Enter SSID = ");     s = Serial.readString(); Serial.println(s); s.toCharArray(ssid, s.length()); ssid[s.length()] = 0;
  Serial.flush();
  Serial.print("Enter password = "); s = Serial.readString(); Serial.println(s); s.toCharArray(password, s.length()); password[s.length()] = 0;
  

  ////////////////////////////////////////////
  
  
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  */
}
 
void loop(){

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
            s.toCharArray(ssid, s.length()+1);
            Serial.println(s);
              
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
