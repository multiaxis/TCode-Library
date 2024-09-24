// TCode-Axis-Manager-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 07/07/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include "TAxis.h"
#include <Arduino.h>
#include <vector>

namespace TCode::Axis {
    class TCodeAxisManager {
    private:
        std::vector<TCodeAxis *> registeredAxes;

    public:
        void stop();

        void setAxisData(const Datatypes::AxisId &id, const Datatypes::AxisData &data);
        float getAxisPosition(const Datatypes::AxisId &id);
        unsigned long getAxisLastCommandTime(const Datatypes::AxisId &id);

        bool registerAxis(TCodeAxis *axis);
        bool getAxisFromId(const Datatypes::AxisId &id, TCodeAxis *&axis);
        bool hasAxisFromId(const Datatypes::AxisId &id);

        TCodeAxis *getAxisIndex(const size_t &index);
        size_t count();

        TCodeAxisManager();
        ~TCodeAxisManager();
    };
};