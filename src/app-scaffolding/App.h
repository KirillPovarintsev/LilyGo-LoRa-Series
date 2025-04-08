#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include <Task.h>
#include "BetterAceButton.h"
#include "Menu.h"
#include "LoRaBoards.h"

class App : IMenuPainter
{
public:
    App();
    virtual ~App();

public:
    void setup();
    void loop();

// IMenuPainter
public:
    virtual void paintMenu(int currentItemIndex);
    virtual void paintItem(int index, const char* text, bool selected);

private:
    void onDraw();
    void onButtonClick();
    void onButtonLongPress();

private:
    void drawMenu();
    void drawMenuItem(int index, bool selected);

private:
    BetterAceButton _button{BUTTON_PIN, [](){}, [](){}, [this](){ onButtonClick(); }, [this](){ onButtonLongPress(); }};
    DISPLAY_MODEL _u8g2{U8G2_R0, U8X8_PIN_NONE};
    Menu _menu;

private:
    Task _draw{[this](){ onDraw(); }, 100};
};

extern App app;

#endif