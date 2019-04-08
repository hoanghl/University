// there are 2 state machines concurrently existing. 
// The outer one loops among Big States
// the inner one loops inside each Big state

#include "General_Resources.hpp"
           

void setup() {
    Serial.begin(57600);

    Serial.println("Start setting up");
    // set up components
    EEPROM_setup();
    LoadCell_setup();
    Serial.println("LoadCell set up done");
    Connection_setup();
    Serial.println("Connection set up done");    
    LCD_Button_setup();
    Serial.println("all done");

    // init state
    state       = St_Calibrate;
    sub_state   = st_calib_noload;
}


void loop() {
    switch (state) {
        case St_Greeting: {
            Greeting();
            break;
        }
        case St_Calibrate: {
            //Serial.println("on St_Calibrate");
            Calibrate();
            break;
        }
        case St_ConnectionCheck: {
            //Serial.println("on St_ConCheck");
            ConnectionCheck();
            break;
        }
        case St_ReadSensor: {
            // Serial.println("on ReadSensor");
            ReadSensor();
            break;
        }
        case St_Wait: {
            // Serial.println("on Wait");
            Wait();
            break;
        }
        case St_LCD_Button: {
            // Serial.println("on LCD");
            LCD_Button();
            break;
        }
        case St_Send: {
            // Serial.println("on Send");
            Send();
            break;
        }
        case St_SaveMem: {
            // Serial.println("on SaveMem");
            SaveMem();
            break;
        }
    }

    delay(10);
}