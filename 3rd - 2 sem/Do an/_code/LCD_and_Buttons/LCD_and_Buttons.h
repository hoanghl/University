#ifndef LCDandButtons_h
#define LCDandButtons_h

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

class LCD_and_Buttons
{
  public:
    int Mbut_state = 0;
    int select = 1;
    bool StartStop = false;
    int Speed = 0;
    float CurrentWeight = 0;
    int count = 0;
    
    LCD_and_Buttons (int VMBut, int SpeedPin, int SDBut, int SUBut, int SSBut, int RBut);
    bool isWeightChange (float weight);
    bool isButPushed (int But);
    void Select ();
    void ViewMode1 ();
    void ViewMode2  ();
    void Toggle (bool data);
    void StartStop();
    void SpeedUp ();
    void SpeedDown ();
    void DataUpdate (float weight);
    void LCDSetUp ();
    void Reset ();
    //void Calib ();
  private:
    int _VMBut; //pin 13
    int _SpeedPin; 
    int _SDBut; 
    int _SUBut; 
    int _SSBut;
    LiquidCrystal_I2C lcd(0x27, 16, 2);
};

#endif
