#include "App.h"
#include <vector>

App app;

char buff[25];

App::App() :
    _menuDrawOffset(0),
    _isOn1(false),
    _isOn2(true)
{
    auto& action1 = _menu.addItem([this]()
    {
        std::sprintf(buff, "O1 is: %s", _isOn1 ? "ON" : "OFF");
        return buff;
    }, [this]() { _isOn1 = !_isOn1; });
    auto& action2 = _menu.addItem([this]()
    {
        std::sprintf(buff, "O2 is: %s", _isOn2 ? "ON" : "OFF");
        return buff;
    }, [this]() { _isOn2 = !_isOn2; });
    auto& action3 = _menu.addItem("Action 3", []() {});
    auto& action4 = _menu.addItem("Action 4", []() {});
    auto& submenu1 = _menu.addMenu("More >");
    auto& submenu1action1 = submenu1.addItem("Action 1-1", []() {});
    auto& submenu1action2 = submenu1.addItem("Action 1-2", []() {});
    auto& submenu1action3 = submenu1.addItem("Action 1-3", []() {});
    auto& submenu1action4 = submenu1.addItem("Action 1-4", []() {});
    auto& submenu1submenu1 = submenu1.addMenu("Even more >");
    auto& submenu1submenu1action1 = submenu1submenu1.addItem("Action 1-1-1", []() {});
    auto& submenu1submenu1action2 = submenu1submenu1.addItem("Action 1-2-1", []() {});
}

App::~App()
{
}

void App::setup()
{
    _u8g2.begin();

    _draw.start();
}

void App::loop()
{
    _button.check();

    _draw.loop();
}

void App::paintMenu(int currentItemIndex)
{
    paintMenuGridMode(currentItemIndex);
}

void App::paintItem(int index, const char* text, bool selected)
{
    paintItemGridMode(index, text, selected);
}

void App::onDraw()
{
    _u8g2.clearBuffer();

    _menu.draw(this);

    _u8g2.sendBuffer();
}

void App::onButtonClick()
{
    _menu.down();
}

void App::onButtonLongPress()
{
    _menu.select();
}

void App::paintMenuListMode(int currentItemIndex)
{
    _menuDrawOffset = 0;

    int selectedRowTop = currentItemIndex * MENU_LIST_MODE_ROW_HEIGHT + _menuDrawOffset;
    int selectedRowBottom = selectedRowTop + MENU_LIST_MODE_ROW_HEIGHT - 1;

    if (selectedRowTop < 0)
    {
        _menuDrawOffset -= selectedRowTop;
    }
    else if (selectedRowBottom >= SCREEN_HEIGHT)
    {
        _menuDrawOffset -= (selectedRowBottom - SCREEN_HEIGHT + 1);
    }

    _u8g2.setFont(MENU_LIST_MODE_FONT);
    _u8g2.setFontMode(1);
}

void App::paintItemListMode(int index, const char* text, bool selected)
{
    int top = index * MENU_LIST_MODE_ROW_HEIGHT + _menuDrawOffset;
    int bottom = top + MENU_LIST_MODE_ROW_HEIGHT - 1;

    if (selected)
    {
        _u8g2.setDrawColor(1);
        _u8g2.drawBox(0, top, SCREEN_WIDTH, MENU_LIST_MODE_ROW_HEIGHT);
    }

    _u8g2.setDrawColor(selected ? 0 : 1);
    _u8g2.setCursor(MENU_LIST_MODE_LEFT_MARGIN, bottom - 3);
    _u8g2.print(text);
}

void App::paintMenuGridMode(int currentItemIndex)
{
    _menuDrawOffset = 0;

    int selectedRowTop = currentItemIndex / 2 * MENU_GRID_MODE_ROW_HEIGHT + _menuDrawOffset;
    int selectedRowBottom = selectedRowTop + MENU_GRID_MODE_ROW_HEIGHT - 1;

    if (selectedRowTop < 0)
    {
        _menuDrawOffset -= selectedRowTop;
    }
    else if (selectedRowBottom >= SCREEN_HEIGHT)
    {
        _menuDrawOffset -= (selectedRowBottom - SCREEN_HEIGHT + 1);
    }

    _u8g2.setFont(MENU_GRID_MODE_FONT);
    _u8g2.setFontMode(1);
}

void App::paintItemGridMode(int index, const char* text, bool selected)
{
    int top = index / 2 * MENU_GRID_MODE_ROW_HEIGHT + _menuDrawOffset;
    int bottom = top + MENU_GRID_MODE_ROW_HEIGHT - 1;
    int left = (index % 2) * MENU_GRID_MODE_COLUMN_WIDTH;
    int right = left + MENU_GRID_MODE_COLUMN_WIDTH - 1;

    if (selected)
    {
        _u8g2.setDrawColor(1);
        _u8g2.drawBox(left, top, MENU_GRID_MODE_COLUMN_WIDTH, MENU_GRID_MODE_ROW_HEIGHT);
    }

    _u8g2.setDrawColor(selected ? 0 : 1);
    _u8g2.drawFrame(left + 1, top + 1, MENU_GRID_MODE_COLUMN_WIDTH - 2, MENU_GRID_MODE_ROW_HEIGHT - 2);
    _u8g2.setCursor(left + MENU_GRID_MODE_LEFT_MARGIN, bottom - 6);
    _u8g2.print(text);
}
