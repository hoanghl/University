#include    "core.hpp"

#include    <iostream>
#include    <unistd.h>
#include    "9DOF/_9DOF.hpp"

using namespace std;

#define     TIME        0.01
#define     TIME_ELAPSE 100


int usleep_time = (int)(TIME * 1000000);

enum State state = St_STOP;

int time_elapse = 0;

void    init()
{
    init_9DOF();
}


void    run()
{
    while (1)
    {
        switch (state)
        {
        case St_STOP:
            cout << "*** onStop ***\n";
            state = St_RUN;
            break;
        case St_RUN:
            cout << "*** onRun ***\n";
            capture();
            cout << "time_elapse: " << time_elapse << "\n";
            if (time_elapse++ == TIME_ELAPSE)
            {
                state = St_CALCULATE;
            }
            cout << "Finish onRun\n";            
            break;
        case St_CALCULATE:
            cout << "*** onCalculate ***\n";
            
            cout << "Step: " << calculate() << "\n";
            
            state = St_EXIT;
            break;
        case St_EXIT:
            return;

        }

        usleep(usleep_time);
    }
    
}


// ********************************************** //

int     main()
{
    init();
    run();

    return 0;
}