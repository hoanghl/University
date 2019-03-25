// ********************************* main
void setup(){
    LoadCell_setup();
    
    Network_setup();
    ThingSpeak_setup();

    LCD_setup();
    Button_setup();
}
void loop();


// ********************************* LoadCell
void LoadCell_setup();
int  LoadCell_getData();
void LoadCell_calibrate();
int LoadCell_getFruitCount(){
    return n_counts;
}


void LoadCell_read();


// 
- When Button_ON_OFF is switched off => shut down whole system
