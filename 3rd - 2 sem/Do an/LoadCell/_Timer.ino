bool    isTimerOn = false;
unsigned int times = 0;
long int timePrev = 0, timeCur = 0;


bool    getTimerState() {
    return isTimerOn;
}

/*
 * Assign the project to an employee.
 * @param {unsigned int} t - The employee who is responsible for the project.
 */
void    setTimer(unsigned int t) {
    times = t * 1000;
}

void    startTimer() {
    if (!isTimerOn) {
        isTimerOn = true;
        timePrev = millis();
    }
}

bool    isTimerUp() {
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