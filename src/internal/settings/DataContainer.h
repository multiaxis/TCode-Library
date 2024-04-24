// TCode-Data-Container-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 24/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <string>
#include <type_traits>

#include "Variant.h"

namespace TCode {

class DataContainer
{
public:
    DataContainer() {}

    template<typename T>
    DataContainer(T value)
    {
        static_assert((std::is_same<T, unsigned long>::value||std::is_same<T, unsigned int>::value||std::is_same<T, char>::value||std::is_same<T, const char *>::value||std::is_same<T, bool>::value||std::is_same<T, int>::value||std::is_same<T, long>::value||std::is_same<T, float>::value)==true,"Invalid Type Used, Only supports char,const char*,bool,int,long,uint,ulong,float");
        data = value;
    }

    VariantType getDataType();

    template <typename T>
    bool getValue(T &value);

    bool getValue(DataContainer &value);

    template <typename T>
    void setValue(T &value);

    void setValue(DataContainer &value);

    variant_t &getUnderlyingType();

    bool toString(size_t &index, char *buffer, size_t length);

    template<typename T>
    DataContainer &operator=(T &value)
    {
        static_assert((std::is_same<T, DataContainer>::value||std::is_same<T, unsigned long>::value||std::is_same<T, unsigned int>::value||std::is_same<T, char>::value||std::is_same<T, const char *>::value||std::is_same<T, bool>::value||std::is_same<T, int>::value||std::is_same<T, long>::value||std::is_same<T, float>::value)==true,"Invalid Type Used, Only supports char,const char*,bool,int,long,uint,ulong,float");
        setValue(value);
        return *this;
    }

private:
    variant_t data;

    bool toStringChar(size_t &index, char *buffer, size_t length);
    bool toStringCharPointer(size_t &index, char *buffer, size_t length);
    bool toStringBool(size_t &index, char *buffer, size_t length);
    bool toStringSignedNumber(size_t &index, char *buffer, size_t length);
    bool toStringUnsignedNumber(size_t &index, char *buffer, size_t length);
    bool toStringFloat(size_t &index, char *buffer, size_t length);
    bool writeEmptyText(size_t &index, char *buffer, size_t length);
    
    void setDataType(VariantType tag);
};

}
