#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 12;
const int LOADCELL_SCK_PIN  = 14;

HX711 scale;

bool    scaleZero = false;

void LoadCell_setup() {
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale(eeGetFloat());                            // get offset saved in EEPROM
}

void LoadCell_calibrate() {
    if (!scaleZero) {
        scaleZero = true;

        // calculate offset as no load
        scale.set_scale();
        scale.tare();

        // require engineer to put already known weight load onto the load cell
        Serial.println("Please put the smartphone onto the sensor.");
        Serial.println("The process will start within 10 seconds");

        setTimer(10);
        startTimer();
        state_prev = state;
        state = St_Timer;
    }
    else
    {
        scaleZero = false;

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
        eeWriteFloat(scale.get_scale());

        // notify to screen
        Serial.print("Calibrating complete. The weight: ");
        Serial.println(scale.get_units(10));

        // change state
        substate = st_LoadCell_getData;
    }    
}

int  LoadCell_getData() {
    if (scale.is_ready()) {
        long reading = scale.get_units(5);
        Serial.print("HX711 reading: ");
        Serial.println(reading);
        Serial.print("and offset = "); Serial.println(scale.get_offset());
    } else {
        Serial.println("HX711 not found.");
    }

    state_prev = state;
    state = St_Timer;
    setTimer(1);
    startTimer();
}
