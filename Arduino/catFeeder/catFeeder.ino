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
MainMenu mainMenu;
HomeScreen homeScreen;

void setup() {  
  enc.setType(TYPE2);
  
  initOled();

  view =&homeScreen;
  view -> show();
}

void loop() {
  // обязательные функции отработки. Должны постоянно опрашиваться
  enc.tick();
  feeder.tick();
  
  if (enc.isRight()) view -> turnRight();
  if (enc.isLeft()) view -> turnLeft();
  if (enc.isClick()){
    ViewScreen newScreen = view -> click();
    view = setScreen(newScreen);
    view -> show();
  }

  //view -> show();

  //if(view )
}

View* setScreen(ViewScreen screen){
  switch (screen)
  {
  case Home: return &homeScreen; break;
  case Main: return &mainMenu; break;
  case SetTime: return &mainMenu; break;
  case FeedingSchedule: return &mainMenu; break;
  case FeedingItemMenu: return &mainMenu; break;
  case FeedingItemSettings: return &mainMenu; break;
  }
}