// TCode-Button-Class-h v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 13/11/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once
#ifndef TCODE_BUTTON_H
#define TCODE_BUTTON_H
#include <Arduino.h>

#define DEFAULT_TIMEOUT 60
class TCodeButton;

using t_Button_Callback = void (*)(TCodeButton *);

class TCodeButton
{
public:
    const char *name;
    TCodeButton() = delete;
    TCodeButton(unsigned int pin, t_Button_Callback t, const char* name);
    void setCallback(t_Button_Callback t);
    bool update();
    bool getState();

private:
    unsigned long lastPressTime = 0;
    unsigned long timeout = DEFAULT_TIMEOUT;
    unsigned int pin = 0;
    bool lastState;
    bool isPressed;
    t_Button_Callback callback = nullptr;
};


#endif