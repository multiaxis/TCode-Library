// TCode-Axis-Manager-CPP v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 07/07/2024
// Please copy, share, learn, innovate, give attribution.
#include "TAxisManager.h"

namespace TCode::Axis {
    void TCodeAxisManager::stop() {
        for (size_t i = 0; i < registeredAxes.size(); i++)
            registeredAxes[i]->stop();
    }

    void TCodeAxisManager::setAxisData(const Datatypes::AxisId &id, const Datatypes::AxisData &data) {
        TCodeAxis *axis = nullptr;
        if (getAxisFromId(id, axis)) {
            axis->set(data);
        }
    }

    const float TCodeAxisManager::getAxisPosition(const Datatypes::AxisId &id){
        TCodeAxis *axis = nullptr;
        if (getAxisFromId(id, axis)) {
            return axis->getPosition();
        }

        return NAN;
    }

    const unsigned long TCodeAxisManager::getAxisLastCommandTime(const Datatypes::AxisId &id){
        TCodeAxis *axis = nullptr;
        if (getAxisFromId(id, axis)) {
            return axis->getLastCommandTime();
        }

        return LONG_MAX;
    }

    const bool TCodeAxisManager::registerAxis(TCodeAxis *axis) {
        if (axis == nullptr)
            return false;

        if (hasAxisFromId(axis->getId()))
            return false;

        registeredAxes.push_back(axis);
        return true;
    }

    const bool TCodeAxisManager::hasAxisFromId(const Datatypes::AxisId &id){
        TCodeAxis *found = nullptr;
        return getAxisFromId(id, found);
    }

    const TCodeAxis *TCodeAxisManager::getAxisIndex(const size_t &index){
        if (index > registeredAxes.size())
            return nullptr;
        return registeredAxes[index];
    }

    const size_t TCodeAxisManager::count(){
        return registeredAxes.size();
    }

    const bool TCodeAxisManager::getAxisFromId(const Datatypes::AxisId &id, TCodeAxis *&axis){
        for (size_t i = 0; i < registeredAxes.size(); i++) {
            TCodeAxis *testAxis = registeredAxes[i];
            if (testAxis->getId() == id) {
                axis = testAxis;
                return true;
            }
        }

        return false;
    }

    const TCodeAxis *TCodeAxisManager::operator[](const size_t &index){
        return getAxisIndex(index);
    }

    TCodeAxisManager::TCodeAxisManager() {
    }

    TCodeAxisManager::~TCodeAxisManager() {
    }

};