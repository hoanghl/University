#include <iostream>
#include "oled/Edison_OLED.h"
#include "gpio/gpio.h"
#include <stdio.h>
#include <unistd.h> // for usleep

#define		PRESSED		LOW

using namespace std;

// Function prototypes:
void setupOLED();

// Define an edOLED object:
edOLED oled;

// Pin definitions:
// All buttons have pull-up resistors on-board, so just declare
// them as regular INPUT's
gpio BUTTON_UP      (47, INPUT);
gpio BUTTON_DOWN    (44, INPUT);
gpio BUTTON_LEFT    (165, INPUT);
gpio BUTTON_RIGHT   (45, INPUT);
gpio BUTTON_SELECT  (48, INPUT);
gpio BUTTON_A       (49, INPUT);
gpio BUTTON_B       (46, INPUT);


// const float paddleWidth = LCDWIDTH/16.0;
// const float paddleHeight = LCDHEIGHT/3.0;
// const float halfPaddleWidth = paddleWidth/2.0;
// const float halfPaddleHeight = paddleHeight/2.0;

int main(int argc, char * argv[])
{
	setupOLED();

	while (1)
	{
		oled.rect(4, 4, 6, 10);
		oled.display();
		usleep(100);
		oled.clear(PAGE);
		oled.rect(4, 5, 6, 10);
		oled.display();
		usleep(100);
		oled.clear(PAGE);
		oled.rect(4, 6, 6, 10);
		oled.display();
		usleep(100);
		oled.clear(PAGE);
		oled.rect(4, 5, 6, 10);
		oled.display();
		usleep(100);
		oled.clear(PAGE);
		oled.rect(4, 4, 6, 10);
		oled.display();
		usleep(100);
		oled.clear(PAGE);
	}
}

void setupOLED()
{
	oled.begin();
	oled.clear(PAGE);
	oled.display();
	oled.setFontType(0);
}
