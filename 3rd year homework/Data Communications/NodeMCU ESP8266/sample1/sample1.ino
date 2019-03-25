String s;
void setup() {
  Serial.begin(9600);  
  Serial.setTimeout(2000);

}

void loop() {
  char x;
  if (Serial.available()){
    s = Serial.readString();
    Serial.flush();
    Serial.println(s);
  }

}
