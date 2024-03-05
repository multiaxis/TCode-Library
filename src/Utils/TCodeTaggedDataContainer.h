// TCode-Tagged-Data-Container-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once
#ifndef TCODE_TAGGED_DATA_CONTAINER_H
#define TCODE_TAGGED_DATA_CONTAINER_H
#include <string>
#include <Arduino.h>
#include "TCodeDataContainer.h"

//implementation of djb2 from http://www.cse.yorku.ca/~oz/hash.html
static unsigned long str2hash(const char *str,size_t length)
{
    unsigned long hash = 5381;
    char c;
    for(int i = 0, c = str[i]; c != '\0', i < length; i++)
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

class TCodeTaggedDataContainer
{
private:
    unsigned int taghash;
    String tag;
    TCodeDataContainer data;

    

public:

    TCodeTaggedDataContainer() {}

    TCodeTaggedDataContainer(const char * _tag, TCodeDataContainer _data) : tag(_tag),data(_data)
    {
        taghash = str2hash(tag.c_str(),tag.length());
    }

    TCodeDataContainer* getDataContainer()
    {
        return &data;
    }

    template<typename T>
    void setValue(T value)
    {
        data.setValue<T>(value);
    }

    template<typename T>
    bool getValue(T& value)
    {
        return data.getValue<T>(value);
    }

    TCodeInterfaceDataTag getDataType()
    {
        return data.getDataType();
    }

    const char* getTag()
    {
        return tag.c_str();
    }

    unsigned int getHash()
    {
        return taghash;
    }

    bool operator==(const TCodeTaggedDataContainer& LHS) const
    {
        if(LHS.taghash != taghash)
            return false;
        
        if(LHS.tag == tag)
            return false;

        return true;
    }
};

#endif