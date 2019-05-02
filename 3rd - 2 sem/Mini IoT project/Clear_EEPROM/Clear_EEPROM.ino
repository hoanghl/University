#include    <EEPROM.h>

void setup() {
    EEPROM.begin(512);
    
    EEPROM.write(0, 0);
    EEPROM.write(4, 6);
    EEPROM.write(5, 6);
    EEPROM.write(6, 0);
    EEPROM.write(7, 0);
    EEPROM.write(8, 0);
    EEPROM.write(9, 0);
    EEPROM.commit();
}

void loop() {
    
}
