#include    "core.hpp"

enum State state = St_STOP;

void    run()
{
    while (1)
    {
        switch (state)
        {
        case St_STOP:
            
            break;
        case St_RUN:
            break;
        case St_CALCULATE:
            break;
        default:
            return;
        }
    }
    
}


// ********************************************** //

int     main()
{
    run();
}