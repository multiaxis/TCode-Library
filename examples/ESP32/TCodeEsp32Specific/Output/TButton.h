// TCode-Button-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 13/11/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <Arduino.h>
#include <TCode.h>

#define TBUTTON_TIMEOUT 30

using namespace TCode;
using namespace TCode::Module;

class TButton : public TCodeModuleBase {
public:
    TButton() = delete;
    TButton(unsigned int pin, const char *name, void (*callback)() = nullptr);

    void update(TCodeContext &context) override;

private:
    unsigned int pin = 0;
    bool isPressed = false;
    bool lastState = false;
    unsigned long lastPressTime = 0;
    const char *name;
    void (*callback)() = nullptr;
};