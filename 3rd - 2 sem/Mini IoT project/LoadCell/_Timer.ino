
extern enum State   state;

bool    isTimerOn = false;
unsigned int times = 0;
long int timePrev = 0, timeCur = 0;


bool    getTimerState() {
    return isTimerOn;
}

void    setTimer(unsigned int t) {
    times = t * 1000;
}

void    setTimerms(unsigned int t) {
    times = t;
}
void    startTimer() {
    if (!isTimerOn) {
        isTimerOn = true;
        timePrev = millis();
    }
}

bool    isTimeOut() {
    if (millis() - timePrev >= times) 
    {
        isTimerOn = false;
        return true;
    }
    else
    {
        return false;
    }
}


/**
 *  Very important state, 
 *  an alternative solution for delay()
 */ 
void    Wait() {
    if (isTimeOut())
        state = St_ReadSensor;
    else
    {
        state = St_LCD_Button;
    }
    
} 