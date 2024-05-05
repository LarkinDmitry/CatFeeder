#include <GyverOLED.h>
#include "viewScreens.h"
#include "model.h"

GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

void initOled(){
    oled.init();
    oled.clear();
}

class View
{
    public:
    View(){};

    virtual ViewScreen click() { }
    virtual void turnRight() { pointerUp(); }
    virtual void turnLeft() { pointerDown(); }
    virtual void tick() { }
    
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

    ViewScreen click() override {
        oled.clear();
        switch (pointer)
        {
        case 0: return Home; break;
        case 1: return SetTime; break;
        case 2: return FeedingSchedule; break;
        }
    }

    protected:
    int8_t getTitlesCount() override { return 3; }
    String* getTitles() override{
        static String str[] = {"Назад", "Настройка времени", "График кормления"};
        return str;
    }
};

class HomeScreen : public View{
    public:
    HomeScreen(){};
    
    void show() override{
        oled.setScale(4);
        oled.setCursor(6, 2);
        DateTime now = rtc.getTime();
        if(now.hour < 10){
            oled.print(0);
        }
        oled.print(now.hour);
        oled.print(":");
        if(now.minute < 10){
            oled.print(0);
        }
        oled.print(now.minute);
    }

    void tick() override{
        show();
    }

    ViewScreen click() override {
        oled.clear();
        return Main;
    }
};