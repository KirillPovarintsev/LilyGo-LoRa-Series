#include "App.h"
#include <vector>

App app;

std::vector<std::string> rows = 
{
    "By 1",
    "By 2",
    "By 3",
    "By 4",
    "By 5",
    "By 6",
    "By 7"
};

int selectedRow = 0;
const uint8_t* font = u8g2_font_pxplusibmvga9_mf;
const int leftMargin = 5;
const int rowHeight = 16;
const int screenWidth = 128;
const int screenHeight = 64;
int offset = 0;

App::App()
{
    auto& action1 = _menu.addItem("Action 1", []() {});
    auto& action2 = _menu.addItem("Action 2", []() {});
    auto& submenu1 = _menu.addMenu("Sub-menu 1 >");
    auto& submenu1action1 = submenu1.addItem("Action 1-1", []() {});
    auto& submenu1action2 = submenu1.addItem("Action 1-2", []() {});
    auto& submenu1submenu1 = submenu1.addMenu("Sub-menu 1-1 >");
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

void App::onButtonClick()
{
    _menu.down();
}

void App::onButtonLongPress()
{
    _menu.select();
}

void App::paintMenu(int currentItemIndex)
{
    int selectedRowTop = currentItemIndex * rowHeight + offset;
    int selectedRowBottom = currentItemIndex + rowHeight - 1;

    if (selectedRowTop < 0)
    {
        offset -= selectedRowTop;
    }
    else if (selectedRowBottom >= screenHeight)
    {
        offset -= (selectedRowBottom - screenHeight + 1);
    }

    _u8g2.setFont(font);
    _u8g2.setFontMode(1);
}

void App::paintItem(int index, const char* text, bool selected)
{
    int top = index * rowHeight + offset;
    int bottom = top + rowHeight - 1;

    if (selected)
    {
        _u8g2.setDrawColor(1);
        _u8g2.drawBox(0, top, screenWidth, rowHeight);
    }

    _u8g2.setDrawColor(selected ? 0 : 1);
    _u8g2.setCursor(leftMargin, bottom - 3);
    _u8g2.print(text);
}

void App::onDraw()
{
    _u8g2.clearBuffer();

    _menu.draw(this);

    _u8g2.sendBuffer();
}

void App::drawMenu()
{
    int selectedRowTop = selectedRow * rowHeight + offset;
    int selectedRowBottom = selectedRowTop + rowHeight - 1;

    if (selectedRowTop < 0)
    {
        offset -= selectedRowTop;
    }
    else if (selectedRowBottom >= screenHeight)
    {
        offset -= (selectedRowBottom - screenHeight + 1);
    }

    _u8g2.setFont(font);
    _u8g2.setFontMode(1);
    _u8g2.setDrawColor(1);

    for (int r = 0; r < rows.size(); r++)
    {
        drawMenuItem(r, r == selectedRow);
    }
}

void App::drawMenuItem(int index, bool selected)
{
    int top = index * rowHeight + offset;
    int bottom = top + rowHeight - 1;

    if (selected)
    {
        _u8g2.drawBox(0, top, screenWidth, rowHeight);
        _u8g2.setDrawColor(0);
    }

    _u8g2.setCursor(leftMargin, bottom - 3);
    _u8g2.print(rows[index].c_str());

    if (selected)
    {
        _u8g2.setDrawColor(1);
    }
}