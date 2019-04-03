// there are 2 state machines concurrently existing. 
// The outer one loops among Big States
// the inner one loops inside each Big state


enum State {St_NULL, St_Calibrate, St_ConnectionCheck, St_ReadSensor, St_Wait, St_LCD_Button,
            St_Send,  St_SaveMem};
enum State      state, prev_state;

void setup() {
    Serial.begin(57600);

    // set up components
    LoadCell_setup();
    Connection_setup();
    LCD_Button_setup();

    // init state
    state       = St_Calibrate;
    prev_state  = St_NULL;
}


void loop() {
    switch (state) {
        case St_Calibrate: {
            Calibrate();
            break;
        }
        case St_ConnectionCheck: {
            ConnectionCheck();
            break;
        }
        case St_ReadSensor: {
            ReadSensor();
            break;
        }
        case St_Wait: {
            Wait();
            break;
        }
        case St_LCD_Button: {
            LCD_Button();
            break;
        }
        case St_Send: {
            Send();
            break;
        }
        case St_SaveMem: {
            SaveMem();
            break;
        }
    }

    delay(10);
}