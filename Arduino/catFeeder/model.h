#include <microDS3231.h>
#include <EEPROM.h>
#include "feeder.h"

MicroDS3231 rtc;
Feeder feeder;

struct FeedingItem
{
  bool isActive = false;
  int8_t hour = 0;
  int8_t minute = 0;
  int8_t portions = 0;

  int portionsNow(DateTime now){
    if(isActive && now.hour == hour && now.minute == minute){
      return portions;
    }
    else{
      return 0;
    }
  }
};

struct FeedingsListData
{
  uint8_t checkValue0 = 0;
  uint8_t checkValue255 = 255;
  uint8_t listLength = 7;
  FeedingItem feedingList[7];
};

class Model{
  public:
  Model(){};

  int selectedItem = 0;
  int readyPortions = 0;
  FeedingsListData feedingData;

  void init(){
    EEPROM.get(0, feedingData);

    // first start check
    if(feedingData.checkValue0 != 0 || feedingData.checkValue255 != 255){
      FeedingsListData defData;
      EEPROM.put(0, defData);
      EEPROM.get(0, feedingData);
    }
  }

  void clearSelectedItem(){
    feedingData.feedingList[selectedItem].isActive = false;
    feedingData.feedingList[selectedItem].hour = 0;
    feedingData.feedingList[selectedItem].minute = 0;
    feedingData.feedingList[selectedItem].portions = 0;
    EEPROM.put(0, feedingData);
  }

  FeedingItem getSelectedItem(){
    return feedingData.feedingList[selectedItem];
  }

  void saveSelectedItem(FeedingItem item){
    feedingData.feedingList[selectedItem] = item;
    EEPROM.put(0, feedingData);
  }

  void tick(){
    DateTime now = rtc.getTime();
    if(lastCheckTime.second != now.second){
      lastCheckTime = now;
      if(now.second == 0){
        for(int i = 0; i < feedingData.listLength; i++){
          int j = feedingData.feedingList[i].portionsNow(now);
          readyPortions += j;
        }
      }
    }

    if(readyPortions > 0){
      feeder.givePortions(readyPortions);
      readyPortions = 0;
    }

    feeder.tick();
  }

  private:
  DateTime lastCheckTime;
};
Model model;

String getHHMM(){
  String time = "";
  if(rtc.getHours() < 10) time += String(0, DEC);
  time += String(rtc.getHours(), DEC) + ":";
  if(rtc.getMinutes() < 10) time += String(0, DEC);
  time += String(rtc.getMinutes(), DEC);
  return time;
}

String feedingItemToString(FeedingItem item){
  String str = "";
  if(item.isActive){
    if(item.hour < 10) str += String(0, DEC);
    str += String(item.hour, DEC) + ":";
    if(item.minute < 10) str += String(0, DEC);
    str += String(item.minute, DEC) + " - ";
    if(item.portions < 10) str += String(0, DEC);
    str += String(item.portions, DEC) + " порц.";
  }
  else{
    str = "---";
  }
  return str;
}