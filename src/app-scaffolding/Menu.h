#ifndef MENU_H
#define MENU_H

#include <string>
#include <functional>
#include <memory>
#include <vector>

class Menu;
class MenuBase;
class SubMenu;

struct MenuItem
{
    Menu* menu; // TODO: get rid of
    std::wstring text;
    std::function<void()> action;
    SubMenu* subMenu;

    MenuItem(Menu* menu_, const wchar_t* text_, std::function<void()> action_);
    MenuItem(Menu* menu_, const wchar_t* text_, MenuBase* parent_);
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
    MenuItem& addItem(const wchar_t* text, std::function<void()> action);
    SubMenu& addMenu(const wchar_t* text);

    virtual void up();
    virtual void down();
    virtual void select();

    virtual void draw();
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
    MenuItem _backItem;

public:
    SubMenu(Menu* menu, MenuBase* parent);

    virtual void up();
    virtual void down();
    virtual void select();

    virtual void draw();
};

class Menu
{
protected:
    MainMenu _mainMenu;
    MenuBase* _currentMenu;

public:
    Menu();

    MenuItem& addItem(const wchar_t* text, std::function<void()> action);
    SubMenu& addMenu(const wchar_t* text);

    void up();
    void down();
    void select();

    void draw();

    friend class MenuBase;
    friend class SubMenu;
};

#endif //MENU_H