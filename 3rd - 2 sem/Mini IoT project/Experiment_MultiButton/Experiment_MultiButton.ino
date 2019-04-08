

typedef enum {Bt_NoPress, Bt_StaSto, Bt_SpDown, Bt_SpUp, Bt_Reset, Bt_Calib, Bt_View} Button;
void setup() {


  Serial.begin(57600);
  
}

void loop() {
  switch (whichButPressed())

    {

        case Bt_StaSto:

            Serial.println("StaSto pressed");

            break;

        case Bt_SpUp:

            Serial.println("SpeUp pressed");

            break;

        case Bt_SpDown:

            Serial.println("SpeDo pressed");

            break;

        case Bt_Reset:

            Serial.println("Reset pressed");

            break;

        case Bt_View:

            Serial.println("View pressed");

            break;

        case Bt_Calib:

            Serial.println("Calib pressed");


            break;



        default:

            break;

    }
    
//     Serial.println(analogRead(A0));
  delay(10);
}

Button whichButPressed ()
{
    int tmp = analogRead(A0);

    if (tmp >= 300 && tmp <= 340)
        return Bt_StaSto;
    if (tmp >= 355 && tmp <= 400)
        return Bt_SpUp;;
    if (tmp >= 415 && tmp <= 490)
        return Bt_SpDown;
    if (tmp >= 570 && tmp <= 630)
        return Bt_Reset;
    if (tmp >= 840 && tmp <= 890)
        return Bt_View;
    if (tmp == 1024)
        return Bt_Calib;

    return Bt_NoPress;
}
