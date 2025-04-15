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

class MenuItem
{
private:
    std::function<std::string()> _textCallback;
    std::function<void()> _action;
    SubMenu* _subMenu;

public:    
    MenuItem(std::function<std::string()> textCallback, std::function<void()> action, SubMenu* subMenu);
    ~MenuItem();

    inline std::string getText() { return _textCallback(); }
    inline void invokeAction() { _action(); }
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

    void logCurrentItem(std::function<void(std::string)> receiver) { receiver(_items[_currentItem]->getText()); }

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

    inline void logCurrentItem(std::function<void(std::string)> receiver) { _currentMenu->logCurrentItem(receiver); }

    void up();
    void down();
    void select();

    void draw(IMenuPainter* painter);

    friend class MenuBase;
    friend class SubMenu;
};

#endif //MENU_H