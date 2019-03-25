#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         D2          // Configurable, see typical pin layout above
#define SS_PIN          D4          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;


byte buff[34];
byte block;  
byte len;
byte maxnchar = 0;

bool VerifyBlock(byte block){
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return false;
    }
    return true;
}

bool WriteBlock(byte block, byte * buff){
    status = mfrc522.MIFARE_Write(block, buff, 16);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return false;
    }
    else {
        Serial.println(F("MIFARE_Write() success."));
        return true;
    }
}
//
//void printArr(byte * arr, byte bound){
//    for (byte i = 0; i < bound; bound++)
//      Serial.print((char)arr[i]);
//    Serial.println();
//}

void setup() {
  Serial.begin(9600);        // Initialize serial communications with the PC
  SPI.begin();               // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card
  Serial.println(F("Write personal data on a MIFARE PICC "));
}



void loop() {

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.

  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print(F("Card UID:"));    //Dump UID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.print(F(" PICC type: "));   // Dump PICC type
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  
  


  Serial.setTimeout(20000L) ;     // wait until 20 seconds for input from serial 
  
  /*
   *    Ask Name
   */
  maxnchar = 30;
  Serial.print(F("--- Type full name, ending with #: "));
  len = Serial.readBytesUntil('#', (char *) buff, maxnchar) ;   // read full name from serial
  //printArr(buff, maxnchar);
  for (byte i = len; i < maxnchar; i++) 
      buff[i] = ' ';                                            // pad with spaces
  

  block = 1;
  if (VerifyBlock(block) == false)              return;
  if (WriteBlock(block, buff) == false)         return;  
  block = 2;
  if (VerifyBlock(block) == false)              return;
  if (WriteBlock(block, &buff[16]) == false)    return;
  
  
  /*
   *    Ask Age
   */
  maxnchar = 5;
  Serial.print(F("--- Type age, ending with #: "));
  len = Serial.readBytesUntil('#', (char *) buff, maxnchar) ;    // read age from serial
  //printArr(buff, maxnchar);
  for (byte i = len; i < maxnchar; i++) 
      buff[i] = ' ';                                             // pad with spaces

  block = 4;
  if (VerifyBlock(block) == false)              return;
  if (WriteBlock(block, buff) == false)         return;
  
  
  /*
   *    Ask Student ID
   */
  maxnchar = 7;
  Serial.print(F("--- Type stuID, ending with #: "));
  len = Serial.readBytesUntil('#', (char *) buff, maxnchar) ;   // read studentID from serial
  //printArr(buff, maxnchar);
  for (byte i = len; i < maxnchar; i++) 
      buff[i] = ' ';                                            // pad with spaces

  block = 5;
  if (VerifyBlock(block) == false)              return;
  if (WriteBlock(block, buff) == false)         return;

  
  /*
   *    Ask Marital status
   */
  
  maxnchar = 1;
  Serial.print(F("--- Type marital status (Never 0, Married 1, Widowed 2, Divorced 3), ending with #: "));
  len = Serial.readBytesUntil('#', (char *) buff, maxnchar) ;   // read studentID from serial
  //printArr(buff, maxnchar);
  for (byte i = len; i < maxnchar; i++) 
      buff[i] = ' ';                                            // pad with spaces
      
  block = 6;
  if (VerifyBlock(block) == false)              return;
  if (WriteBlock(block, buff) == false)         return;


  
  
  
  
  
  Serial.println(" ");
  mfrc522.PICC_HaltA(); // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD

}
