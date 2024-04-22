// TCode-Tagged-Data-Container-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <string>
#include <Arduino.h>

#include "DataContainer.h"

namespace TCode {

class TaggedDataContainer
{
public:
    TaggedDataContainer();
    TaggedDataContainer(const char *tag, DataContainer data);

    DataContainer* getDataContainer();

    template<typename T>
    void setValue(T value);

    template<typename T>
    bool getValue(T& value);

    VariantType getDataType();

    const char* getTag();

    unsigned int getHash();

    bool operator==(const TaggedDataContainer& LHS) const
    {
        if(LHS.taghash != taghash)
            return false;
        
        if(LHS.tag == tag)
            return false;

        return true;
    }

private:
    unsigned int taghash;
    String tag;
    DataContainer data;
};

}
