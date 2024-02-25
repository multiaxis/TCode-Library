#include "Button.h"


TCodeButton::TCodeButton(unsigned int _pin, t_Button_Callback t, const char* _name) : name(_name),pin(_pin)
{
    pinMode(_pin, INPUT);
    setCallback(t);
}

void TCodeButton::setCallback(t_Button_Callback _callback)
{
    if (_callback != nullptr)
        callback = _callback;
}

bool TCodeButton::update()
{
    bool triggered = false;
    unsigned long current_time = millis();
    if (current_time > timeout + lastPressTime)
    {
        isPressed = digitalRead(pin);

        if(lastState ^ isPressed == true)
        {
            triggered = true;
        }

        if ((lastState ^ isPressed == true) && (lastState == false))
        {
            lastPressTime = current_time;
            if (callback != nullptr)
                callback(this);
        }
        lastState = isPressed;
    }
    return triggered;
}
