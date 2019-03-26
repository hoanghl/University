//Nhớ copy thư viện LiquidCrustal_I2C.h, và LCD_and_Buttons.h vô arduino thì mới chạy đc nha
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <LCD_and_Buttons.h>

/*
 *MBut là địa chỉ chân của nút ViewMode
 *SpeedPin là địa chỉ của chân điều khiển tốc độ băng chuyền
 *SDBut và SUBut là địa chỉ chân nút giảm tốc và tăng tốc băng chuyền 
 *SSBut là địa chỉ chân nút Start/Stop
 *RBut là địa chỉ chân nút Reset
 *
 *ví dụ:
 *LCD_and_Buttons LCDBut (3, 1, 4, 5, 6, 7);
 *Trong ví dụ nút ViewMode được gắng vào pin số 3, 
 *tương tự, nút giảm tốc, tăng tốc được gắng vào pin số 4 và 5, và nút reset đc gắng vào pin số 7
 *
 *Lưu ý:
 *Chân điều khiển tốc độ băng chuyền là chân analog nên bắt buộc vào nối vào pin có hỗ trợ analog trên NodeMCU là pin số 1 hoặc 2
 */
LCD_and_Buttons LCDBut (int MBut, int SpeedPin, int SDBut, int SUBut, int SSBut, int RBut);

// Biến Weight để lưu cân nặng của trái
float weight = 0;

void setup() 
{
  LCDBut.LCDSetUp ()  //Setup ban đầu cho LCD, xuất ra màn hình "Welcome!" (Lưu ý địa chỉ bus ic là 0x27 nếu thay đổi địa chỉ ic thì vào 
                      //LCD_and_Buttons.h sửa lại)
}

void loop() 
{  
  if (isWeightChange (weight))     
  {                               //Nếu Weight thay đổi nghĩa là có trái mới được đặt lên bàn cân
    LCDBut.DataUpdate (weight);   //Thì lưu cân nặng của trái đó vào biến LCDBut.CurrentWeight
    LCDBut.ViewMode1 ();          //Rồi xuất ra LCD
  }

  if (isButPushed (SSBut))        //Nếu nút Start/Stop ở chân SSBut được tích cực bậc cao 
    StartStop();                  //Thì gọi hàm StartStop

  if (isButPushed (SDBut))        //Nếu nút SpeedDown ở chân SDBut được tích cực bậc cao 
    SpeedDown ();                 //Thì tốc độ được giảm đi 64 (Lưu ý: NodeMCU hỗ trợ chân PCM mô phỏng tín hiệu analog theo 256 
                                  //trạng thái từ 0 - 255, nên dãy tốc độ được chia thành 4 cấp, mỗi cấp cách nhau 64 trạng thái, 
                                  //mỗi lần gọi hàm SpeedDown(), tốc độ sẽ giảm đi 64 trạng thái)

  if (isButPushed (SUBut))        //Lần này là SpeedUp, tương tự như SpeedDown
    SpeedUp ();

  if (isButPushed (MBut))         //Nếu nút ViewMode ở chân MBut được tích cực bậc cao 
    Select ();                    //Thì LCD sẽ chuyển giữa 2 chế độ xem: xem cân nặng từng trái và xem số trái cân được

  if (isButPushed (RBut))         //Nếu nút ViewMode ở chân MBut được tích cực bậc cao 
    Reset ();                     //Thì reset count về 0
}
