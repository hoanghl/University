#include <LiquidCrystal_I2C.h>

#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

Serial.begin(115200);

//Use predefined PINS consts

Wire.begin(D2, D1);

lcd.begin();

lcd.home();


}
String s = "Hello, NodeMCU  ";
String tmp = "";
short i = 0;
long int timeCurrent = 0, timePrev = 0;
void loop(){
  timeCurrent = millis();
  if (timeCurrent - timePrev >= 500){
    timePrev = timeCurrent;
    if (i > 15){
      i = 0;
    }
    else {
      tmp = s.substring(i);
      for (short j = 0; j < i; j++)
        tmp += s.charAt(j);
      lcd.setCursor(0, 0);
      lcd.print(tmp);
      i++;
    }
  }
}
