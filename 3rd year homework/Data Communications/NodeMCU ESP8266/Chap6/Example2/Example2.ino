 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server(80); // 80 is the port number

#include "WorldTime.h"

const char* ssid="Home_BVD";
const char* password="360486d4tdt";

String Website,data,XML,Javascript;

WorldTime Saigon;
String payload;

int dayUTC, monthUTC, yearUTC, hourUTC, minuteUTC, secondUTC;
bool isDayUTC = false;

// ****************************** routines ****************************
String arr[5];
short i = 0;
short pos = 0;

void proceduceDate(){  
  if (payload.indexOf("AM") == -1)
    isDayUTC = false;
  else
    isDayUTC = true;
   
  int i = 0;
  for (; i < payload.length() && (payload.charAt(i) < '0' || payload.charAt(i) > '9'); i++);

  payload = payload.substring(i);

   i = 0;
  while (pos != -1){
    pos = payload.indexOf(" ");
    arr[i] = payload.substring(0, pos);
    payload = payload.substring(pos + 1);
    i++;
  }
  dayUTC = arr[0].toInt();
  for (int i = 0; i < 12; i++){
    if (MONTH[(monthUTC = i)] == arr[1]){
      break;
    }
  }
  monthUTC++;
  yearUTC = arr[2].toInt();

  payload = arr[3];
  // export hour::min:sec
  i = pos = 0;
  while (pos != -1){
    pos = payload.indexOf(":");
    arr[i] = payload.substring(0, pos);
    payload = payload.substring(pos + 1);
    i++;
  }  
  hourUTC = arr[0].toInt();
  minuteUTC = arr[1].toInt();
  secondUTC = arr[2].toInt();
}

//**************************************************************************************



//**************************************************************************************
// *************************** WEB *****************************************************
//**************************************************************************************
void javascriptContent(){
    Javascript ="<SCRIPT>\n";
    Javascript+="var xmlHttp=createXmlHttpObject();\n";
    Javascript+="function createXmlHttpObject(){\n";
    Javascript+="if(window.XMLHttpRequest){\n";
    Javascript+="xmlHttp=new XMLHttpRequest();\n";
    Javascript+="}else{\n";
    Javascript+="xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\n";
    Javascript+="}\n";
    Javascript+="return xmlHttp;\n";
    Javascript+="}\n";
    Javascript+="\n";
    Javascript+="function response(){\n";
    Javascript+="xmlResponse=xmlHttp.responseXML;\n";
    Javascript+="xmldoc = xmlResponse.getElementsByTagName('data');\n";
    Javascript+="message = xmldoc[0].firstChild.nodeValue;\n";
    Javascript+="document.getElementById('div1').innerHTML=message;\n";
    Javascript+="}\n";

    Javascript+="function process(){\n";
    Javascript+="xmlHttp.open('PUT','xml',true);\n";
    Javascript+="xmlHttp.onreadystatechange=response;\n";
    Javascript+="xmlHttp.send(null);\n";
    Javascript+="setTimeout('process()',200);\n";
    Javascript+="}\n";
    
    Javascript+="</SCRIPT>\n";
    
  
  }

void WebsiteContent(){
    javascriptContent();
    
    Website="<html>\n";
    Website+="<body onload='process()'>";
    Website+="<div id='div1'>"+data+"</div></body></html>";
    Website+=Javascript;
    server.send(200,"text/html",Website);
  }
void XMLcontent(){

  XML ="<?xml version='1.0'?>";
  XML+="<data>";
  XML+=data;
  XML+="</data>";

  server.send(200,"text/xml",XML);
  
  }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED)delay(500);
  WiFi.mode(WIFI_STA);
  Serial.print(WiFi.localIP());
  server.on("/",WebsiteContent);
  server.on("/xml",XMLcontent);
  server.begin();

  HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin("http://www.iforce2d.net/test.php");  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code
 
       payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
 
    }
 
    http.end();   //Close connection

  Saigon.addDat("Saigon", 7, dayUTC, monthUTC, yearUTC, hourUTC, minuteUTC, secondUTC, isDayUTC);
  Saigon.updateDay();
}
int inc = 122;
long int timeCurrent, timePrev = 0;
void loop() {

  
  timeCurrent = millis();
  if (timeCurrent - timePrev >= 1000){
    timePrev = timeCurrent;
    data = Saigon.getStr();
    Serial.println(data);
    Saigon.incDay();
  }
  server.handleClient();
}
     
    
