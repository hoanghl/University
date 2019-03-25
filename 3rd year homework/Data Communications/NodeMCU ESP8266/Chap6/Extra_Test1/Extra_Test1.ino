String payload;
String arr[5];
short i = 0;
short pos = 0;

int day = 0, month = 0, year = 0, hour = 0, minute = 0, second = 0;
String MONTH[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
void proceduceDate(){
  
  int i = 0;
  for (; i < payload.length() && (payload.charAt(i) < '0' || payload.charAt(i) > '9'); i++);

  payload = payload.substring(i);

   i = 0;
  while (pos != -1){
    pos = payload.indexOf(" ");
    arr[i] = payload.substring(0, pos);
    payload = payload.substring(pos + 1);
    i++;
  }
  day = arr[0].toInt();
  for (int i = 0; i < 12; i++){
    if (MONTH[(month = i)] == arr[1]){
      break;
    }
  }
  month++;
  year = arr[2].toInt();

  payload = arr[3];
  // export hour::min:sec
  i = pos = 0;
  while (pos != -1){
    pos = payload.indexOf(":");
    arr[i] = payload.substring(0, pos);
    payload = payload.substring(pos + 1);
    i++;
  }
  Serial.print("'" + arr[0] + "'\n");
  Serial.print("'" + arr[1] + "'\n");
  Serial.print("'" + arr[2] + "'\n");
  
  hour = arr[0].toInt();
  minute = arr[1].toInt();
  second = arr[2].toInt();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  payload = "Current UTC: 10 Nov 2018 2:50:45 PM";
  proceduceDate();

  Serial.println(day);
  Serial.println(month);
  Serial.println(year);
  Serial.println(hour);
  Serial.println(minute);
  Serial.println(second);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
