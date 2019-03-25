/*  Pseudo code:
 *  timeout = 5s
 *  cases exporting fail:
 *    - String == null or string's length < 4
 *    - unmatched passcode
 *    keep reading char x:
 *      if (x == '#'){
 *        if (has read '#' before){
 *          if (!isTimeOut())
 *            if (begin_exporting_passcode_from_read_characters() == true)
 *              open_door();
 *            else
 *              close_door();
 *            flush_Buffer();
 *            resetCounter();
 *          }
 *        }
 *        else timeout{
 *          reset everything
 *        }
 *        else
 *          read_#_before = true
 *          startCounter();
 *      } 
 *      else{ // read another characters
 *        if (isTimeOut() == false && has read '#' before) //
 *          put_read_character_to_end_of_buffer();
 *        else
 *          <do_nothing>     
 *      }
 *   
 */
///////////////////////////////////////////////////////////


char s[100];
int n = 0;

void Buffer_flush(){
  for (int i = 0; i < n; i++)
    s[i] = 0;
  n = 0;
}

void Buffer_add(char x){
  s[n] = x;
  n++;
}
bool Buffer_export(){
  /* Failed cases:
   * 1. String null or string's length < 4
   * 2. unmatched passcode
   */

   if (n == 0)
    return false;

   if ((s[n-4] == 'O') &&
       (s[n-3] == 'P') &&
       (s[n-2] == 'E') &&
       (s[n-1] == 'N')
      )
     return true;
   else
     return false;
}

///////////////////////////////////////////////////////////

unsigned long  current, prev;
bool  counting = false;
void  Counter_start(){
  counting = true;
  prev = millis();
}

bool Counter_is5sElapsed(){
  if (counting == false)
    return false;
  current = millis();
  if (current - prev >= 5000){
    counting = false;
    return true;
  }    
  else return false;
}

void Counter_reset(){
  counting = false;
}

///////////////////////////////////////////////////////////

void Door_open(){
  digitalWrite(LED_BUILTIN, LOW);
}

void Door_close(){
  digitalWrite(LED_BUILTIN, HIGH);
}

///////////////////////////////////////////////////////////

bool sharpPressed = false;

void resetAll(){
  Buffer_flush();
  Counter_reset();
  sharpPressed = false;
}


void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  char tmp = Serial.read();
  if ((int)tmp > 31 && (int)tmp < 127){
    if (tmp == '#'){
      if (!Counter_is5sElapsed()){
        if (sharpPressed == true){  // received '#' before
          if (Buffer_export() == true)
            Door_open(); 
          else
            Door_close();
          resetAll();
        }
        else {
          Serial.println(tmp);
          sharpPressed = true;
          Counter_start();
        }
      }
      else     // time is up
        resetAll();
    }
    else{        // read another characters
      Serial.println(tmp);
      if (!Counter_is5sElapsed()){
        if (sharpPressed)
          Buffer_add(tmp);
      }
      else {
        resetAll();
      }
    }
  }
}
