#include <Arduino.h>
#include "GyverEncoder.h"
#include "view.h"

#define CLK 6
#define DT 7
#define SW 8
Encoder enc(CLK, DT, SW);

View* view;
MainMenuView mainMenuView;
HomeView homeView;
SetTimeView setTimeView;
FeedingListView feedingListView;
FeedingItemMenuView feedingItemMenuView;
FeedingItemSettingsView feedingItemSettingsView;

DateTime lastActionTime;

void setup() {
  Serial.begin(115200);
  enc.setType(TYPE2);
  model.init();
  oled.init();
  oled.clear();
  setScreen(Home);
}

void loop() {
  // обязательные функции отработки. Должны постоянно опрашиваться
  enc.tick();
  view->tick();
  model.tick();
  
  if (enc.isRight()){
    lastActionTime = rtc.getTime();
    view -> turnRight();
  }
  
  if (enc.isLeft()){
    lastActionTime = rtc.getTime();
    view -> turnLeft();
  }
  
  if (enc.isClick()){
    lastActionTime = rtc.getTime();
    setScreen(view -> click());
  }

  checkInactivityTime();
}

void setScreen(ViewScreen screen){
  switch (screen)
  {
  case Home: view = &homeView; resetAllPointers(); break;
  case Main: view = &mainMenuView; break;
  case SetTime: view = &setTimeView; break;
  case FeedingsList: view = &feedingListView; break;
  case FeedingItemMenu: view = &feedingItemMenuView; break;
  case FeedingItemSettings: view = &feedingItemSettingsView; break;
  }
  view -> show();
}

void resetAllPointers(){
  homeView.resetPointer();
  mainMenuView.resetPointer();
  setTimeView.resetPointer();
  feedingListView.resetPointer();
  feedingItemMenuView.resetPointer();
  feedingItemSettingsView.resetPointer();
}

void checkInactivityTime(){
  if(view->screenType != Home){
    if(getDeltaSeconds(lastActionTime, rtc.getTime()) > 30){      
      oled.clear();     
      setScreen(Home);
    }
  }
}

int getDeltaSeconds(DateTime firstDate, DateTime secondDate){
  int firstSeconds = firstDate.hour * 3600 + firstDate.minute * 60 + firstDate.second;
  int secondSeconds = secondDate.hour * 3600 + secondDate.minute * 60 + secondDate.second;
  return secondSeconds - firstSeconds;
}