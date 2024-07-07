// TCode-Axis-Manager-CPP v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 07/07/2024
// Please copy, share, learn, innovate, give attribution.
#include "TCodeAxisManager.h"

namespace TCode
{
    void TCodeAxisManager::stop()
    {
        for (size_t i = 0; i < registeredAxes.size(); i++)
            registeredAxes[i]->stop();
    }

    void TCodeAxisManager::setAxisData(const AxisId &id, const AxisData &data)
    {
        TAxis *axis = nullptr;
        if (getAxisFromId(id,axis))
        {
            axis->set(data);
        }
    }

    float TCodeAxisManager::getAxisPosition(const AxisId &id)
    {
        TAxis *axis = nullptr;
        if (getAxisFromId(id,axis))
        {
            return axis->getPosition();
        }   
        
        return NAN;
    }

    unsigned long TCodeAxisManager::getAxisLastCommandTime(const AxisId &id)
    {
        TAxis *axis = nullptr;
        if (getAxisFromId(id,axis))
        {
            return axis->getLastCommandTime();
        }

        return LONG_MAX;
    }

    bool TCodeAxisManager::registerAxis(TAxis *axis)
    {
        if (axis == nullptr)
            return false;

        if (hasAxisFromId(axis->getId()))
            return false;

        registeredAxes.push_back(axis);
        return true;
    }

    bool TCodeAxisManager::hasAxisFromId(const AxisId &id)
    {
        TAxis *found = nullptr;
        return getAxisFromId(id, found);
    }

    bool TCodeAxisManager::getAxisFromId(const AxisId &id, TAxis *&axis)
    {
        for (size_t i = 0; i < registeredAxes.size(); i++)
        {
            TAxis *testAxis = registeredAxes[i];
            if (testAxis->getId() == id)
            {
                axis = testAxis;
                return true;
            }
        }

        return false;
    }

    TCodeAxisManager::~TCodeAxisManager()
    {
    }

};