#include <ESP8266WiFi.h>

#define CONNCT    0
#define WAIT      1
#define GET       2
#define RESPONSE  4
#define DISCONNCT 5


// Change your WiFi SSID and password 
const char* ssid     = "Home_BVD";
const char* password = "360486d4tdt";

 
int ledPin = LED_BUILTIN; // GPIO2 or you can change it to your own pin

WiFiServer server(80);
WiFiClient client;


String request = "";
char   c       = 0;
int value = LOW;
short timer = 0;
short state = 0;

void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  
       
}
 
void loop() {
  switch (state) {
    case CONNCT: {
      // Connect to WiFi network
      if (timer == 0){
        Serial.println();
        Serial.println();
        Serial.print("Connecting to ");
        Serial.println(ssid);
        WiFi.begin(ssid, password);
      }

     
      if (WiFi.status() != WL_CONNECTED) {
        if (++timer == 50){
          Serial.print(".");
          timer = 1;
        }
      }
      else {
        timer = 0;
        Serial.println("");
        Serial.println("WiFi connected");
       
        // Start the server
        server.begin();
        Serial.println("Server started");
       
        // Print the IP address
        Serial.print("Use this URL to connect: ");
        Serial.print("http://");
        Serial.print(WiFi.localIP());
        Serial.println("/");

        state = WAIT;
        
      }
      break;
    }

    case WAIT: {
      client = server.available();
          if (!client) {
            return;
          }
 
      // Wait until the client sends some data
      Serial.println("new client");
      if (client.available())
        state = GET;
      break;
    }

    case GET: {
      // Read the first line of the request
      String request = client.readStringUntil('\r');
      Serial.println(request);
      client.flush();
 
      // Match the request
     
      int value = LOW;
      if (request.indexOf("/LED=ON") != -1)  {
        digitalWrite(ledPin, LOW);
        value = LOW;
      }
      if (request.indexOf("/LED=OFF") != -1)  {
        digitalWrite(ledPin, HIGH);
        value = HIGH;
      }

      state = RESPONSE;
      break;
    }

    case RESPONSE: {
      // Return the response in HTML
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println(""); //  do not forget this one
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
     
      client.print("Led pin is now: ");
     
      if(value == HIGH) {
        client.print("On");
      } else {
        client.print("Off");
      }
      client.println("<br><br>");
      client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
      client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");  
      client.println("</html>");

      state = DISCONNCT;
      break;
    }

    case DISCONNCT: {
      Serial.println("Client disonnected");
      Serial.println("");
      state = WAIT;
      break;
    } 
  }
}
