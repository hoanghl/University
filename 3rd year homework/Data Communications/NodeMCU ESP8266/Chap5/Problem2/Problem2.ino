/*
 * *************************************************************
 * *              IMPORTANT NOTES FOR LECTURER                 *
 * *************************************************************
 *  Dear Dr.Nhan, to run sucessfully my small program you need *
 *  following configuration for Arduino IDE:                   *
 *      In Serial Mornitor, you must set "New line", not       *
 *      "No line ending".                                      *
 *  And because I don't have any weather sensors to send       *
 *  pratical parameters to ThinkSpeak.com, I used random() to  *
 *  generate numbers.                                          *
 *                                                             *
 *  Thank you                                                  *
 */

#include <ESP8266WiFi.h>
 
String apiKey = "DUANLWTJNUMVT7FW";    //  Enter your Write API key from ThingSpeak

char     ssid[20], pass[20];
const char* server = "api.thingspeak.com";

WiFiClient client;


void setup(){
  Serial.begin(115200);
  delay(10);

  /////////// get SSID and password //////////
  String s;
  Serial.setTimeout(10000);
  Serial.print("Enter SSID = ");     s = Serial.readString(); Serial.println(s); s.toCharArray(ssid, s.length()); ssid[s.length()] = 0;
  Serial.flush();
  Serial.print("Enter password = "); s = Serial.readString(); Serial.println(s); s.toCharArray(pass, s.length()); pass[s.length()] = 0;
  

  ////////////////////////////////////////////
  
  
 
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}
 
void loop(){
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
}
