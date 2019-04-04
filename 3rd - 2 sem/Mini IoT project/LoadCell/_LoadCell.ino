#include "HX711.h"

#include <math.h>

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 12;
const int LOADCELL_SCK_PIN  = 14;

HX711 scale;

extern enum State   state, prev_state, p_prev_State;
extern int          newly_data;

// private variables
enum SubState {st_calib_noload, st_calib, st_calib_wait} sub_state = st_calib_noload;


void    LoadCell_setup() {
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale(eeGetCalibrate());                            // get offset saved in EEPROM
}


/**
 *  Calibrate LoadCell
 */
void    Calibrate() {
    if (prev_state == St_LCD_Button || !isCalibrated()) {
        switch (sub_state) {
            case st_calib_noload: {
                // calculate offset as no load
                scale.set_scale();
                scale.tare();

                // require engineer to put already known weight load onto the load cell
                Serial.println("Please put the smartphone onto the sensor.");
                Serial.println("The process will start within 10 seconds");

                sub_state = st_wait;
                break;
            }
            case st_wait: {
                if (isTimeOut()) {
                    sub_state = st_calib;
                }
                break;
            }
            case st_calib: {
                Serial.println("Start.");
                Serial.print("Please type the weight of the object: ");
                Serial.setTimeout(5000);
                int weight = Serial.readStringUntil('\n').toInt();
                Serial.println(weight);

                for (int i = 0; i < 15; i++) {
                    float tmp = scale.get_units(10);
                    scale.set_scale(tmp / weight);
                }

                // write to EEPROM
                eeWriteCalibrate(scale.get_scale());

                // notify onto screen
                Serial.print("Calibrating complete. The weight: ");
                Serial.println(scale.get_units(5));


                // modify program flow
                if (prev_state == St_LCD_Button) {
                    if (p_prev_State == St_Wait) {
                        state = St_Wait;
                    }
                    else {  // p_prev_State == St_ReadSensor
                        prev_state = state;
                        state = St_ConnectionCheck;
                    }
                }
                else {  // prev_state == St_NULL
                    prev_state = state;
                    state = St_ConnectionCheck;
                }
                break;
            }
        }
    }
    else {  // prev_state == St_NULL && isCalib()
        scale.set_scale(eeGetCalibrate());
    }
}

/**
 *  Read data from LoadCell
 */ 
void    ReadSensor() {
    long reading;
    if (scale.is_ready()) {
        reading = scale.get_units(5);
        Serial.print("HX711 reading: ");
        Serial.println((int)reading);
    } else {
        Serial.println("HX711 not found.");
    }

    newly_data = (int)reading;

    state_prev = state;
    state = St_LCD_Button;
}
