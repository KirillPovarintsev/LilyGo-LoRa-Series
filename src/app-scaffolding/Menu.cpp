#include "Menu.h"

MenuItem::MenuItem(const char* text_, std::function<void()> action_) :
    text(text_),
    textCallback(nullptr),
    action(action_),
    subMenu(nullptr)
{
}

MenuItem::MenuItem(std::function<std::string()> textCallback_, std::function<void()> action_) :
    text(""),
    textCallback(textCallback_),
    action(action_),
    subMenu(nullptr)
{
}

MenuItem::MenuItem(const char* text_, MenuBase* parent_) :
    text(text_),
    textCallback(nullptr),
    subMenu(new SubMenu(parent_))
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

MenuBase::MenuBase(MenuBase* parent) :
    _menu(parent->_menu),
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
    auto item = new MenuItem(text, action);
    _items.push_back(item);
    return *item;
}

MenuItem& MenuBase::addItem(std::function<std::string()> textCallback, std::function<void()> action)
{
    auto item = new MenuItem(textCallback, action);
    _items.push_back(item);
    return *item;
}

SubMenu& MenuBase::addMenu(const char* text)
{
    auto item = new MenuItem(text, this);
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

void MenuBase::draw(IMenuPainter* painter)
{
    painter->paintMenu(_currentItem);

    for (int i = 0; i < _items.size(); i++)
    {
        painter->paintItem(i, _items[i]->getText().c_str(), i == _currentItem);
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

SubMenu::SubMenu(MenuBase* parent) :
    MenuBase(parent),
    _parent(parent),
    _backItem("...Back", [this](){ _menu->_currentMenu = _parent; _currentItem = 0; })
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

void SubMenu::draw(IMenuPainter* painter)
{
    painter->paintMenu(_currentItem == -1 ? _items.size() : _currentItem);

    for (int i = 0; i < _items.size(); i++)
    {
        painter->paintItem(i, _items[i]->getText().c_str(), i == _currentItem);
    }

    painter->paintItem(_items.size(), _backItem.getText().c_str(), _currentItem == -1);
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

MenuItem& Menu::addItem(std::function<std::string()> textCallback, std::function<void()> action)
{
    return _mainMenu.addItem(textCallback, action);
}

SubMenu& Menu::addMenu(const char* text)
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

void Menu::draw(IMenuPainter* painter)
{
    _currentMenu->draw(painter);
}