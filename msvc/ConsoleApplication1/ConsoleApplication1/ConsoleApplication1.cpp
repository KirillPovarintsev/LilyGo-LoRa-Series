// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <conio.h>

#include "..\..\..\src\app-scaffolding\Menu.h"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13
#define KEY_ESC 27

int main()
{
    Menu _menu;

    auto& action1 = _menu.addItem("Action 1", []() {});
    auto& action2 = _menu.addItem("Action 2", []() {});
    auto& submenu1 = _menu.addMenu("Sub-menu 1");
    auto& submenu1action1 = submenu1.addItem("Action 1-1", []() {});
    auto& submenu1action2 = submenu1.addItem("Action 1-2", []() {});
    auto& submenu1submenu1 = submenu1.addMenu("Sub-menu 1-1");
    auto& submenu1submenu1action1 = submenu1submenu1.addItem("Action 1-1-1", []() {});
    auto& submenu1submenu1action2 = submenu1submenu1.addItem("Action 1-2-1", []() {});

    auto logToStdOut = std::function<void(std::string)>([](auto s) { std::cout << s << "\n"; });

    _menu.log(logToStdOut);

    int c;

    do
    {
        c = _getch();

        switch (c)
        {
        case KEY_UP:
            _menu.up();
            _menu.log(logToStdOut);
            break;
        case KEY_DOWN:
            _menu.down();
            _menu.log(logToStdOut);
            break;
        case KEY_ENTER:
            _menu.select();
            _menu.log(logToStdOut);
            break;
        }

    } while (c != KEY_ESC);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
