// TCode-Data-Container-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 24/02/2024
// Please copy, share, learn, innovate, give attribution.
#include "DataContainer.h"
#include "../../utils/TString.h"

namespace TCode
{ 

DataContainer::DataContainer() {}

template<typename T>
DataContainer::DataContainer(T value) {
    data = value;
}

VariantType DataContainer::getDataType() {
    return data.type;
}

template <typename T>
bool DataContainer::getValue(T &value) {
    return data.get(value);
}

bool DataContainer::getValue(DataContainer &other) {
    other.data = data;
    return true;
}

template <typename T>
void DataContainer::setValue(const T &value) {
    data = value;
}

void DataContainer::setValue(const DataContainer &other) {
    data = other.data;
}

template<typename T>
DataContainer &DataContainer::operator=(T &value) {
    setValue(value);
    return *this;
}

}