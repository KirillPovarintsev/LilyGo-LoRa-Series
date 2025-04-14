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

MenuItem::MenuItem(const char* text_, SubMenu* subMenu_) :
    text(text_),
    textCallback(nullptr),
    subMenu(subMenu_)
{
}

MenuItem::~MenuItem()
{
    if (subMenu) delete subMenu;
}

void MenuItem::select()
{
    if (subMenu)
    {
        subMenu->makeCurrent();
    }
    else if (action)
    {
        action();
    }
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
    _items.insert(_items.begin() + newItemPosition(), item);
    return *item;
}

MenuItem& MenuBase::addItem(std::function<std::string()> textCallback, std::function<void()> action)
{
    auto item = new MenuItem(textCallback, action);
    _items.insert(_items.begin() + newItemPosition(), item);
    return *item;
}

SubMenu& MenuBase::addMenu(const char* text)
{
    auto subMenu = new SubMenu(this);
    auto item = new MenuItem(text, subMenu);
    _items.insert(_items.begin() + newItemPosition(), item);
    return *subMenu;
}

void MenuBase::makeCurrent()
{
    _menu->_currentMenu = this;
}

void MenuBase::up()
{
    _currentItem--;

    if (_currentItem < 0) _currentItem = _items.size() - 1;
}

void MenuBase::down()
{
    _currentItem++;

    if (_currentItem >= _items.size()) _currentItem = 0;
}

void MenuBase::select()
{
    _items[_currentItem]->select();
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

SubMenu::SubMenu(MenuBase* parent) :
    MenuBase(parent),
    _parent(parent)
{
    _items.push_back(new MenuItem("...Back", [this]() { _menu->_currentMenu = _parent; _currentItem = 0; }));
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