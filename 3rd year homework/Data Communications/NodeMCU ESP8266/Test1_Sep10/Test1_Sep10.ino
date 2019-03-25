#define LED_ON  1
#define LED_OFF 2
#define IDLING  3


unsigned long prevMillis = 0;        

const long T_ON = 3000, T_OFF = 5000;
unsigned int interval;

short state;

void setup() {
  Serial.begin(9600); 
  pinMode(LED_BUILTIN, OUTPUT);

  state = IDLING;
  interval = T_ON;
}

void loop() {
  switch (state){
    case IDLING:{
      unsigned long currentMillis = millis();
      if (currentMillis - prevMillis >= interval){
        prevMillis = currentMillis;
        if (interval == T_ON){
          state = LED_OFF;
          interval = T_OFF;
        } else {
          state = LED_ON;
          interval = T_ON;
        }
      }    
      break;
    }
     case LED_ON:{
      digitalWrite(LED_BUILTIN, LOW);
      state = IDLING;
      break;
     }
     case LED_OFF:{
      digitalWrite(LED_BUILTIN, HIGH);
      state = IDLING;
      break;
     }
  }
  delay(10);
}
