// TCode-Button-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 13/11/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <Arduino.h>
#include "TInputBase.h"

#define TBUTTON_TIMEOUT 60

namespace TCode {

class TButton : public TInputBase
{
public:
    TButton() = delete;
    TButton(unsigned int pin, const char* name, void (*callback)() = nullptr);

    void update(const TCodeManager &context) override;

private:
    char *printBuffer;
    size_t printBufferLength;
    unsigned int pin = 0;
    bool isPressed = false;
    bool lastState = false;
    unsigned long lastPressTime = 0;
    void (*callback)() = nullptr;
};

}
