#include    <EEPROM.h>

const int   EEPROMAddr = 0;

void eeWriteFloat(float val) {
    int pos = EEPROMAddr;    
    byte* p = (byte*) &val;

    EEPROM.write(pos, *p);
    EEPROM.write(pos + 1, *(p + 1));
    EEPROM.write(pos + 2, *(p + 2));
    EEPROM.write(pos + 3, *(p + 3));
    EEPROM.commit();
}

float eeGetFloat() {
  int pos = EEPROMAddr;
  float val = 0;
  byte* p = (byte*) &val;
  *p        = EEPROM.read(pos);
  *(p + 1)  = EEPROM.read(pos + 1);
  *(p + 2)  = EEPROM.read(pos + 2);
  *(p + 3)  = EEPROM.read(pos + 3);

  return val;
}