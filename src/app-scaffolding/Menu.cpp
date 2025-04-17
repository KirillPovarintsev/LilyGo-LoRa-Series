#include "Menu.h"

MenuItem::MenuItem(std::function<std::string()> textCallback, std::function<void()> action, SubMenu* subMenu = nullptr) :
    _textCallback(textCallback),
    _action(action),
    _subMenu(subMenu)
{
}

MenuItem::~MenuItem()
{
    if (_subMenu) delete _subMenu;
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
    return addItem([text]() { return std::string(text); }, action);
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
    auto item = new MenuItem([text]() { return std::string(text); }, [this, subMenu]() { _menu->_currentMenu = subMenu; }, subMenu);
    _items.insert(_items.begin() + newItemPosition(), item);
    return *subMenu;
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
    _items[_currentItem]->invokeAction();
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
    _items.push_back(new MenuItem([]() { return std::string("< Back"); }, [this]() { _menu->_currentMenu = _parent; _currentItem = 0; }));
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