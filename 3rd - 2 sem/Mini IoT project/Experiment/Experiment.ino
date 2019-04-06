void setup() {
    Serial.begin(57600);
    
    int t = 65535;
    byte *p = (byte *)&t;

    Serial.println(*p);
    Serial.println(*(p+1));
    Serial.println(*(p + 2));
    Serial.println(*(p + 3));

}

void loop() {}
