#include <microDS3231.h>
#include <EEPROM.h>

MicroDS3231 rtc;

struct FeedingItem
{
  bool isActive = false;
  uint8_t hour = 0;
  uint8_t minute = 0;
  uint8_t portions = 0;
};

struct FeedingsListData
{
  uint8_t checkValue0 = 0;
  uint8_t checkValue255 = 255;
  uint8_t listLength = 5;
  FeedingItem feedingList[5];
};

class Model{
  public:
  Model(){};

  int selectedItem = 0;
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
    
  }
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
    str += String(item.minute, DEC) + " - " + String(item.portions, DEC) + " порц.";
  }
  else{
    str = "---";
  }
  return str;
}