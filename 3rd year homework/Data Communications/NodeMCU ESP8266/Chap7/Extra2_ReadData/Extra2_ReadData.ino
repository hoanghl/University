#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         D2          // Configurable, see typical pin layout above
#define SS_PIN          D4          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;


byte buff[34];
byte block = 0;  
byte len = 18;
byte maxnchar = 0;

bool VerifyBlock (byte block){
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return false;
    }
    return true;
}

bool ReadBlock   (byte block, byte * buff, byte * len){
    status = mfrc522.MIFARE_Read(block, buff, len);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("Reading failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return false;
    }
    return true;
}

void setup() {
    Serial.begin(9600);                                           // Initialize serial communications with the PC
    SPI.begin();                                                  // Init SPI bus
    mfrc522.PCD_Init();                                              // Init MFRC522 card
    Serial.println(F("Read personal data on a MIFARE PICC:"));    //shows in serial that it is ready to read
}


void loop() {
    // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

    //-------------------------------------------

    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    Serial.println(F("**Card Detected:**"));

    //-------------------------------------------

    mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card

    //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //uncomment this to see all blocks in hex

    //-------------------------------------------

    Serial.print(F("Full name: "));

    maxnchar = 30;
    block = 1;
    if (VerifyBlock(block) == false)                    return;
    if (ReadBlock(block, buff, &len) == false)        return;
    block = 2;
    if (VerifyBlock(block) == false)                    return;
    if (ReadBlock(block, &buff[16], &len) == false)   return;
    
    for (byte i = 0; i < maxnchar; i++)
        Serial.write(buff[i]);
    Serial.println();

    

    Serial.print(F("Age: "));
    
    maxnchar = 5;
    block = 4;
    if (VerifyBlock(block) == false)                    return;
    if (ReadBlock(block, buff, &len) == false)        return;

    for (byte i = 0; i < maxnchar; i++)
        Serial.write(buff[i]);
    Serial.println();

    
    
    Serial.print(F("stuID: "));
    
    maxnchar = 7;
    block = 5;
    if (VerifyBlock(block) == false)                    return;
    if (ReadBlock(block, buff, &len) == false)        return;

    for (byte i = 0; i < maxnchar; i++)
        Serial.write(buff[i]);
    Serial.println();
    
    
    
    Serial.print(F("Marital Status: "));
    
    maxnchar = 1;
    block = 6;
    if (VerifyBlock(block) == false)                    return;
    if (ReadBlock(block, buff, &len) == false)          return;
    
    String s;
    for (byte i = 0; i < maxnchar; i++)
        s += (char)buff[i];
    
    int maritalstatus = s.toInt();
    Serial.println(maritalstatus);
    switch (maritalstatus){
        case 0:{
            Serial.println("Never married");
            break;
        }
        case 1:{
            Serial.println("Married");
            break;
        }
        case 2:{
            Serial.println("Widowed");
            break;
        }
        case 3:{
            Serial.println("Divorced");
            break;
        }
    }

    //----------------------------------------

    Serial.println(F("\n**End Reading**\n"));

    delay(1000); //change value if you want to read cards faster

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
}
//*****************************************************************************************//
