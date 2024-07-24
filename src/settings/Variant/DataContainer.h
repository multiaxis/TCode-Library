// TCode-Data-Container-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 24/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <type_traits>
#include <string>
#include "Variant.h"

namespace TCode {

class DataContainer {
public:
    DataContainer();
    
    template<typename T>
    DataContainer(T value);

    VariantType getDataType();

    template <typename T>
    bool getValue(T &value);

    bool getValue(DataContainer &value);

    template <typename T>
    void setValue(const T &value);

    void setValue(const DataContainer &value);

    template<typename T>
    DataContainer &operator=(T &value);

private:
    variant_t data;
};

template<typename T>
DataContainer &DataContainer::operator=(T &value) {
    setValue(value);
    return *this;
}

template <typename T>
void DataContainer::setValue(const T &value) {
    data = value;
}

template <typename T>
bool DataContainer::getValue(T &value) {
    return data.get(value);
}

template<typename T>
DataContainer::DataContainer(T value) {
    data = value;
}

}