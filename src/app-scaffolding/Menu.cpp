#include <Arduino.h>

#include "Menu.h"

MenuItem::MenuItem(Menu* menu_, const char* text_, std::function<void()> action_) :
    menu(menu_),
    text(text_),
    action(action_),
    subMenu(nullptr)
{
}

MenuItem::MenuItem(Menu* menu_, const char* text_, MenuBase* parent_) :
    menu(menu_),
    text(text_),
    subMenu(new SubMenu(menu_, parent_))
{
}

MenuItem::~MenuItem()
{
    if (subMenu) delete subMenu;
}

MenuBase::MenuBase(Menu* menu) :
    _menu(menu),
    _currentItem(0)
{
}

MenuBase::~MenuBase()
{
    for (const auto& item : _items)
    {
        delete item;
    }
}

MenuItem& MenuBase::addItem(const char* text, std::function<void()> action)
{
    auto item = new MenuItem(_menu, text, action);
    _items.push_back(item);
    return *item;
}

SubMenu& MenuBase::addMenu(const char* text)
{
    auto item = new MenuItem(_menu, text, this);
    _items.push_back(item);
    return *item->subMenu;
}

void MenuBase::up()
{
    _currentItem--;
}

void MenuBase::down()
{
    _currentItem++;
}

void MenuBase::select()
{
    auto item = _items[_currentItem];

    if (item->subMenu)
    {
        _menu->_currentMenu = static_cast<MenuBase*>(item->subMenu);
    }
    else if (item->action)
    {
        item->action();
    }
}

void MenuBase::log()
{
    Serial.println(_items[_currentItem]->text.c_str());
}

void MenuBase::draw(IMenuPainter* painter)
{
    painter->paintMenu(_currentItem);

    for (int i = 0; i < _items.size(); i++)
    {
        painter->paintItem(i, _items[i]->text.c_str(), i == _currentItem);
    }
}

MainMenu::MainMenu(Menu* menu) : MenuBase(menu)
{
}

void MainMenu::up()
{
    MenuBase::up();

    if (_currentItem < 0) _currentItem = _items.size() - 1;
}

void MainMenu::down()
{
    MenuBase::down();

    if (_currentItem >= _items.size()) _currentItem = 0;
}

SubMenu::SubMenu(Menu* menu, MenuBase* parent) :
    MenuBase(menu),
    _parent(parent),
    _backItem(_menu, "< Back", [this](){ _menu->_currentMenu = _parent; _currentItem = 0; })
{
}

void SubMenu::up()
{
    MenuBase::up();

    if (_currentItem < -1) _currentItem = _items.size() - 1;
}

void SubMenu::down()
{
    MenuBase::down();

    if (_currentItem >= _items.size()) _currentItem = -1;
}

void SubMenu::select()
{
    if (_currentItem == -1)
    {
        _backItem.action();
    }
    else
    {
        MenuBase::select();
    }
}

void SubMenu::log()
{
    Serial.println(_currentItem == -1 ? _backItem.text.c_str() : _items[_currentItem]->text.c_str());
}

void SubMenu::draw(IMenuPainter* painter)
{
    painter->paintMenu(_currentItem == -1 ? _items.size() : _currentItem);

    for (int i = 0; i < _items.size(); i++)
    {
        painter->paintItem(i, _items[i]->text.c_str(), i == _currentItem);
    }

    painter->paintItem(_items.size(), _backItem.text.c_str(), _currentItem == -1);
}

Menu::Menu() :
    _mainMenu(this),
    _currentMenu(&_mainMenu)
{
}

MenuItem& Menu::addItem(const char* text, std::function<void()> action)
{
    return _mainMenu.addItem(text, action);
}

SubMenu& Menu::addMenu(const char* text)
{
    return _mainMenu.addMenu(text);
}

void Menu::up()
{
    _currentMenu->up();

    log();
}

void Menu::down()
{
    _currentMenu->down();

    log();
}

void Menu::select()
{
    _currentMenu->select();

    log();
}

void Menu::log()
{
    _currentMenu->log();
}

void Menu::draw(IMenuPainter* painter)
{
    _currentMenu->draw(painter);
}