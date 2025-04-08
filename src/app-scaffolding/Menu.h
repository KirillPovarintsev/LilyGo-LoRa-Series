#ifndef MENU_H
#define MENU_H

#include <string>
#include <functional>
#include <memory>
#include <vector>

#include "IMenuPainter.h"

class Menu;
class MenuBase;
class SubMenu;

struct MenuItem
{
    Menu* menu; // TODO: get rid of
    std::string text;
    std::function<void()> action;
    SubMenu* subMenu;

    MenuItem(Menu* menu_, const char* text_, std::function<void()> action_);
    MenuItem(Menu* menu_, const char* text_, MenuBase* parent_);
    ~MenuItem();
};

class MenuBase
{
protected:
    Menu* _menu;
    int _currentItem;
    std::vector<MenuItem*> _items;

protected:
    MenuBase(Menu* menu);
    virtual ~MenuBase();

public:
    MenuItem& addItem(const char* text, std::function<void()> action);
    SubMenu& addMenu(const char* text);

    virtual void up();
    virtual void down();
    virtual void select();

    virtual void draw(IMenuPainter* painter);
    virtual void log();
};

class MainMenu : public MenuBase
{
public:
    MainMenu(Menu* menu);

    virtual void up();
    virtual void down();
};

class SubMenu : public MenuBase
{
protected:
    MenuBase* _parent;
    MenuItem _backItem; // TODO: get rid of

public:
    SubMenu(Menu* menu, MenuBase* parent);

    virtual void up();
    virtual void down();
    virtual void select();

    virtual void draw(IMenuPainter* painter);
    virtual void log();
};

class Menu
{
protected:
    MainMenu _mainMenu;
    MenuBase* _currentMenu;

public:
    Menu();

    MenuItem& addItem(const char* text, std::function<void()> action);
    SubMenu& addMenu(const char* text);

    void up();
    void down();
    void select();

    void draw(IMenuPainter* painter);
    virtual void log();

    friend class MenuBase;
    friend class SubMenu;
};

#endif //MENU_H