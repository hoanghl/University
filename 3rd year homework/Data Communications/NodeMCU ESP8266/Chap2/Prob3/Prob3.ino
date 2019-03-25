
unsigned long  current, prev;
short n = 0;    //count read charaters

bool  counting = false;
void  startCounting(){
  n++;
  counting = true;
  prev = millis();
}

bool is5sElapsed(){
  current = millis();
  if (current - prev >= 5000){
    counting = false;
    return true;
  }    
  else return false;
}

void resetCounting(){
  n = 0;
  counting = false;
}

void setup(){
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop(){
  if (Serial.available()){
    char tmp = Serial.read();
    switch(tmp){
      case 'O':{
        if (counting){
          resetCounting();
          break;
        } else{
          startCounting();
        }
        break;
      }
      case 'N':{
        if (counting && !is5sElapsed() && n == 1){
          digitalWrite(LED_BUILTIN, LOW);
          resetCounting();
          break;
        } else {
          resetCounting();
          break;
        }        
      }
      case 'F':{
        if (n == 1 && !is5sElapsed()){
          n++;
          break;
        }
        else if (n == 2 && !is5sElapsed()){
          digitalWrite(LED_BUILTIN, HIGH);
        }
        resetCounting();
        break;
      }
      default:{
        resetCounting();        
      }
    }
     
      
  }
}
