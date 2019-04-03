// there are 2 state machines concurrently existing. 
// The outer one loops among Big States
// the inner one loops inside each Big state

enum BigState {St_Timer, St_LoadCell};
enum smallState {st_LoadCell_calibrate, st_LoadCell_getData};

enum BigState   state, state_prev;
enum smallState substate;



void setup() {
    Serial.begin(57600);


    // setup components
    LoadCell_setup();
    LCD_Button_setup();


    // init state
    state = St_LoadCell;
    substate = st_LoadCell_calibrate;
}

void loop() {
    switch (state)
    {
        case St_LoadCell:
            switch (substate)
            {
                case st_LoadCell_calibrate:
                    LoadCell_calibrate();
                    break;
                case st_LoadCell_getData:
                    LoadCell_getData();
                    break;
            }
            break;
        case St_Timer:
            State_Timer();
            break;

        default:
            break;
    }
    delay(10);    
}

void State_Timer() {
    if (isTimerUp()) {
        state = state_prev;
    }
}