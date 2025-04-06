#include "Menu.h"

MenuItem::MenuItem(Menu* menu_, const wchar_t* text_, std::function<void()> action_) :
    menu(menu_),
    text(text_),
    action(action_),
    subMenu(nullptr)
{
}

MenuItem::MenuItem(Menu* menu_, const wchar_t* text_, MenuBase* parent_) :
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

MenuItem& MenuBase::addItem(const wchar_t* text, std::function<void()> action)
{
    auto item = new MenuItem(_menu, text, action);
    _items.push_back(item);
    return *item;
}

SubMenu& MenuBase::addMenu(const wchar_t* text)
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

void MenuBase::draw()
{
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
    _backItem(_menu, L"Back", [this](){ _menu->_currentMenu = _parent; _currentItem = 0; })
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

void SubMenu::draw()
{
    if (_currentItem == -1)
    {
    }
    else
    {
        MenuBase::draw();
    }
}

Menu::Menu() :
    _mainMenu(this),
    _currentMenu(&_mainMenu)
{
}

MenuItem& Menu::addItem(const wchar_t* text, std::function<void()> action)
{
    return _mainMenu.addItem(text, action);
}

SubMenu& Menu::addMenu(const wchar_t* text)
{
    return _mainMenu.addMenu(text);
}

void Menu::up()
{
    _currentMenu->up();
}

void Menu::down()
{
    _currentMenu->down();
}

void Menu::select()
{
    _currentMenu->select();
}

void Menu::draw()
{
    _currentMenu->draw();
}