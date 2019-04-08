#include    <EEPROM.h>

void setup() {
    EEPROM.begin(512);
    Serial.begin(57600);
    
    
}

void loop() {
    Serial.println(EEPROM.read(4));
    Serial.println(EEPROM.read(5));
    Serial.println( (EEPROM.read(5) - EEPROM.read(4)) / 4 );
    
    delay(100);
}
 
