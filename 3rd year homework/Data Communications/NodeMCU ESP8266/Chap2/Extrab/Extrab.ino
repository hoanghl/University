#define WAIT_1SHARP 0
#define WAIT_2SHARP 1
#define DOOR_OPEN   2
#define DOOR_CLOSE  3

short state;
char c;               // store read character
char c1, c2, c3, c4;   // store read characters like stack

void pushchartoStack(){
  c1 = c2;
  c2 = c3;
  c3 = c4;
  c4 = c;
}

boolean ispassvalid(){
  if ((c1 == 'O') &&
      (c2 == 'P') &&
      (c3 == 'E') &&
      (c4 == 'N'))
    return true;
  else
    return false;     
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  c = c1 = c2 = c3 = c4 = 0;
  state = WAIT_1SHARP;
}

void loop() {
  switch(state){
    case WAIT_1SHARP:{
      if (Serial.available() > 0){
        c1 = 0;       // this to ensure the input string "##" is invalid
        c = Serial.read();
        if (c == '#'){
          state = WAIT_2SHARP;
          Timer1_set(5);          
        }          
      }
      break;
    }

    case WAIT_2SHARP:{
      if (Timer1_tick() == true){
        state = WAIT_1SHARP;
        break;
      }

      if (Serial.available() > 0){
        c = Serial.read();        
        if (c == '#'){       // read second '#'
          if (ispassvalid())
            state = DOOR_OPEN;
          else
            state = DOOR_CLOSE;
        }
        else
          pushchartoStack();
      }
      break;
    }
    
    case DOOR_OPEN:{
      digitalWrite(LED_BUILTIN, LOW);
      state = WAIT_1SHARP;
      break;
    }
    case DOOR_CLOSE:{
      digitalWrite(LED_BUILTIN, HIGH);
      state = WAIT_1SHARP;
      break;
    }
  }

  delay(10);
}
