#ifndef DRAW_HPP_
#define DRAW_HPP_


#include <iostream>
#include "oled/Edison_OLED.h"
#include "gpio/gpio.h"
#include <stdio.h>
#include <unistd.h> // for usleep

#define		PRESSED		LOW

using namespace std;

extern edOLED  oled;

extern gpio    BUTTON_UP;
extern gpio    BUTTON_DOWN;
extern gpio    BUTTON_LEFT;
extern gpio    BUTTON_RIGHT;

enum    Direction {NON, LEFT, DOWN, RIGHT, UP};

// *********************** Functions *****************
void        setupOLED();
Direction   readButton();
void        drawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void        printScore(int score);
void        prize(const char * msg);
#endif