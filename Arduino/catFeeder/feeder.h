#include <GyverStepper2.h>

#define STEPS 200
#define STEP 4
#define DIR 2
#define EN 5
#define DRIVER_STEP_TIME 2  // меняем задержку на 2 мкс

GStepper2<STEPPER2WIRE> stepper(STEPS, STEP, DIR, EN); // драйвер step-dir + пин enable

class Feeder{
  public:
  Feeder()
  {
    stepper.setAcceleration(200 * 10000);
    stepper.setMaxSpeedDeg(180 * 1);  
    stepper.autoPower(true);
    stepper.disable();
  };

  bool tick(){
    if(stepper.tick()){

      return true;
    }
    else{

      return false;
    }
  }

  // 1 порция - это половина оборота
  void givePortions(int count) {
    for(int i = 0; i < count; i++){
      for(int j = 0; j < 4; j++){
        stepper.setTarget(-5, RELATIVE);
        while(tick()){
          tick();
        }
        stepper.setTarget(30, RELATIVE);
        while(tick()){
          tick();
        }
      }
    }
  }
};