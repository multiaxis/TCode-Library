// TCode-External-Interface-Class v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 24/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once
#include <string.h>
#include <type_traits>
#include "../Parsing/TCodeParser.h"

enum class TCodeInterfaceDataTag
{
    CHAR,
    CHARPOINTER,
    BOOL,
    INT,
    LONG,
    FLOAT,
    EMPTY,
};

struct TCodeInterfaceVariant_t
{
    TCodeInterfaceDataTag tag = TCodeInterfaceDataTag::EMPTY;
    union DataValue
    {
        char dataChar;
        const char *dataCharP;
        bool dataBool;
        int dataInt;
        long dataLong;
        float dataFloat;
    } data;

    template <typename T>
    TCodeInterfaceVariant_t &operator=(const T &value) = delete;

    template <typename T>
    T &operator=(const TCodeInterfaceVariant_t &value) = delete;

    TCodeInterfaceVariant_t &operator=(char &value)
    {
        data.dataChar = value;
        tag = TCodeInterfaceDataTag::CHAR;
        return *this;
    }

    TCodeInterfaceVariant_t &operator=(const char *&value)
    {
        data.dataCharP = value;
        tag = TCodeInterfaceDataTag::CHARPOINTER;
        return *this;
    }

    TCodeInterfaceVariant_t &operator=(bool &value)
    {
        data.dataBool = value;
        tag = TCodeInterfaceDataTag::BOOL;
        return *this;
    }

    TCodeInterfaceVariant_t &operator=(int &value)
    {
        data.dataInt = value;
        tag = TCodeInterfaceDataTag::INT;
        return *this;
    }

    TCodeInterfaceVariant_t &operator=(long &value)
    {
        data.dataLong = value;
        tag = TCodeInterfaceDataTag::LONG;
        return *this;
    }

    TCodeInterfaceVariant_t &operator=(float &value)
    {
        data.dataFloat = value;
        tag = TCodeInterfaceDataTag::FLOAT;
        return *this;
    }

    template <typename T>
    bool get(T &value) = delete;

    bool get(char &value)
    {
        if (tag != TCodeInterfaceDataTag::CHAR)
            return false;

        value = data.dataChar;
        return true;
    }

    bool get(const char *&value)
    {
        if (tag != TCodeInterfaceDataTag::CHARPOINTER)
            return false;

        value = data.dataCharP;
        return true;
    }

    bool get(bool &value)
    {
        if (tag != TCodeInterfaceDataTag::BOOL)
            return false;

        value = data.dataBool;
        return true;
    }

    bool get(int &value)
    {
        if (tag != TCodeInterfaceDataTag::INT)
            return false;

        value = data.dataInt;
        return true;
    }

    bool get(long &value)
    {
        if (tag != TCodeInterfaceDataTag::LONG)
            return false;

        value = data.dataLong;
        return true;
    }

    bool get(float &value)
    {
        if (tag != TCodeInterfaceDataTag::FLOAT)
            return false;

        value = data.dataFloat;
        return true;
    }
};

class TCodeDataContainer
{
private:
    TCodeInterfaceVariant_t data;

    bool toStringChar(char *buffer, size_t length)
    {
        if (length < 2)
            return false;
        char value;

        if(!getValue(value))
            return false;

        buffer[0] = value;
        buffer[1] = '\0';
        return true;
    }

    bool toStringCharPointer(char *buffer, size_t length)
    {
        const char *value;
        if(!getValue(value))
            return false;

        size_t value_length = strlen(value);
        if (length < value_length + 3) // add '\0' and two quotes'""'
            return false;
        buffer[0] = '\"';
        for (size_t i = 0; i < length; i++)
        {
            buffer[i+1] = value[i];
        }
        buffer[value_length] = '\"';
        return true;
    }

    bool toStringBool(char *buffer, size_t length)
    {
        if (length < 6)
            return false;

        const char *true_value = "TRUE";
        const char *false_value = "FALSE";
        bool value;
        if(!getValue(value))
            return false;

        if (value)
        {
            strcpy(buffer, true_value);
        }
        else
        {
            strcpy(buffer ,false_value);
        }

        return true;
    }

    bool toStringInt(char *buffer, size_t length)
    {
        int value;
        if(!getValue(value))
            return false;
        if (TCodeParser::uintToStrLen(value) + 1 > length)
            return false;
        TCodeParser::uintToStr(value, (unsigned char*)buffer, length, 0);
        return true;
    }

    bool toStringLong(char *buffer, size_t length)
    {
        long value;
        if(!getValue(value))
            return false;
        if (TCodeParser::uintToStrLen(value) + 1 > length)
            return false;
        TCodeParser::uintToStr(value, (unsigned char*)buffer, length, 0);
        return true;
    }

    bool toStringFloat(char *buffer, size_t length)
    {
        float value;
        if(!getValue(value))
            return false;
        if (snprintf(buffer, length - 1, "%.4f", value) > length)
        {
            return false;
        }
        return true;
    }

    bool writeEmptyText(char *buffer, size_t length)
    {
        const char *errorText = "EMPTY";
        if(length < 6)
            return false;
        strcpy(buffer, errorText);
        return true;
    }

public:

    TCodeDataContainer()
    {}

    template<typename T>
    TCodeDataContainer(T value)
    {
        static_assert((std::is_same<T, char>||std::is_same<T, const char *>||std::is_same<T, bool>||std::is_same<T, int>||std::is_same<T, long>||std::is_same<T, float>)==false,"Invalid Type Used, Only supports char,const char*,bool,int,long,float");
        data = value;
    }

    TCodeInterfaceDataTag getDataType()
    {
        return data.tag;
    }

    template <typename T>
    bool getValue(T &value) = delete;
    bool getValue(char &value) { return data.get(value); }
    bool getValue(const char *&value) { return data.get(value); }
    bool getValue(bool &value) { return data.get(value); }
    bool getValue(int &value) { return data.get(value); }
    bool getValue(long &value) { return data.get(value); }
    bool getValue(float &value) { return data.get(value); }

    template <typename T>
    void setValue(T &value) = delete;
    void setValue(char value) { data = value; }
    void setValue(const char *value) { data = value; }
    void setValue(bool value) { data = value; }
    void setValue(int value) { data = value; }
    void setValue(long value) { data = value; }
    void setValue(float value) { data = value; }

    bool toString(char *buffer, size_t length)
    {
        bool valid = false;
        switch (getDataType())
        {
        case TCodeInterfaceDataTag::CHAR:
            valid = toStringChar(buffer, length);
            break;
        case TCodeInterfaceDataTag::CHARPOINTER:
            valid = toStringCharPointer(buffer, length);
            break;
        case TCodeInterfaceDataTag::BOOL:
            valid = toStringBool(buffer, length);
            break;
        case TCodeInterfaceDataTag::INT:
            valid = toStringInt(buffer, length);
            break;
        case TCodeInterfaceDataTag::LONG:
            valid = toStringLong(buffer, length);
            break;
        case TCodeInterfaceDataTag::FLOAT:
            valid = toStringFloat(buffer, length);
            break;
        case TCodeInterfaceDataTag::EMPTY:
            valid = writeEmptyText(buffer, length);
            break;
        }
        return valid;
    }
};
