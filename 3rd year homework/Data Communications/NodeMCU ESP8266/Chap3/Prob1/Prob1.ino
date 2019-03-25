
#include <SoftwareSerial.h>
//Pinout for NodeMCU
//#define RX_PIN    D3
//#define TX_PIN    D2  
//#define RESET_PIN D4

//Pinout for Arduino Uno R3
#define RX_PIN    10
#define TX_PIN    11  
#define RESET_PIN 2

SoftwareSerial SIM(RX_PIN, TX_PIN);

char * text   = "Lance Vance";
char * number = "+84901350308";

void setup(){
  Serial.begin(9600);
  while(!Serial);
  SIM.begin(9600);

  resetSIM();
  delay(5000);                    // time needed for the connection's stablization
  Serial.println("Reset done");

  GSMCommunicate();
}

void loop(){
  // do nothin
}

//***************************************************************

String _readSerial(){
  int _timeout=0;
  while  (!SIM.available() && _timeout < 12000) {
    delay(13);
    _timeout++;

  }
  if (SIM.available())
    return SIM.readString();
}

void  resetSIM(){
  pinMode(2, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);
  delay(10);
  digitalWrite(RESET_PIN, LOW);
  delay(1000);
  digitalWrite(RESET_PIN, HIGH);
  delay(10);
}

void GSMCommunicate(){
  SIM.print("AT+CMGF=1\r");                  Serial.println(_readSerial());  delay(1000);
  SIM.print("AT+CSCS=\"GSM\"\r");            Serial.println(_readSerial());  delay(1000);
  SIM.print("AT+CMGS=\""); 
  SIM.print(number); SIM.print("\"\r");                                      delay(10);
  SIM.print(text); SIM.print ("\r");         //Serial.println(_readSerial());
  SIM.print((char)26);                       Serial.println(_readSerial());  delay(100);

  Serial.println("Sent successfully");
}
