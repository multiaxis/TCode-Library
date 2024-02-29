// TCode-Tagged-Data-Container-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once
#ifndef TCODE_TAGGED_DATA_CONTAINER_H
#define TCODE_TAGGED_DATA_CONTAINER_H
#include <string>
#include "TCodeDataContainer.h"

//implementation of djb2 from http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash(const char *str)
{
    unsigned long hash = 5381;
    char c;
    for(int i = 0, c = str[i]; c != '\0'; i++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

class TCodeTaggedDataContainer
{
private:
    unsigned int taghash;
    const char* tag;
    TCodeDataContainer data;
public:
    TCodeTaggedDataContainer(const char * _tag, TCodeDataContainer _data) : tag(_tag),data(_data)
    {
        taghash = hash(tag);
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

    bool operator==(const TCodeTaggedDataContainer& LHS) const
    {
        if(LHS.taghash != taghash)
            return false;
        
        if(strcmp(LHS.tag,tag) != 0)
            return false;

        return true;
    }
};

#endif