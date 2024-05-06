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
        if(pointer > getTitlesCount() - 1) pointer = getTitlesCount() - 1;
        show();
    }
    
    void pointerDown() {
        pointer--;
        if(pointer < 0) pointer = 0;
        show();
    }

    virtual int8_t getTitlesCount() { return 0; }
    virtual String* getTitles(){
        static String str[] = {};
        return str;
    }
};

class HomeScreen : public View{
    public:
    HomeScreen(){};
    
    void show() override{
        oled.setScale(4);
        oled.setCursor(6, 2);
        oled.print(getHHMM());
    }

    void tick() override{
        show();
    }

    ViewScreen click() override {
        oled.clear();
        return Main;
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
        static String str[] = {"Назад", "Настройка времени", "График кормлений"};
        return str;
    }
};

class SetTimeScreen : public View{
    public:
    SetTimeScreen(){};

    ViewScreen click() override {
        pointer++;
        if(pointer == 3){
            pointer = 0;
            oled.clear();
            rtc.setTime(0,tempMM,tempHH,0,0,0);
            return Main;
        }
        else
        {
            return SetTime;
        }
    }
    
    void turnRight() override {
        if(pointer == 1){
            tempHH++;
            if(tempHH > 23) tempHH = 0;
        }
        else if(pointer == 2){
            tempMM++;
            if(tempMM > 59) tempMM = 0;
        }
        show();
    }
    
    void turnLeft() override {
        if(pointer == 1){
            tempHH--;
            if(tempHH < 0) tempHH = 23;
        }
        else if(pointer == 2){
            tempMM--;
            if(tempMM < 0) tempMM = 59;
        }

        show();
    }
    
    void show() override{
        if(pointer == 0){
            tempHH = rtc.getHours();
            tempMM = rtc.getMinutes();
            pointer++;
        }
        
        oled.setScale(4);

        String time = "";
        if(tempHH < 10) time += String(0, DEC);
        time += String(tempHH, DEC);        
        time += ":";        
        if(tempMM < 10) time += String(0, DEC);
        time += String(tempMM, DEC);
        oled.setCursor(6, 0);
        oled.print(time);
        
        oled.setCursor(6, 4);
        if(pointer == 1) oled.print("--   ");
        else if(pointer == 2) oled.print("   --");
    }
    
    protected:
    int8_t getTitlesCount() override { return 3; }

    private:
    int8_t tempHH = 0;
    int8_t tempMM = 0;
};