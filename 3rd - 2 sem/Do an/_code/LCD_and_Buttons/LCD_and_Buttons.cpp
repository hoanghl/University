#include <Arduino.h>
#include <LCD_and_Buttons.h>

LCD_and_Buttons :: LCD_and_Buttons (int MBut, int SpeedPin, int SDBut, int SUBut, int SSBut, int RBut)
{
  _MBut = MBut;
  _SDBut = SDBut;
  _SUBut = SUBut;
  _SSBut = SSBut;
  _SpeedPin = Speedpin;
  _Rbut = RBut;
}

bool LCD_and_Buttons :: isWeightChange (float weight)
{
  if (weight != CurrentWeight)
    return true;
  else
    return false;
}

bool LCD_and_Buttons :: isButPushed (int But)
{
  if (digitalRead(But == 1))
    return true;
  else
    return false;
}

void LCD_and_Buttons :: Select ()
{
  if(sel==2)
    sel=1;
  else
    sel++; 
  
  switch(sel)
  {
    case 1 :
      ViewMode1();
      break;

    case 2 :
      ViewMode2();
      break;
  }
}

void LCD_and_Buttons :: ViewMode1 ()
{
  lcd.setCursor(1,0); 
  lcd.print("Weight");
  lcd.setCursor(1,1); 
  lcd.print(CurrentWeight);
}

void LCD_and_Buttons :: ViewMode2 ()
{
  lcd.setCursor(1,0); 
  lcd.print("Count");
  lcd.setCursor(1,1); 
  lcd.print(count);
}

void LCD_and_Buttons :: Toggle (bool data)
{
  data = !data;
}

void LCD_and_Buttons :: StartStop()
{
  Toggle(StartStop);

  switch (StartStop)
  {
    case 0: 
    Speed = 0;
    break;
  
    case 1:
    Speed = 63;
    break;
  };
  analogWrite(SpeedPin, Speed);
}

void LCD_and_Buttons :: SpeedUp ()
{
  if (Speed == 255)
    Speed = 63;
  else 
    Speed += 64;
      
  analogWrite(SpeedPin, Speed);
}

void LCD_and_Buttons :: SpeedDown ()
{ 
  if (Speed == 0)
    Speed = 255;
  else 
    Speed -= 64;
  
  analogWrite(SpeedPin, Speed);
}

void LCD_and_Buttons :: DataUpdate (float weight)
{
  this -> CurrentWeight = weight;
  this -> count ++;
}

void LCD_and_Buttons :: LCDSetUp ()
{
  Serial.begin(115200);

  Wire.begin(D2, D1);
  
  lcd.begin();
  lcd.home();
  
  lcd.setCursor(4, 1); 
  lcd.print("Welcome!");
}

void LCD_annd_Buttons :: Reset ()
{
  this -> count = 0;
}
