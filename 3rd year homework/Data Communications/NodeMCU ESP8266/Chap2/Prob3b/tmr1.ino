long tmr1;


void Timer1_set(unsigned int sec){
  tmr1 = sec * 100;
}

bool Timer1_tick(){
  if (tmr1 > 0){
    return (--tmr1) == 0;
  }
  else
    return false;
}
