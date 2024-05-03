#include <microDS3231.h>

MicroDS3231 rtc;

class Model{
  public:
  Model()
  {
    currentTime = rtc.getTime();
  };

  void setTime(DateTime time){
    rtc.setTime(BUILD_SEC, BUILD_MIN, BUILD_HOUR, BUILD_DAY, BUILD_MONTH, BUILD_YEAR);
  }

  private:
  DateTime currentTime;
};