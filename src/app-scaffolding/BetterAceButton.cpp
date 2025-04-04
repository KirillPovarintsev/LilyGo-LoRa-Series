#include "BetterAceButton.h"

void onButtonEvent(ace_button::AceButton* button, uint8_t eventType, uint8_t buttonState)
{
    switch (eventType)
    {
    case ace_button::AceButton::kEventPressed:
        ((BetterAceButton*)button)->onButtonDown();
        break;
    case ace_button::AceButton::kEventReleased:
        ((BetterAceButton*)button)->onButtonUp();
        break;
    case ace_button::AceButton::kEventClicked:
        ((BetterAceButton*)button)->onButtonClick();
        break;
    case ace_button::AceButton::kEventLongPressed:
        ((BetterAceButton*)button)->onButtonLongPress();
        break;
    }
}

BetterAceButton::BetterAceButton(uint8_t pin,
                                 std::function<void()> onButtonDown,
                                 std::function<void()> onButtonUp,
                                 std::function<void()> onButtonClick,
                                 std::function<void()> onButtonLongPress) :
    ace_button::AceButton(pin),
    _onButtonDown(onButtonDown),
    _onButtonUp(onButtonUp),
    _onButtonClick(onButtonClick),
    _onButtonLongPress(onButtonLongPress)
{
    pinMode(pin, INPUT_PULLUP);

    init(pin);

    ace_button::ButtonConfig* buttonConfig = getButtonConfig();

    buttonConfig->setEventHandler(onButtonEvent);
    buttonConfig->setFeature(ace_button::ButtonConfig::kFeatureClick);
    buttonConfig->setFeature(ace_button::ButtonConfig::kFeatureLongPress);

}

void BetterAceButton::onButtonDown()
{
    _onButtonDown();
}

void BetterAceButton::onButtonUp()
{
    _onButtonUp();
}

void BetterAceButton::onButtonClick()
{
    _onButtonClick();
}

void BetterAceButton::onButtonLongPress()
{
    _onButtonLongPress();
}

