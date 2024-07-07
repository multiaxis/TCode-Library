// TCode-Axis-Manager-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 07/07/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <Arduino.h>
#include <vector>
#include "../../internal/axis/TAxis.h"

namespace TCode
{
    class TCodeAxisManager
    {
    private:
        vector<TAxis*> registeredAxis;
    public:
        void stop();
        
        void setAxisData(const AxisId &id, const AxisData &data);
        float getAxisPosition(const AxisId &id);
        unsigned long getAxisLastCommandTime(const AxisId &id);

        bool registerAxis(TAxis *axis);
        bool getAxisFromId(const AxisId &id, TAxis* &axis);
        bool hasAxisFromId(const AxisId &id);

        TCodeAxisManager();
        ~TCodeAxisManager();
    };
};