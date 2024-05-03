#include <GyverOLED.h>
#include "viewScreens.h"

GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

void initOled(){
    oled.init();
    oled.clear();
}

class View
{
    public:
    View(){};

    virtual View* click() { return this; }
    virtual void turnRight() {};    
    virtual void turnLeft() {};
    
    virtual void show() {
        oled.setScale(1);
        for(int i = 0; i < getTitlesCount(); i++){
            oled.setCursor(0, i);
            if(i == pointer){ oled.print("> " + getTitles()[i]); }
            else{ oled.print("  " + getTitles()[i]); }
        }
    }
    
    protected:
    int8_t pointer = 0;
    
    void pointerUp() {
        pointer++;
        if(pointer > getTitlesCount() - 1){
            pointer = getTitlesCount() - 1;
        }
        show();
    }
    
    void pointerDown() {
        pointer--;
        if(pointer < 0){
            pointer = 0;
        }
        show();
    }

    virtual int8_t getTitlesCount() { return 0; }
    virtual String* getTitles(){
        static String str[] = {};
        return str;
    }
};

class MainMenu : public View{
    public:
    MainMenu(){};
    
    void turnRight() override{
        pointerUp();
    }
    
    void turnLeft() override{
        pointerDown();
    }

    protected:
    int8_t getTitlesCount() override { return 3; }
    String* getTitles() override{
        static String str[] = {"Назад", "Настройка времени", "График кормления"};
        return str;
    }
};
MainMenu mainMenu;

class HomeScreen : public View{
    public:
    HomeScreen(){};
    
    void show() override{
        oled.setScale(2);
        oled.setCursor(0, 0);
        oled.print("HomeScreen");
    }

    View* click() override {
        oled.clear();
        return &mainMenu;
    }
};
HomeScreen homeScreen;





/*
void print(const char str[]) {
  //oled.clear();   // очистить дисплей (или буфер)
  //oled.setScale(2);
  //oled.home();    // курсор в 0,0
  //oled.print(str);  // печатай что угодно: числа, строки, float, как Serial!

  DateTime now = rtc.getTime();    
  Serial.print(now.hour);
  Serial.print(":");
  Serial.print(now.minute);
  Serial.print(":");
  Serial.print(now.second);
  Serial.print(" ");
  Serial.print(now.day);
  Serial.print(" ");
  Serial.print(now.date);
  Serial.print("/");
  Serial.print(now.month);
  Serial.print("/");
  Serial.println(now.year);

  //oled.setCursor(1, 5);   // курсор в (пиксель X, строка Y)
  //oled.setScale(2);
  //oled.print(rtc.getTimeString());  // печатай что угодно: числа, строки, float, как Serial!
  //oled.update();
}
*/