
#include <SoftwareSerial.h>

#define RX_PIN    D3
#define TX_PIN    D2  
#define RESET_PIN D4


//#define RX_PIN    10
//#define TX_PIN    11  
//#define RESET_PIN 2

#define RESET       0
#define CARRIERCONN 1
#define SMS         2
#define SMSVERIFY   3
#define WAITING     4
#define LOOP        5

short state = 0;
int   _timeout=0;
short command = 0;    // control which command is going to be executed

SoftwareSerial SIM(RX_PIN, TX_PIN);

char * text   = "Auntie Paulet";
char * number = "+84901350308";

String response = "";

void setup(){
  Serial.begin(9600);
  SIM.begin(9600);
}

void loop(){
  delay(3000);
  switch (state){
    case RESET:{
      resetSIM();
      command = 0;
      _timeout=0;
            
      Serial.println("Reset done");
      state = CARRIERCONN;
      
      break;
    }

    case CARRIERCONN:{
      SIM.print("AT+CSQ\r");
      state = WAITING;
      break;
    }


    
    case SMS:{
      GSMCommunicate();
      break;
    }
    case SMSVERIFY:{
      GSMResponseVerify();
      break;
    }
    case WAITING:{
      _readSerial();
      break;
    }
    case LOOP:{
      break;
    }
  }
}

//***************************************************************


void _readSerial(){
  delay(1000);
  if (_timeout == 501){
    state = RESET;
  }
  else {
    if (SIM.available()){
      Serial.println("Available");
      response = SIM.readString();
      Serial.print("Begin: '"); Serial.print(response); Serial.println("'");
      state = SMSVERIFY;
    }
    else{
      _timeout++;
    }
  }
}

void  resetSIM(){
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);
  delay(10);
  digitalWrite(RESET_PIN, LOW);
  delay(1000);
  digitalWrite(RESET_PIN, HIGH);
  delay(10);
}

void GSMCommunicate(){
  Serial.print("Commu state: "); Serial.println(command);
  switch(command){
    case 1:{
      SIM.print("AT+CMGF=1\r");
      break;
    }
    case 2:{
      SIM.print("AT+CSCS=\"GSM\"\r");
      Serial.println("SEND 2");
      break;
    }
    case 3:{
      SIM.print("AT+CMGS=\""); 
      SIM.print(number); SIM.print("\"\r");                                      delay(10);
      SIM.print(text); SIM.print ("\r");        
      SIM.print((char)26);                         delay(1000);
      break;
    }
    default:{
      state = LOOP;
    }
  };

  state = WAITING;
}
void GSMResponseVerify(){
  Serial.print("Verify state: "); Serial.println(command);
  switch(command){
    case 0:{
      if (response.indexOf("+CSQ: 0,0") == -1){
        state = SMS;
        command++;
      }
      else
        state= CARRIERCONN;
      break;
    }
    case 1:{
      if(response.indexOf("OK") != -1){
        command++;
        state = SMS;
      }
      else{
        state = RESET;
      }
      break;
    }
    case 2:{
      if(response.indexOf("OK") != -1){
        command++;
        state = SMS;
      }
      else{
        state = RESET;
      }
      break;      
    }
    case 3:{
      if(response.indexOf("+CMGS: ") != -1){
        command++;
        state = SMS;
      }
      else{
        state = RESET;
      }
      break; 
    }
  }
  response="";
}
