// TCode-Tagged-Data-Container-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/02/2024
// Please copy, share, learn, innovate, give attribution.
#include "TaggedDataContainer.h"


namespace TCode {

TaggedDataContainer::TaggedDataContainer() {}



const char* TaggedDataContainer::getTag() {
    return tag;
}

unsigned int TaggedDataContainer::getHash() {
    return tagHash;
}

}

