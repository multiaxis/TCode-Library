// TCode-Button-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 13/11/2023
// Please copy, share, learn, innovate, give attribution.
#include "TButton.h"

using namespace TCode;

TButton::TButton(unsigned int pin, const char* name, void (*callback)())
 : name(name), pin(pin), callback(callback)
{
    pinMode(pin, INPUT);
    isPressed = digitalRead(pin);
    lastState = isPressed;
}

void TButton::update(TCodeManager context)
{
    //TODO debounce?
    unsigned long currentTime = millis();
    if (currentTime < lastPressTime + TBUTTON_TIMEOUT)
        return;
    
    isPressed = digitalRead(pin);

    bool changed = lastState ^ isPressed;
    if (changed && !lastState)
    {
        lastPressTime = currentTime;
        if (callback != nullptr)
            callback();
        
        //TODO: println true
    }

    if (changed && lastState)
    {
        //TODO: println false
    }

    lastState = isPressed;
}
