#define RESET     0
#define WAIT_NEXT 1
#define LED_ON    2
#define LED_OFF   3

short state;

unsigned short num;
char  c;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  num = 0;
  state = RESET;
  c = 0;
}

void loop() {
  switch(state){
    case RESET:{      
      num = 0;
      if (Serial.available()){
        c = Serial.read();
        switch(c){
          case 'O':{
            num++;
            state = WAIT_NEXT;
            Timer1_set(5);
            break;
          }
        }
      }
      break;
    }
    case WAIT_NEXT:{                            // wait for the the incomming second or third character      
      if (Timer1_tick() == true){              // tick the timer and check if it reaches 0
        state = RESET;
        break;
      }

      if (Serial.available()){
        c = Serial.read();
        switch(c){
          case 'N':{
            if (num == 1)                         // this is the second charater to form a word "ON"
              state = LED_ON;
            else
              state = RESET;
            break;
          }
          case 'F':{
            if (num == 2)
              state = LED_OFF;
            else
              num++;
            break;
          }
        }
      }

      break;
    }

    case LED_ON:{
      digitalWrite(LED_BUILTIN, LOW);
      state = RESET;
      break;
    }
    case LED_OFF:{
      digitalWrite(LED_BUILTIN, HIGH);
      state = RESET;
      break;
    }
  }
  
  
  delay(10);
}
