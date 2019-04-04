//Nhớ cài thư viện LiquidCrystal_I2C.h vô arduino thì mới chạy đc nha
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

/*
 *MBut là địa chỉ chân của nút ViewMode
 *SSBut là địa chỉ chân nút Start/Stop
 *SDBut và SUBut là địa chỉ chân nút giảm tốc và tăng tốc băng chuyền 
 *RBut là địa chỉ chân nút Reset
 *CBut là địa chỉ chân nút calibate
 *SpeedPin là địa chỉ của chân điều khiển tốc độ băng chuyền
 */
int VBut        = D0;
int SSBut       = D4;
int SDBut       = D7;
int SUBut       = D8;
int RBut        = D9;
int CBut        = D10;
int SpeedPin    = D3;

int pre = LOW;
int val = LOW;

int StartStopVal    = 1;                        // Start/stop value có giá trị 1(stop) hoặc 2(start)
int ViewMode        = 0;                        // Có 2 loại ViewMode (1 và 2)
int Speed           = 0;                        // Speed Level

int weight = 0;                                 // Biến Weight lưu khối lượng của trái vừa được cân

void LCD_Button()
{
    if (isButtonPressed (SSBut))                //Nếu nút Start/Stop ở chân SSBut được tích cực bậc cao 
        StartStop();                            //Thì gọi hàm StartStop

    if (isButtonPressed (SDBut))                //Nếu nút SpeedDown ở chân SDBut được tích cực bậc cao 
        SpeedDown ();                           //Thì tốc độ được giảm đi 64 (Lưu ý: NodeMCU hỗ trợ chân PCM mô phỏng tín hiệu analog theo 256 
                                                //trạng thái từ 0 - 255, nên dãy tốc độ được chia thành 4 cấp, mỗi cấp cách nhau 64 trạng thái, 
                                                //mỗi lần gọi hàm SpeedDown(), tốc độ sẽ giảm đi 64 trạng thái)

    if (isButtonPressed (SUBut))                //Lần này là SpeedUp, tương tự như SpeedDown
        SpeedUp ();

    if (isButtonPressed (VBut))                 //Nếu nút ViewMode ở chân MBut được tích cực bậc cao 
        processView (ViewMode);                 //Thì LCD sẽ chuyển giữa 2 chế độ xem: xem cân nặng từng trái và xem số trái cân được

    if (isButtonPressed (RBut))                 //Nếu nút Reset ở chân RBut được tích cực bậc cao 
        Reset ();                               //Thì reset count về 0

    if (isButtonPressed (CBut))                 //Nếu nút Calib ở chân CBut được tích cực bậc cao 
    {
        lcd.setCursor(2, 0);
        lcd.print("Calibrating");
        lcd.setCursor(6, 0);
        lcd.print("...");
        Calibrate();                            //Thì gọi hàm calib
        lcd.setCursor(2, 0);            
        lcd.print("Calibration");
        lcd.setCursor(4, 1);
        lcd.print("Complete");
    }
}

// Setup ban đầu cho LCD, xuất ra màn hình "Welcome!" 
// Lưu ý địa chỉ bus ic là 0x27 nếu thay đổi địa chỉ ic thì vào LCD_and_Buttons.h sửa lại
void LCD_Button_setup ()
{
    pinMode (D10, OUTPUT);
    pinMode (D9, OUTPUT);
    pinMode (D8, OUTPUT);
    pinMode (D7, OUTPUT);
    pinMode (D4, OUTPUT);
    pinMode (D3, INPUT);
    pinMode (LED_BUILTIN, OUTPUT);
    
    lcd.begin(16,2);
    lcd.init();
    lcd.backlight();

    lcd.setCursor(2, 0);            
    lcd.print("SMART_SCALE");
    lcd.setCursor(4, 1);            
    lcd.print("WELCOME!");
}

void ViewMode1 ()
{
    lcd.begin(16,2);
    lcd.setCursor(0,0); 
    lcd.print("Weight:");
    lcd.setCursor(8,0); 
    lcd.print(weight);
    lcd.setCursor(12,0); 
    lcd.print("Kg");
}

void ViewMode2 ()
{
    lcd.begin(16,2);
    lcd.setCursor(0,0); 
    lcd.print("Count:");
    lcd.setCursor(7,0); 
    lcd.print(count);
    lcd.setCursor(11,0); 
    lcd.print("Unit");
}

void choice (int a)
{
    if (a == 1)
        ViewMode1 ();
    else
        ViewMode2 ();
}

bool isButtonPressed (int but)
{
    pre = val;
    val = digitalRead (but);
    
    if (val == HIGH && pre == LOW)
    { 
        return true;
    }
    else
        return false;
}

