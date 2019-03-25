#define WAITING 0
#define ON      1
#define OFF     2

short state;
char tmp;

void setup(){
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  state = WAITING;
  tmp = 0;
}
void loop(){
  switch (state){
    case WAITING:{      
      if (Serial.available()){
        char tmp = Serial.read();
        switch (tmp){
          case 'O':{
            state = ON;
            break;
          }
          case 'F':{
            state = OFF;
            break;
          }
        }
      }
      
      break;
    }

    case ON:{
      digitalWrite(LED_BUILTIN, LOW);
      state = WAITING;
      break;
    }
    case OFF:{
      digitalWrite(LED_BUILTIN, HIGH);
      state = WAITING;
      break;
    }
  }
  delay(10);
}
