#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include <Task.h>
#include "BetterAceButton.h"
#include "Menu.h"
#include "LoRaBoards.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define MENU_LIST_MODE_FONT u8g2_font_pxplusibmvga9_mf
#define MENU_LIST_MODE_LEFT_MARGIN 5
#define MENU_LIST_MODE_ROW_HEIGHT 16

#define MENU_GRID_MODE_FONT u8g2_font_squeezed_b6_tr
#define MENU_GRID_MODE_LEFT_MARGIN 7
#define MENU_GRID_MODE_COLUMN_WIDTH 64
#define MENU_GRID_MODE_ROW_HEIGHT 32

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

    void paintMenuListMode(int currentItemIndex);
    void paintItemListMode(int index, const char* text, bool selected);
    void paintMenuGridMode(int currentItemIndex);
    void paintItemGridMode(int index, const char* text, bool selected);

private:
    BetterAceButton _button{BUTTON_PIN, [](){}, [](){}, [this](){ onButtonClick(); }, [this](){ onButtonLongPress(); }};
    DISPLAY_MODEL _u8g2{U8G2_R0, U8X8_PIN_NONE};
    Menu _menu;
    int _menuDrawOffset;
    bool _isOn1;
    bool _isOn2;
    
private:
    Task _draw{[this](){ onDraw(); }, 100};
};

extern App app;

#endif