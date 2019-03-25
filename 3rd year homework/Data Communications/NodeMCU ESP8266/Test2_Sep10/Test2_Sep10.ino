void setup(){
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop(){
  if (Serial.available()){
    char tmp = Serial.read();
    Serial.print("Node MCU received: ");
    Serial.println(tmp);
    switch (tmp){
      case 'O':{
        digitalWrite(LED_BUILTIN, LOW);
        break;        
      }
      case 'F':{
        digitalWrite(LED_BUILTIN, HIGH);
        break;
      }
    }
     
      
  }
}
