// TCode-Arduino-Print-Output-Stream-Interface-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 16/08/2023
// Please copy, share, learn, innovate, give attribution.
//
#pragma once
#include <TCode.h>
#include <Arduino.h>

using namespace TCode;
using namespace TCode::Output;

class TCodeArduinoPrintWrapper : public TCodeIOutputStream {
    Print* print_ptr;
public:
    TCodeArduinoPrintWrapper(Print* value)
    {
        print_ptr = value;
    }

    void write(const char value) const { if(print_ptr != nullptr) print_ptr->write(value); }
    void write(const char *value) const { if(print_ptr != nullptr) print_ptr->write(value); }
    void write(const __FlashStringHelper *value) const { if(print_ptr != nullptr) print_ptr->print(value); }
    void write(const String &value) const { if(print_ptr != nullptr) print_ptr->print(value); }

    void print(const char value) const { if(print_ptr != nullptr) print_ptr->print(value); }
    void print(const char *value) const { if(print_ptr != nullptr) print_ptr->print(value); }
    void print(const __FlashStringHelper *value) const { if(print_ptr != nullptr) print_ptr->print(value); }
    void print(const String &value) const { if(print_ptr != nullptr) print_ptr->print(value); }

    void println(const char value) const { if(print_ptr != nullptr) print_ptr->println(value); }
    void println(const char *value) const { if(print_ptr != nullptr) print_ptr->println(value); }
    void println(const __FlashStringHelper *value) const { if(print_ptr != nullptr) print_ptr->println(value); }
    void println(const String &value) const { if(print_ptr != nullptr) print_ptr->println(value); }
};

