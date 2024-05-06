#include <Arduino.h>
#include "GyverEncoder.h"
#include "feeder.h"
#include "view.h"

#define CLK 6
#define DT 7
#define SW 8
Encoder enc(CLK, DT, SW);

Model model;

Feeder feeder;

View* view;
MainMenu mainMenu;
HomeScreen homeScreen;
SetTimeScreen setTimeScreen;

void setup() {  
  enc.setType(TYPE2);
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
  case Home: view = &homeScreen; break;
  case Main: view = &mainMenu; break;
  case SetTime: view = &setTimeScreen; break;
  case FeedingSchedule: view = &mainMenu; break;
  case FeedingItemMenu: view = &mainMenu; break;
  case FeedingItemSettings: view = &mainMenu; break;
  }
  view -> show();
}