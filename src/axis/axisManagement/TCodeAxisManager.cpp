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
        TCodeAxis *axis = nullptr;
        if (getAxisFromId(id,axis))
        {
            axis->set(data);
        }
    }

    float TCodeAxisManager::getAxisPosition(const AxisId &id)
    {
        TCodeAxis *axis = nullptr;
        if (getAxisFromId(id,axis))
        {
            return axis->getPosition();
        }   
        
        return NAN;
    }

    unsigned long TCodeAxisManager::getAxisLastCommandTime(const AxisId &id)
    {
        TCodeAxis *axis = nullptr;
        if (getAxisFromId(id,axis))
        {
            return axis->getLastCommandTime();
        }

        return LONG_MAX;
    }

    bool TCodeAxisManager::registerAxis(TCodeAxis *axis)
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
        TCodeAxis *found = nullptr;
        return getAxisFromId(id, found);
    }

    TCodeAxis *TCodeAxisManager::getAxisIndex(const size_t &index)
    {
        if(index > registeredAxes.size())
            return nullptr;
        return registeredAxes[index];
    }

    size_t TCodeAxisManager::count()
    {
        return registeredAxes.size();
    }

    bool TCodeAxisManager::getAxisFromId(const AxisId &id, TCodeAxis *&axis)
    {
        for (size_t i = 0; i < registeredAxes.size(); i++)
        {
            TCodeAxis *testAxis = registeredAxes[i];
            if (testAxis->getId() == id)
            {
                axis = testAxis;
                return true;
            }
        }

        return false;
    }

    TCodeAxisManager::TCodeAxisManager()
    {
    }

    TCodeAxisManager::~TCodeAxisManager()
    {
    }

};