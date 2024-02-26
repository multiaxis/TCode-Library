// TCode-External-Interface-Class v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 24/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once
#include <variant>
#include <string>
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
}

using TCodeInterfaceVariant_t = std::variant<char,const char*,bool,int,long,float>;


class TCodeDataContainer
{
private:
    TCodeInterfaceVariant_t data;
    

    template<typename T>
    bool toStringTemplated(char* buffer,size_t length) = delete;

    template<>
    bool toStringTemplated<char>(char* buffer,size_t length)
    {
        if(length < 2)
            return false;
        
        buffer[0] = getValue<char>();
        buffer[1] = '\0';
        return true;
    }

    template<>
    bool toStringTemplated<const char*>(char* buffer,size_t length)
    {
        const char* value = getValue<const char*>();
        size_t value_length = strlen(value);
        if(length < value_length+1)
            return false;
        
        for(size_t i = 0; i < length; i++)
        {
            buffer[i] = value[i];
        }    
        return true;
    }

    template<>
    bool toStringTemplated<bool>(char* buffer,size_t length)
    {
        if(length < 6)
            return false;
        
        const char* true_value = "TRUE";
        const char* false_value = "FALSE";

        if(getValue<bool>())
        {
            strcpy(true_value,buffer);
        }
        else
        {
            strcpy(false_value,buffer);
        }

        return true;
    }

    template<>
    bool toStringTemplated<int>(char* buffer,size_t length)
    {
        int value = getValue<int>();
        if(TCodeParser::uintToStrLen(value)+1 > length)
            return false;
        TCodeParser::uinttostr(value,buffer,length,0);
        return true;
    }

    template<>
    bool toStringTemplated<long>(char* buffer,size_t length)
    {
        long value = getValue<long>();
        if(TCodeParser::uintToStrLen(value)+1 > length)
            return false;
        TCodeParser::uinttostr(value,buffer,length,0);
        return true;
    }

    template<>
    bool toStringTemplated<float>(char* buffer,size_t length)
    {
        float value = getValue<float>();
        if(snprintf(buffer,length-1,"%.4f",value) > length)
        {
            return false;
        }
        return true;
    }

    size_t writeEmptyText(char* buffer, size_t length)
    {
        const char* errorText = "EMPTY";
        return strncpy(buffer,errorText,length);
    }

public:
    TCodeInterfaceDataTag getDataType()
    {
        switch (data.index())
        {
        case const_cast<int>(TCodeInterfaceDataTag::CHAR): return TCodeInterfaceDataTag::CHAR;
        case const_cast<int>(TCodeInterfaceDataTag::CHARPOINTER): return TCodeInterfaceDataTag::CHARPOINTER;
        case const_cast<int>(TCodeInterfaceDataTag::BOOL): return TCodeInterfaceDataTag::BOOL;
        case const_cast<int>(TCodeInterfaceDataTag::INT): return TCodeInterfaceDataTag::INT;
        case const_cast<int>(TCodeInterfaceDataTag::LONG): return TCodeInterfaceDataTag::LONG;
        case const_cast<int>(TCodeInterfaceDataTag::FLOAT): return TCodeInterfaceDataTag::FLOAT;
        default:
            return TCodeInterfaceDataTag::EMPTY;
        }
    }


    template<typename T>
    T getValue() = delete;

    template<>
    char getValue<char>() {return std::get(data);}
    template<>
    const char* getValue<const char*>() {return std::get(data);}
    template<>
    bool getValue<bool>() {return std::get(data);}
    template<>
    int getValue<int>() {return std::get(data);}
    template<>
    int getValue<long>() {return std::get(data);}
    template<>
    float getValue<float>() {return std::get(data);}
    
    template<typename T>
    void setValue(T value) = delete;

    template<>
    void setValue<char>(char value) { data = value; }
    template<>
    void setValue<const char*>(const char* value) {data = value; }
    template<>
    void setValue<bool>(bool value) {data = value;}
    template<>
    void setValue<int>(int value) {data = value; }
    template<>
    void setValue<long>(long value) {data = value; }
    template<>
    void setValue<float>(float value) {data = value; }


    bool toString(char* buffer, size_t length)
    {
        bool valid = false;
        switch (getDataType())
        {
            case TCodeInterfaceDataTag::CHAR: valid = toStringTemplated<char>(buffer,length); break;
            case TCodeInterfaceDataTag::CHARPOINTER: valid = toStringTemplated<const char*>(buffer,length); break;
            case TCodeInterfaceDataTag::BOOL: valid = toStringTemplated<bool>(buffer,length); break;
            case TCodeInterfaceDataTag::INT: valid = toStringTemplated<int>(buffer,length); break;
            case TCodeInterfaceDataTag::LONG: valid = toStringTemplated<long>(buffer,length); break;
            case TCodeInterfaceDataTag::FLOAT: valid = toStringTemplated<float>(buffer,length); break;
            case TCodeInterfaceDataTag::EMPTY: valid = (writeEmptyText(buffer,length) >= length); break;
        }
        return valid;
    }
};