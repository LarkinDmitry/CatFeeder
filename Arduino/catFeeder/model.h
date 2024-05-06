#include <microDS3231.h>

MicroDS3231 rtc;

String getHHMM(){  
  String time = "";
  if(rtc.getHours() < 10) time += String(0, DEC);
  time += String(rtc.getHours(), DEC);
  time += ":";
  if(rtc.getMinutes() < 10) time += String(0, DEC);
  time += String(rtc.getMinutes(), DEC);  
  return time;
}

class Model{
  public:
  Model(){};

  void setBuildTime(){
    rtc.setTime(BUILD_SEC, BUILD_MIN, BUILD_HOUR, BUILD_DAY, BUILD_MONTH, BUILD_YEAR);
  }

  void tick(){

  }

  private:
  DateTime currentTime;
};