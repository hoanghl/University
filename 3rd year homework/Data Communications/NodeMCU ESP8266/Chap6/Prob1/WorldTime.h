/*
 *  Created by Tommy,
 *  Nov 11, 2018
 *  Saigon, Vietnam
 */ 
#ifndef WORLDTIME_H_
#define WORLDTIME_H_


String MONTH[] = {
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"};

short MAX_MONTH[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

class WorldTime {
private:
    short _GMT;
    String _location;
    int   _day, _month, _year, _hour, _minute, _second;
    bool  _isDay;
public:
    int dayUTC, monthUTC, yearUTC, hourUTC, minUTC, secUTC;
    bool isDayUTC;
    
    WorldTime(){}
    
    
    void addDat (String lo, short GMT, int da, int mo, int ye, int ho, int mi, int se, bool isD){
        dayUTC = da; monthUTC = mo; yearUTC = ye; hourUTC = ho; minUTC = mi; secUTC = se; isDayUTC = isD;
        _location = lo; _GMT = GMT;
    }


    
    short getMaxDay(int year, int month){
        if (year % 4 == 0)
            MAX_MONTH[1] = 29;
        else
            MAX_MONTH[1] = 28;
        return MAX_MONTH[month - 1];
    }

    void dayafter(){
        _isDay = ! isDayUTC;
        if (++_day > getMaxDay(_year, _month)){
            _day = 1;
            if (++_month > 12){
                _month = 1;
                _year++;
            }
        }
    }

    void daybefore(){
        _isDay = !isDayUTC;
        if (--_day == 0){        
            if (--_month == 0){
                _month = 12;
                _year--;            
            }
            _day = getMaxDay(_year, _month);
        }
    }

        
    void updateDay(){
        // min and sec remain the same, only hour changes
        _minute   = minUTC;
        _second   = secUTC;    
        _hour     = hourUTC + _GMT;
        _day      = dayUTC;
        _month    = monthUTC;
        _year     = yearUTC;
        
        // GMT varies from -12 to +12, so there 
        if (isDayUTC){
            if (_hour < 0)
                daybefore();
            else if (_hour >= 12)
                _isDay = ! isDayUTC;

        }
        if (!isDayUTC){
            if (_hour > 12)
                dayafter();
            else if (_hour <= 0)
                _isDay = ! isDayUTC;
        }
        
        _hour = abs(_hour) % 12;
        if (_hour == 0)
            _hour = 12;
    }

    void incDay(){
        if (++_second == 60){
            _second = 0;
            if (++_minute == 60){
                _minute = 0;
                if (++_hour == 12){
                    if (_isDay)
                        _isDay = !_isDay;
                    else
                        dayafter();
                }
                else _hour = _hour % 12;
            }
        }
    }
    
    String getStr(){
        String tmp = _location + ": " + MONTH[_month - 1] + " " + (String)_day + " " + (String)_year + " ";
        if (_hour < 10)
          tmp += "0";
        tmp += (String)_hour + ":";

        if (_minute < 10)
          tmp += "0";
        tmp += (String)_minute + ":";

        if (_second < 10)
          tmp += "0";
        tmp += (String)_second;
        
        if (_isDay)
            tmp += " AM";
        else
            tmp += " PM";
        return tmp;
    }
  
};

#endif 
