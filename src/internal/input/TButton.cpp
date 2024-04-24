// TCode-Button-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 13/11/2023
// Please copy, share, learn, innovate, give attribution.
#include "TButton.h"
#include "../utils/TString.h"

using namespace TCode;

TButton::TButton(unsigned int pin, const char* name, void (*callback)()) {
    this->pin = pin;
    this->callback = callback;
    
    pinMode(pin, INPUT);
    isPressed = digitalRead(pin);
    lastState = isPressed;

    printBufferLength = strlen(name) + 4;
    printBuffer = new char[printBufferLength];
    
    size_t index = 0;
    TString::writeChar('#', index, printBuffer, printBufferLength);
    TString::writeString(name, index, printBuffer, printBufferLength);
    TString::writeChar(':', index, printBuffer, printBufferLength);
    TString::writeInt((int)isPressed, index, printBuffer, printBufferLength);
    TString::writeChar('\n', index, printBuffer, printBufferLength);
}

void TButton::update(const TCodeManager &context) {
    //TODO debounce?
    unsigned long currentTime = millis();
    if (currentTime < lastPressTime + TBUTTON_TIMEOUT)
        return;
    
    isPressed = digitalRead(pin);

    bool changed = lastState ^ isPressed;
    if (changed && !lastState) {
        lastPressTime = currentTime;
        if (callback != nullptr)
            callback();
        
        TString::writeInt((int)isPressed, printBufferLength - 2, printBuffer, printBufferLength);
        context.writeLine(printBuffer);
    }

    if (changed && lastState) {
        TString::writeInt((int)isPressed, printBufferLength - 2, printBuffer, printBufferLength);
        context.writeLine(printBuffer);
    }

    lastState = isPressed;
}
