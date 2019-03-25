#define ledPin LED_BUILTIN
void setup() {
  pinMode(ledPin, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}
void loop(){
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(3000);
}
