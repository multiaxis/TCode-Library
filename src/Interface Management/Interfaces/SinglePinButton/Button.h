// TCode-Button-Class-h v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 13/11/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once
#ifndef TCODE_BUTTON_H
#define TCODE_BUTTON_H
#include <Arduino.h>
#include "../IExternalInterface.h"

#define DEFAULT_TIMEOUT 60
class TCodeButton;

using t_Button_Callback = void (*)(TCodeButton *);

class TCodeButton : public IExternalInterface
{
public:
    const char *name;
    TCodeButton() = delete;
    TCodeButton(unsigned int pin, const char* name);
    void setCallback(t_Button_Callback t);
    virtual void update(TCodeVariantSet& returnValues);
    bool getState();

private:
    unsigned long lastPressTime = 0;
    unsigned long timeout = DEFAULT_TIMEOUT;
    unsigned int pin = 0;
    bool lastState;
    bool isPressed;
    t_Button_Callback callback = nullptr;
};


TCodeButton::TCodeButton(unsigned int _pin, const char* _name) : name(_name),pin(_pin)
{
    pinMode(_pin, INPUT);
}

void TCodeButton::setCallback(t_Button_Callback _callback)
{
    if (_callback != nullptr)
        callback = _callback;
}

bool TCodeButton::update(TCodeVariantSet& returnValues)
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
            

            TCodeDataContainer data(true);
            TCodeTaggedDataContainer dataTagged(name,data);
            returnValues.append(dataTagged);
        }

        if ((lastState ^ isPressed == true) && (lastState == true))
        {
            TCodeDataContainer data(false);
            TCodeTaggedDataContainer dataTagged(name,data);
            returnValues.append(dataTagged);
        }

        lastState = isPressed;
    }
    return triggered;
}

#endif