#include    <EEPROM.h>

void setup() {
    EEPROM.begin(512);
    
    EEPROM.write(0, 0);
    EEPROM.write(4, 0);
    EEPROM.commit();
}

void loop() {
    
}
