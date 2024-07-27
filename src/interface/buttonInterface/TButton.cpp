// TCode-Button-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 13/11/2023
// Please copy, share, learn, innovate, give attribution.
#include "TButton.h"
#include "../../context/TCodeContext.h"
#include "../../utils/TString.h"

namespace TCode {

    TButton::TButton(unsigned int pin, const char *name, void (*callback)()) {
        this->pin = pin;
        this->callback = callback;
        this->name = name;

        pinMode(pin, INPUT);
        isPressed = digitalRead(pin);
        lastState = isPressed;
    }

    void TButton::update(TCodeContext &context) {
        unsigned long currentTime = millis();
        if (currentTime < lastPressTime + TBUTTON_TIMEOUT)
            return;

        isPressed = digitalRead(pin);

        bool changed = lastState ^ isPressed;
        if (changed) {
            if (!lastState) {
                lastPressTime = currentTime;
                if (callback != nullptr)
                    callback();
            }
            TEncoding::writeTCodeValue(name, isPressed, context);
        }

        lastState = isPressed;
    }

};