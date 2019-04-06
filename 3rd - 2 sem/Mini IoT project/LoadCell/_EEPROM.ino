/**
 *    This files contains functions used to communicate with EEPROM
 */ 

#include        <EEPROM.h>

/**
 *   The format of EEPROM is as follows:
 *      0th -> 3rd byte     : contains calibrated value
 *      4th byte            : no. data saved in EEPROM
 *      from 5th byte       : contains weighted data
 */

const int   EEPROM_CalibAddr    = 0;                     // first position of Calibration index
const int   EEPROM_NDat         = 4;                     // byte containing no. data saved in EEPROM
const int   EEPROM_DatAddr      = 5;                     // first position of Data


//extern variables
extern int  newly_data;

void        EEPROM_setup() {
    EEPROM.begin(400);

}
/**
 *  Write calibrated index to EEPROM
 *  @params val - calibrated index
 */ 
void        eeWriteCalibrate(float val) {
    int pos = EEPROM_CalibAddr;        
    byte* p = (byte*) &val;

    EEPROM.write(pos, *p);
    EEPROM.write(pos + 1, *(p + 1));
    EEPROM.write(pos + 2, *(p + 2));
    EEPROM.write(pos + 3, *(p + 3));
    EEPROM.commit();
}

/**
 *  Get calibrated index saved in EEPROM
 *  @return int - calibrated index
 */
float       eeGetCalibrate() {
    int pos = EEPROM_CalibAddr;
    float val = 0;
    byte* p = (byte*) &val;
    *p          = EEPROM.read(pos);
    *(p + 1)    = EEPROM.read(pos + 1);
    *(p + 2)    = EEPROM.read(pos + 2);
    *(p + 3)    = EEPROM.read(pos + 3);

    return val;
}

/**
 *  Check whether the LoadCel has been calibrated by checking the 0th byte
 *  Usually the scale value is negative number => 0th is always different from 0
 */ 
bool        isCalibrated() {
    return EEPROM.read(EEPROM_CalibAddr) != 0;
}


/**
 *  Check whether weighted data are stored in EEPROM by checking 4th byte
 */ 
bool        isDataInEEPR() {
    return EEPROM.read(EEPROM_NDat) != 0;
}


/**
 *  Write int array containing weighted data into EEPROM
 *  @params values - int array
 *          data_size - no. elements in array
 */ 
void        SaveMem() {
    // update no. data
    byte nDataInMem = getNSavedData() + 1;
    EEPROM.write(EEPROM_NDat, nDataInMem);

    // write newly data from LoadCell
    int pos = EEPROM_DatAddr + (nDataInMem - 1) * 4;          // calculate position of byte to write

    byte* p = (byte*) &newly_data;

    EEPROM.write(pos, *p);
    EEPROM.write(pos + 1, *(p + 1));
    EEPROM.write(pos + 2, *(p + 2));
    EEPROM.write(pos + 3, *(p + 3));
    EEPROM.commit();

    Serial.print("nDataInMem in savemem: "); Serial.println(nDataInMem);

    state = St_LCD_Button;
    setTimer(5);
    startTimer();
}


/**
 *  Get number of weighted data stored in EEPROM
 *  @return int - number of weighted data
 */
byte         getNSavedData() {
    return EEPROM.read(EEPROM_NDat);
}
/**
 *  Get weighted values stored in EEPROM
 *  @return -1: no data in EEPROM
 *          positive number: number from EEPROM
 */
int        getSavedData() {
    byte nDataInMem = getNSavedData();
    if (nDataInMem == 0)
        return -1;
    EEPROM.write(EEPROM_NDat, nDataInMem - 1);
    EEPROM.commit();

    Serial.print("nDataInMem in getSaved: "); Serial.println(getNSavedData());

    

    int pos = EEPROM_DatAddr + (nDataInMem - 1) * 4;
    int tmp = 0;
    byte *p = (byte *) &tmp;
    *p          = EEPROM.read(pos);
    *(p + 1)    = EEPROM.read(pos + 1);
    *(p + 2)    = EEPROM.read(pos + 2);
    *(p + 3)    = EEPROM.read(pos + 3);

    return tmp;
}