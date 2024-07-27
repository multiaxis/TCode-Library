// TCode-Button-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 13/11/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include "../interfaceBaseClass/TInterfaceBase.h"
#include "../../events/DataWriter/TCodeDataWriter.h"
#include <Arduino.h>

#define TBUTTON_TIMEOUT 60

namespace TCode {

    class TButton : public TInterfaceBase {
    public:
        TButton() = delete;
        TButton(unsigned int pin, const char *name, void (*callback)() = nullptr);

        void update(TCodeContext &context) override;

    private:
        unsigned int pin = 0;
        bool isPressed = false;
        bool lastState = false;
        unsigned long lastPressTime = 0;
        const char* name;
        void (*callback)() = nullptr;
    };

}
