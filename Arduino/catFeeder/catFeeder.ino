#include <Arduino.h>
#include "GyverEncoder.h"
#include "feeder.h"
#include "view.h"
#include "model.h"

#define CLK 6
#define DT 7
#define SW 8
Encoder enc(CLK, DT, SW);

Feeder feeder;
View* view;

void setup() {  
  enc.setType(TYPE2);
  
  initOled();

  view =&homeScreen;
  view -> show();

}

void loop() {
  feeder.tick();

  // обязательная функция отработки. Должна постоянно опрашиваться
  enc.tick();
  
  if (enc.isRight()) view -> turnRight();
  if (enc.isLeft()) view -> turnLeft();
  
  
  if (enc.isClick()){
    view =(view -> click());
    view -> show();
  }
}