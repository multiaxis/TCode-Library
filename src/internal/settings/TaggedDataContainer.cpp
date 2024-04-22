// TCode-Tagged-Data-Container-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/02/2024
// Please copy, share, learn, innovate, give attribution.
#include "TaggedDataContainer.h"
#include "../utils/TString.h"

namespace TCode {

TaggedDataContainer::TaggedDataContainer() {}

TaggedDataContainer::TaggedDataContainer(const char *tag, DataContainer data)
{
    this->tag = tag;
    this->data = data;
    
    taghash = TString::getHash(this->tag.c_str(), this->tag.length());
}

DataContainer* TaggedDataContainer::getDataContainer()
{
    return &data;
}

template<typename T>
void TaggedDataContainer::setValue(T value)
{
    data.setValue<T>(value);
}

template<typename T>
bool TaggedDataContainer::getValue(T& value)
{
    return data.getValue<T>(value);
}

VariantType TaggedDataContainer::getDataType()
{
    return data.getDataType();
}

const char* TaggedDataContainer::getTag()
{
    return tag.c_str();
}

unsigned int TaggedDataContainer::getHash()
{
    return taghash;
}

}
