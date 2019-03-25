#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 12;
const int LOADCELL_SCK_PIN = 14;

HX711 scale;

void setup() {
    Serial.begin(57600);
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

    //calibrate
    scale.set_scale();
    scale.tare();

    Serial.println("Please put the smartphone onto the sensor.");
    Serial.println("The process will start within 10 seconds");
    delay(10000);
    Serial.println("Start.");

    
    for (int i = 0; i < 15; i++) {
        float tmp = scale.get_units(10);
        scale.set_scale(tmp / 129);
    }
    Serial.print("Calibrating complete. The weight: ");
    Serial.print(scale.get_units(10));
}

void loop() {

    if (scale.is_ready()) {
        long reading = scale.get_units(5);
        Serial.print("HX711 reading: ");
        Serial.println(reading);
        Serial.print("and offset = "); Serial.println(scale.get_offset());
    } else {
        Serial.println("HX711 not found.");
    }

    delay(1000);
    
}
