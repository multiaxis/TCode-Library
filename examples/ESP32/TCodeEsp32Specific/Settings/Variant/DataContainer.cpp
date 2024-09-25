// TCode-Data-Container-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 24/02/2024
// Please copy, share, learn, innovate, give attribution.
#include "DataContainer.h"

    DataContainer::DataContainer() {}

    VariantType DataContainer::getDataType() {
        return data.type;
    }

    bool DataContainer::getValue(DataContainer &other) {
        other.data = data;
        return true;
    }

    void DataContainer::setValue(const DataContainer &other) {
        data = other.data;
    }
