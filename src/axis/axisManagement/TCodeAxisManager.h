// TCode-Axis-Manager-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 07/07/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include "../axisClass/TAxis.h"
#include <Arduino.h>
#include <vector>

namespace TCode {
    class TCodeAxisManager {
    private:
        std::vector<TCodeAxis *> registeredAxes;

    public:
        void stop();

        void setAxisData(const AxisId &id, const AxisData &data);
        float getAxisPosition(const AxisId &id);
        unsigned long getAxisLastCommandTime(const AxisId &id);

        bool registerAxis(TCodeAxis *axis);
        bool getAxisFromId(const AxisId &id, TCodeAxis *&axis);
        bool hasAxisFromId(const AxisId &id);

        TCodeAxis *getAxisIndex(const size_t &index);
        size_t count();

        TCodeAxisManager();
        ~TCodeAxisManager();
    };
};