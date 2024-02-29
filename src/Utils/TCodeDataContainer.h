// TCode-Data-Container-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 24/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once
#ifndef TCODE_DATA_CONTAINER_H
#define TCODE_DATA_CONTAINER_H
#include <string.h>
#include <type_traits>
#include "TCodeCStringUtils.h"

enum class TCodeInterfaceDataTag
{
    CHAR,
    CHARPOINTER,
    BOOL,
    INT,
    LONG,
    UINT,
    ULONG,
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
        unsigned int dataUInt;
        unsigned long dataULong;
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

    TCodeInterfaceVariant_t &operator=(unsigned int &value)
    {
        data.dataUInt = value;
        tag = TCodeInterfaceDataTag::UINT;
        return *this;
    }

    TCodeInterfaceVariant_t &operator=(unsigned long &value)
    {
        data.dataULong = value;
        tag = TCodeInterfaceDataTag::ULONG;
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

    bool get(unsigned int &value)
    {
        if (tag != TCodeInterfaceDataTag::UINT)
            return false;

        value = data.dataUInt;
        return true;
    }

    bool get(unsigned long &value)
    {
        if (tag != TCodeInterfaceDataTag::ULONG)
            return false;

        value = data.dataULong;
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

    bool toStringSignedNumber(char *buffer, size_t length)
    {
        long value;
        switch(data.tag)
        {
            case TCodeInterfaceDataTag::BOOL:
                {
                bool retrieved;
                if(!getValue(retrieved))
                    return false;
                value = (long)retrieved;
                break;
                }
            case TCodeInterfaceDataTag::INT:
                {
                int retrieved;
                if(!getValue(retrieved))
                    return false;
                value = (long)retrieved;
                break;
                }
            case TCodeInterfaceDataTag::LONG:
                {
                long retrieved;
                if(!getValue(retrieved))
                    return false;
                value = (long)retrieved;
                break;
                }
            case TCodeInterfaceDataTag::UINT:
                {
                unsigned int retrieved;
                if(!getValue(retrieved))
                    return false;
                value = (long)retrieved;
                break;   
                }
            case TCodeInterfaceDataTag::ULONG:
                {
                unsigned long retrieved;
                if(!getValue(retrieved))
                    return false;
                value = (long)retrieved;
                break;   
                }
            default:
                return false;
        }

        unsigned long valueabs;
        if(value < 0)
            valueabs = -value;
        else
            valueabs = value;

        if (TCodeCStringUtils::uintToStrLen(valueabs) + 1 > length)
            return false;
        size_t index = 0;
        if(value < 0)
            buffer[index++] = '-';
        TCodeCStringUtils::uintToStr(valueabs, (unsigned char*)buffer, length, index);
        return true;
    }

    bool toStringUnsignedNumber(char *buffer, size_t length)
    {
        unsigned long value;
        switch(data.tag)
        {
            case TCodeInterfaceDataTag::BOOL:
                {
                bool retrieved;
                if(!getValue(retrieved))
                    return false;
                value = (unsigned long)retrieved;
                break;
                }
            case TCodeInterfaceDataTag::INT:
                {
                int retrieved;
                if(!getValue(retrieved))
                    return false;
                value = (unsigned long)retrieved;
                break;
                }
            case TCodeInterfaceDataTag::LONG:
                {
                long retrieved;
                if(!getValue(retrieved))
                    return false;
                value = (unsigned long)retrieved;
                break;
                }
            case TCodeInterfaceDataTag::UINT:
                {
                unsigned int retrieved;
                if(!getValue(retrieved))
                    return false;
                value = (unsigned long)retrieved;
                break;   
                }
            case TCodeInterfaceDataTag::ULONG:
                {
                unsigned long retrieved;
                if(!getValue(retrieved))
                    return false;
                value = (unsigned long)retrieved;
                break;   
                }
            default:
                return false;
        }

        unsigned long valueabs;
        if(value < 0)
            valueabs = -value;
        else
            valueabs = value;
        
        if (TCodeCStringUtils::uintToStrLen(valueabs) + 1 > length)
            return false;
        TCodeCStringUtils::uintToStr(valueabs, (unsigned char*)buffer, length, 0);
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
        static_assert((std::is_same<T, unsigned long>::value||std::is_same<T, unsigned int>::value||std::is_same<T, char>::value||std::is_same<T, const char *>::value||std::is_same<T, bool>::value||std::is_same<T, int>::value||std::is_same<T, long>::value||std::is_same<T, float>::value)==true,"Invalid Type Used, Only supports char,const char*,bool,int,long,uint,ulong,float");
        data = value;
    }

    TCodeInterfaceDataTag getDataType()
    {
        return data.tag;
    }

    template <typename T>
    bool getValue(T &value)
    {
        static_assert((std::is_same<T, unsigned long>::value||std::is_same<T, unsigned int>::value||std::is_same<T, char>::value||std::is_same<T, const char *>::value||std::is_same<T, bool>::value||std::is_same<T, int>::value||std::is_same<T, long>::value||std::is_same<T, float>::value)==true,"Invalid Type Used, Only supports char,const char*,bool,int,long,uint,ulong,float");
        return data.get(value);
    }

    template <typename T>
    void setValue(T &value)
    {
        static_assert((std::is_same<T, unsigned long>::value||std::is_same<T, unsigned int>::value||std::is_same<T, char>::value||std::is_same<T, const char *>::value||std::is_same<T, bool>::value||std::is_same<T, int>::value||std::is_same<T, long>::value||std::is_same<T, float>::value)==true,"Invalid Type Used, Only supports char,const char*,bool,int,long,uint,ulong,float");
        data = value;
    }

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
            valid = toStringSignedNumber(buffer, length);
            break;
        case TCodeInterfaceDataTag::LONG:
            valid = toStringSignedNumber(buffer, length);
            break;
        case TCodeInterfaceDataTag::UINT:
            valid = toStringUnsignedNumber(buffer, length);
            break;
        case TCodeInterfaceDataTag::ULONG:
            valid = toStringUnsignedNumber(buffer, length);
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

#endif