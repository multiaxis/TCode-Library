// TCode-Tagged-Data-Container-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <string>
#include <Arduino.h>

#include "DataContainer.h"

namespace TCode {

class TaggedDataContainer : DataContainer {
public:
    TaggedDataContainer();
    
    template<typename T>
    TaggedDataContainer(const char *tag, T value);

    const char* getTag();

    unsigned int getHash();

    bool operator==(const TaggedDataContainer& o) const {
        return tagHash == o.tagHash;
    }

private:
    const char *tag;
    unsigned int tagHash;
};

}
