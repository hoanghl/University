#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>


const char *ssid             = "Home_BVD";    
const char *password         = "360486d4tdt";

const char *ThingSpeakServer = "api.thingspeak.com";
const int  ID                = 584123;                            // ThingSpeak Channel ID
String     writeAPIKey       = "NBM8S01IAHD0GBJG";                // ThingSpeak API


WiFiClient client;


// extern variable
extern int  newly_data;



/**
 *  Send to ThingSpeak: send newly weighted data and the ones stored in EEPROM if exist
 */ 
void Send(){
    

    client.stop();
}


void    Connection_setup() {
    WiFi.begin(ssid, password);
    WiFi.mode(WIFI_OFF);
}

void    ConnectionCheck() {
    if (prev_state == St_NULL)
        state = St_ReadSensor;
    else { // prev_state == St_LCDButton
        if (!(WiFi.status() == WL_CONNECTED && client.connect(ThingSpeakServer,80)))
            state = St_SaveMem;        
        else {
            // connection establised successfully
            Serial.println(" * WiFi connection:")
            Serial.print(ssid);
            Serial.print("  --  ");
            Serial.println(WiFi.localIP());
            Serial.println(" * ThingSpeak connection: Established")
        
            state = St_Send;
        }
    }
}


// ******************** private functions *********************
/**
 * 
 */
void    sendToThingSpeak(int data) {
    String postStr = writeAPIKey;
    postStr +="&field3=";
    postStr += String(data);
    postStr += "\r\n\r\n";
        
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);


    Serial.print("Sent to server: "); Serial.println(data);
}