#pragma once

namespace TCode {

enum class VariantType
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

struct variant_t
{
    VariantType tag = VariantType::EMPTY;
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
    variant_t &operator=(const T &value) = delete;

    template <typename T>
    T &operator=(const variant_t &value) = delete;

    variant_t &operator=(char &value)
    {
        data.dataChar = value;
        tag = VariantType::CHAR;
        return *this;
    }

    variant_t &operator=(const char *&value)
    {
        data.dataCharP = value;
        tag = VariantType::CHARPOINTER;
        return *this;
    }

    variant_t &operator=(bool &value)
    {
        data.dataBool = value;
        tag = VariantType::BOOL;
        return *this;
    }

    variant_t &operator=(int &value)
    {
        data.dataInt = value;
        tag = VariantType::INT;
        return *this;
    }

    variant_t &operator=(long &value)
    {
        data.dataLong = value;
        tag = VariantType::LONG;
        return *this;
    }

    variant_t &operator=(float &value)
    {
        data.dataFloat = value;
        tag = VariantType::FLOAT;
        return *this;
    }

    variant_t &operator=(unsigned int &value)
    {
        data.dataUInt = value;
        tag = VariantType::UINT;
        return *this;
    }

    variant_t &operator=(unsigned long &value)
    {
        data.dataULong = value;
        tag = VariantType::ULONG;
        return *this;
    }

    template <typename T>
    bool get(T &value) = delete;

    bool get(char &value)
    {
        if (tag != VariantType::CHAR)
            return false;

        value = data.dataChar;
        return true;
    }

    bool get(const char *&value)
    {
        if (tag != VariantType::CHARPOINTER)
            return false;

        value = data.dataCharP;
        return true;
    }

    bool get(bool &value)
    {
        if (tag != VariantType::BOOL)
            return false;

        value = data.dataBool;
        return true;
    }

    bool get(int &value)
    {
        if (tag != VariantType::INT)
            return false;

        value = data.dataInt;
        return true;
    }

    bool get(long &value)
    {
        if (tag != VariantType::LONG)
            return false;

        value = data.dataLong;
        return true;
    }

    bool get(unsigned int &value)
    {
        if (tag != VariantType::UINT)
            return false;

        value = data.dataUInt;
        return true;
    }

    bool get(unsigned long &value)
    {
        if (tag != VariantType::ULONG)
            return false;

        value = data.dataULong;
        return true;
    }

    bool get(float &value)
    {
        if (tag != VariantType::FLOAT)
            return false;

        value = data.dataFloat;
        return true;
    }
};

}
