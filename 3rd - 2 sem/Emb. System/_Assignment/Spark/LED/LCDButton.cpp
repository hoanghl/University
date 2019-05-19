#include    "LCDButton.hpp"

#include    <iostream>
#include    "oled/Edison_OLED.h"
#include    "gpio/gpio.h"
#include    <stdio.h>
#include    <unistd.h>

#define     PRESSED     LOW

using namespace std;

//**************** global variables ******************
edOLED  oled;

gpio BUTTON_A (49, INPUT);
gpio BUTTON_B (46, INPUT);


//**************** functions definition **************
void    init_LCDButton()
{
    oled.begin();
	oled.clear(PAGE);
	oled.display();
	oled.setFontType(0);
}


bool    isButAPressed()
{
    if (BUTTON_A.pinRead() == PRESSED)
    {
        usleep(10 * 1000);

        if (BUTTON_A.pinRead() == PRESSED)
            return true;
    }
    
    return false;
}


void    printLCD_1()
{

}
void    printLCD_2()
{

}
void    printLCD_3()
{
    
}