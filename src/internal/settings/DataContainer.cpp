// TCode-Data-Container-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 24/02/2024
// Please copy, share, learn, innovate, give attribution.
#include "DataContainer.h"
#include "../utils/TString.h"

using namespace TCode;

bool DataContainer::toStringChar(char *buffer, size_t length)
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

bool DataContainer::toStringCharPointer(char *buffer, size_t length)
{
    const char *value;
    if(!getValue(value))
        return false;

    size_t valueLength = strlen(value);
    if (length < valueLength + 3) // add '\0' and two quotes'""'
        return false;
    buffer[0] = '\"';
    for (size_t i = 0; i < length; i++)
    {
        buffer[i+1] = value[i];
    }
    buffer[valueLength] = '\"';
    return true;
}

bool DataContainer::toStringBool(char *buffer, size_t length)
{
    if (length < 6)
        return false;

    const char *trueValue = "TRUE";
    const char *falseValue = "FALSE";
    bool value;
    if(!getValue(value))
        return false;

    if (value)
    {
        strcpy(buffer, trueValue);
    }
    else
    {
        strcpy(buffer ,falseValue);
    }

    return true;
}

bool DataContainer::toStringSignedNumber(char *buffer, size_t length)
{
    long value;
    switch(data.tag)
    {
        case VariantType::BOOL:
            {
            bool retrieved;
            if(!getValue(retrieved))
                return false;
            value = (long)retrieved;
            break;
            }
        case VariantType::INT:
            {
            int retrieved;
            if(!getValue(retrieved))
                return false;
            value = (long)retrieved;
            break;
            }
        case VariantType::LONG:
            {
            long retrieved;
            if(!getValue(retrieved))
                return false;
            value = (long)retrieved;
            break;
            }
        case VariantType::UINT:
            {
            unsigned int retrieved;
            if(!getValue(retrieved))
                return false;
            value = (long)retrieved;
            break;   
            }
        case VariantType::ULONG:
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

    unsigned long valueabs = abs(value);
    if (TString::uintToStrLen(valueabs) + 1 > length)
        return false;

    size_t index = 0;
    if (value < 0)
        buffer[index++] = '-';

    TString::uintToStr(valueabs, buffer, length, index);
    return true;
}

bool DataContainer::toStringUnsignedNumber(char *buffer, size_t length)
{
    unsigned long value;
    switch(data.tag)
    {
        case VariantType::BOOL:
            {
            bool retrieved;
            if(!getValue(retrieved))
                return false;
            value = (unsigned long)retrieved;
            break;
            }
        case VariantType::INT:
            {
            int retrieved;
            if(!getValue(retrieved))
                return false;
            value = (unsigned long)retrieved;
            break;
            }
        case VariantType::LONG:
            {
            long retrieved;
            if(!getValue(retrieved))
                return false;
            value = (unsigned long)retrieved;
            break;
            }
        case VariantType::UINT:
            {
            unsigned int retrieved;
            if(!getValue(retrieved))
                return false;
            value = (unsigned long)retrieved;
            break;   
            }
        case VariantType::ULONG:
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
    if (TString::uintToStrLen(valueabs) + 1 > length)
        return false;

    TString::uintToStr(valueabs, buffer, length, 0);
    return true;
}

bool DataContainer::toStringFloat(char *buffer, size_t length)
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

bool DataContainer::writeEmptyText(char *buffer, size_t length)
{
    const char *errorText = "EMPTY";
    if(length < 6)
        return false;
    strcpy(buffer, errorText);
    return true;
}

void DataContainer::setDataType(VariantType tag)
{
    data.tag = tag;
}

VariantType DataContainer::getDataType()
{
    return data.tag;
}

template <typename T>
bool DataContainer::getValue(T &value)
{
    static_assert((std::is_same<T, unsigned long>::value||std::is_same<T, unsigned int>::value||std::is_same<T, char>::value||std::is_same<T, const char *>::value||std::is_same<T, bool>::value||std::is_same<T, int>::value||std::is_same<T, long>::value||std::is_same<T, float>::value)==true,"Invalid Type Used, Only supports char,const char*,bool,int,long,uint,ulong,float");
    return data.get(value);
}

bool DataContainer::getValue(DataContainer &value)
{
    value.data.tag = data.tag;
    value.data.data = data.data;
    return true;
}

template <typename T>
void DataContainer::setValue(T &value)
{
    static_assert((std::is_same<T, unsigned long>::value||std::is_same<T, unsigned int>::value||std::is_same<T, char>::value||std::is_same<T, const char *>::value||std::is_same<T, bool>::value||std::is_same<T, int>::value||std::is_same<T, long>::value||std::is_same<T, float>::value)==true,"Invalid Type Used, Only supports char,const char*,bool,int,long,uint,ulong,float");
    data = value;
}

void DataContainer::setValue(DataContainer &value)
{
    data.tag = value.getDataType();
    data.data = value.getUnderlyingType().data;
}

variant_t &DataContainer::getUnderlyingType()
{
    return data;
}

bool DataContainer::toString(char *buffer, size_t length)
{
    bool valid = false;
    switch (getDataType())
    {
    case VariantType::CHAR:
        valid = toStringChar(buffer, length);
        break;
    case VariantType::CHARPOINTER:
        valid = toStringCharPointer(buffer, length);
        break;
    case VariantType::BOOL:
        valid = toStringBool(buffer, length);
        break;
    case VariantType::INT:
        valid = toStringSignedNumber(buffer, length);
        break;
    case VariantType::LONG:
        valid = toStringSignedNumber(buffer, length);
        break;
    case VariantType::UINT:
        valid = toStringUnsignedNumber(buffer, length);
        break;
    case VariantType::ULONG:
        valid = toStringUnsignedNumber(buffer, length);
        break;
    case VariantType::FLOAT:
        valid = toStringFloat(buffer, length);
        break;
    case VariantType::EMPTY:
        valid = writeEmptyText(buffer, length);
        break;
    }
    return valid;
}
