#ifndef BETTERACEBUTTON_H
#define BETTERACEBUTTON_H

#include <AceButton.h>

class BetterAceButton : public ace_button::AceButton
{
public:
    BetterAceButton(uint8_t pin,
                    std::function<void()> onButtonDown,
                    std::function<void()> onButtonUp,
                    std::function<void()> onButtonClick,
                    std::function<void()> onButtonLongPress);

public:
    void onButtonDown();
    void onButtonUp();
    void onButtonClick();
    void onButtonLongPress();

private:
    std::function<void()> _onButtonDown;
    std::function<void()> _onButtonUp;
    std::function<void()> _onButtonClick;
    std::function<void()> _onButtonLongPress;
};

#endif //BETTERACEBUTTON_H