void processView (int &a)
{
    if (a == 2)
    {
        a = 0;
    }
    a++;
    choice (a);
}

void printSpeed (int Speed)
{
    if (Speed == 0)
    {
        lcd.begin(16,2);
        lcd.setCursor(4,0); 
        lcd.print("STOPPED");
        lcd.setCursor(3,1); 
        lcd.print('|');
        lcd.setCursor(7,1); 
        lcd.print('|');
        lcd.setCursor(11,1); 
        lcd.print('|');
        lcd.setCursor(15,1); 
        lcd.print('|');
    }
    else if (Speed == 63)
    {
        lcd.begin(16,2);
        lcd.setCursor(3,0); 
        lcd.print("SPEED: 1/4");
        lcd.setCursor(0,1); 
        lcd.print("####");
        lcd.setCursor(3,1); 
        lcd.print('|');
        lcd.setCursor(7,1); 
        lcd.print('|');
        lcd.setCursor(11,1); 
        lcd.print('|');
        lcd.setCursor(15,1); 
        lcd.print('|');
    }
    else if (Speed == 127)
    {
        lcd.begin(16,2);
        lcd.setCursor(3,0); 
        lcd.print("SPEED: 2/4");
        lcd.setCursor(0,1); 
        lcd.print("########");
        lcd.setCursor(3,1); 
        lcd.print('|');
        lcd.setCursor(7,1); 
        lcd.print('|');
        lcd.setCursor(11,1); 
        lcd.print('|');
        lcd.setCursor(15,1); 
        lcd.print('|');
    }
    else if (Speed == 191)
    {
        lcd.begin(16,2);
        lcd.setCursor(3,0); 
        lcd.print("SPEED: 3/4");
        lcd.setCursor(0,1); 
        lcd.print("############");
        lcd.setCursor(3,1); 
        lcd.print('|');
        lcd.setCursor(7,1); 
        lcd.print('|');
        lcd.setCursor(11,1); 
        lcd.print('|');
        lcd.setCursor(15,1); 
        lcd.print('|');
    }
    else if (Speed == 255)
    {
        lcd.begin(16,2);
        lcd.setCursor(3,0); 
        lcd.print("SPEED: 4/4");
        lcd.setCursor(0,1); 
        lcd.print("################");
        lcd.setCursor(3,1); 
        lcd.print('|');
        lcd.setCursor(7,1); 
        lcd.print('|');
        lcd.setCursor(11,1); 
        lcd.print('|');
        lcd.setCursor(15,1); 
        lcd.print('|');
    }
    else
    {
        lcd.begin(16,2);
        lcd.setCursor(1,0); 
        lcd.print("Engine Has Not");
        lcd.setCursor(2,1); 
        lcd.print("Started Yet!");
    }
    
}

void StartStop ()
{
    if (StartStopVal == 2)
        StartStopVal = 0;
    StartStopVal ++;
    
    if (StartStopVal == 1)
    {
        Speed = 0;
    }
    else
    {
        Speed = 63;
    }
    printSpeed (Speed);
    analogWrite(SpeedPin, Speed);
}

void SpeedUp ()
{
    if (Speed == 255)
    {
        lcd.begin(16,2);
        lcd.setCursor(1,0); 
        lcd.print("Top-SPEED: 4/4");
        lcd.setCursor(0,1); 
        lcd.print("################");
        lcd.setCursor(3,1); 
        lcd.print('|');
        lcd.setCursor(7,1); 
        lcd.print('|');
        lcd.setCursor(11,1); 
        lcd.print('|');
        lcd.setCursor(15,1); 
        lcd.print('|');
    }
    else
    {
        Speed += 64;
        printSpeed (Speed);        
        analogWrite(SpeedPin, Speed);
    }
}

void SpeedDown ()
{ 
    if (Speed == 63)
    {
        lcd.begin(16,2);
        lcd.setCursor(1,0); 
        lcd.print("Min-SPEED: 1/4");
        lcd.setCursor(0,1); 
        lcd.print("####");
        lcd.setCursor(3,1); 
        lcd.print('|');
        lcd.setCursor(7,1); 
        lcd.print('|');
        lcd.setCursor(11,1); 
        lcd.print('|');
        lcd.setCursor(15,1); 
        lcd.print('|');
    }
    else
    { 
        Speed -= 64;
        printSpeed (Speed);
        analogWrite(SpeedPin, Speed);
    }
}

void Reset ()
{
    count = 0;
    lcd.begin(16,2);
    lcd.setCursor(6,0); 
    lcd.print("Reset");
    lcd.setCursor(0,1); 
    lcd.print("Count:");
    lcd.setCursor(7,1); 
    lcd.print(count);
    lcd.setCursor(11,1); 
    lcd.print("Unit");
}
