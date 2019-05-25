#include <Ticker.h>

Ticker sTick;

volatile int wd_count = 0;

void ISRWatchdog(){
  wd_count++;
  if (wd_count == 30){
    Serial.println();
    Serial.println("Watchdog bite, resetting....");
    ESP.reset();
  };
};
