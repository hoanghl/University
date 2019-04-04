/**
 *    This files contains functions used to communicate with EEPROM
 */ 

#include        <EEPROM.h>

/**
 *   The format of EEPROM is as follows:
 *      0th byte            : flag - check whether the LoadCell is calibrated
 *      1st -> 3rd byte     : contains calibrated value
 *      from 4th byte       : contains weighted data
 */

const int   EEPROM_isCalib      = 0;                     // position of isCalib
const int   EEPROM_CalibAddr    = 1;                     // first position of Calibration index
const int   EEPROM_DatAddr      = 4;                     // first position of Data

int         nDataInMem          = 0;                     // number of weighted numbers saved in EEPROM

//extern variables
extern int  newly_data;

/**
 *  Write calibrated index to EEPROM
 *  @params val - calibrated index
 */ 
void        eeWriteCalibrate(float val) {
    // set flag isCalib
    EEPROM.write(EEPROM_isCalib, 1);

    int pos = EEPROM_CalibAddr;        
    byte* p = (byte*) &val;

    EEPROM.write(pos, *p);
    EEPROM.write(pos + 1, *(p + 1));
    EEPROM.write(pos + 2, *(p + 2));
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

    return val;
}

bool        isCalibrated() {
    return EEPROM.read(EEPROM_isCalib) != 0;
}

/**
 *  Write int array containing weighted data into EEPROM
 *  @params values - int array
 *          data_size - no. elements in array
 */ 
void        SaveMem() {
    int pos = EEPROM_DatAddr + nDataInMem * 4;

    byte* p = (byte*) &newly_data;

    EEPROM.write(pos, *p);
    EEPROM.write(pos + 1, *(p + 1));
    EEPROM.write(pos + 2, *(p + 2));
    EEPROM.write(pos + 3, *(p + 3));
    EEPROM.commit();

    nDataInMem++;
}


/**
 *  Get number of weighted data stored in EEPROM
 *  @return int - number of weighted data
 */
int         getNSavedData() {
    return nDataInMem;
}

/**
 *  Get weighted values stored in EEPROM
 *  @return int array - if data available
 *          nullptr - otherwise
 */
int*        getSavedData() {
    if (nDataInMem == 0)
        return nullptr;

    int* retval = new int[nDataInMem];
    int pos = EEPROM_DatAddr;
    for (int i = 0; i < nDataInMem; i++) {
        byte* p = (byte*) &retval[i];
        
        *p          = EEPROM.read(pos);
        *(p + 1)    = EEPROM.read(pos + 1);
        *(p + 2)    = EEPROM.read(pos + 2);
        *(p + 3)    = EEPROM.read(pos + 3);

        pos += 4;
    }
    nDataInMem = 0;

    return retval;
}