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
private:
    std::string text;
    std::function<std::string()> textCallback;
    std::function<void()> action;
    SubMenu* subMenu;

public:    
    MenuItem(const char* text_, std::function<void()> action_);
    MenuItem(std::function<std::string()> textCallback_, std::function<void()> action_);
    MenuItem(const char* text_, MenuBase* parent_);
    MenuItem(const char* text_, SubMenu* subMenu_);
    ~MenuItem();

    void select();

    inline std::string getText() { return textCallback ? textCallback() : text; }
};

class MenuBase
{
protected:
    Menu* _menu;
    int _currentItem;
    std::vector<MenuItem*> _items;

protected:
    MenuBase(Menu* menu);
    MenuBase(MenuBase* parent);
    virtual ~MenuBase();

public:
    MenuItem& addItem(const char* text, std::function<void()> action);
    MenuItem& addItem(std::function<std::string()> textCallback, std::function<void()> action);
    SubMenu& addMenu(const char* text);

    virtual std::string currentItem() { return _items[_currentItem]->getText(); }
    void makeCurrent();

    virtual void up();
    virtual void down();
    virtual void select();

    virtual void draw(IMenuPainter* painter);

protected:
    virtual int newItemPosition() { return _items.size(); }
};

class MainMenu : public MenuBase
{
public:
    MainMenu(Menu* menu);
};

class SubMenu : public MenuBase
{
protected:
    MenuBase* _parent;

public:
    SubMenu(MenuBase* parent);

protected:
    virtual int newItemPosition() { return _items.size() - 1; }
};

class Menu
{
protected:
    MainMenu _mainMenu;
    MenuBase* _currentMenu;

public:
    Menu();

    MenuItem& addItem(const char* text, std::function<void()> action);
    MenuItem& addItem(std::function<std::string()> textCallback, std::function<void()> action);
    SubMenu& addMenu(const char* text);

    void log(std::function<void(std::string)> receiver)
    {
        receiver(_currentMenu->currentItem());
    }

    void up();
    void down();
    void select();

    void draw(IMenuPainter* painter);

    friend class MenuBase;
    friend class SubMenu;
};

#endif //MENU_H