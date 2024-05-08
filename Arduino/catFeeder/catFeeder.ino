#include <Arduino.h>
#include "GyverEncoder.h"
#include "feeder.h"
#include "view.h"

#define CLK 6
#define DT 7
#define SW 8
Encoder enc(CLK, DT, SW);

Feeder feeder;

View* view;
MainMenuView mainMenuView;
HomeView homeView;
SetTimeView setTimeView;
FeedingListView feedingListView;
FeedingItemMenuView feedingItemMenuView;

void setup() {
  enc.setType(TYPE2);
  model.init();
  initOled();

  setScreen(Home);
}

void loop() {
  // обязательные функции отработки. Должны постоянно опрашиваться
  enc.tick();
  model.tick();
  feeder.tick();
  view->tick();
  
  if (enc.isRight()) view -> turnRight();
  if (enc.isLeft()) view -> turnLeft();
  if (enc.isClick()) setScreen(view -> click());
}

void setScreen(ViewScreen screen){
  switch (screen)
  {
  case Home: view = &homeView; resetAllPointers(); break;
  case Main: view = &mainMenuView; break;
  case SetTime: view = &setTimeView; break;
  case FeedingsList: view = &feedingListView; break;
  case FeedingItemMenu: view = &feedingItemMenuView; break;
  case FeedingItemSettings: view = &homeView; break;
  }
  view -> show();
}

void resetAllPointers(){
  homeView.resetPointer();
  mainMenuView.resetPointer();
  setTimeView.resetPointer();
  feedingListView.resetPointer();
}