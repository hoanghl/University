long tmr1;

void Timer1_set(unsigned int second){
  tmr1 = second * 100;
}

boolean Timer1_tick(){
  if (tmr1 > 0)
    return (--tmr1) == 0;
  else
    return false;
}
