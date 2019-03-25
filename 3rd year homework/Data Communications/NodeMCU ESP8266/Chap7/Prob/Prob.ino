#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN D4
#define RST_PIN D2

#define WAITING 0
#define VERIFY  1
#define LED     2

short state = 0;
byte CARD1[] = {0xE9, 0x77, 0xCA, 0x73};
long int timeCurrent = 0, timePrev = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class
void setup() {
   Serial.begin(9600);
   SPI.begin();       // Init SPI bus
   mfrc522.PCD_Init(); // Init MFRC522
   Serial.println("RFID reading UID");

   pinMode(LED_BUILTIN, OUTPUT);
   digitalWrite(LED_BUILTIN, HIGH);

   state = WAITING;
}
void loop() {
  switch (state){
    case WAITING:{
      if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
        state = VERIFY;
      break;
    }
    case VERIFY:{
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        if (mfrc522.uid.uidByte[i] != CARD1[i]){
          state = WAITING;
          Serial.println("Access Denied");
          return;
        }
      }
      Serial.println("Access Accepted");
      state = LED;
      digitalWrite(LED_BUILTIN, LOW);
      timePrev = millis();
      break;
    }
    case LED:{
      timeCurrent = millis();
      if (timeCurrent - timePrev >= 3000){
        
        digitalWrite(LED_BUILTIN, HIGH);        
        state = WAITING;
      }
      break;
    }
  }
}
