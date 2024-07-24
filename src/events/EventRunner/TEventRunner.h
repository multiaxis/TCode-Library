// TCode-Event-Runner-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 07/07/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once
#include "../../datatypes/CommandDataTypes.h"
#include "../../datatypes/CommonDataTypes.h"
#include "../../datatypes/EnumTypes.h"
#include "../../context/TCodeContext.h"

namespace TCode::TEvents
{
    void runEvent(TCodeContext& context,const TCodeEvent &event);
    bool runAxisCommand(TCodeContext& context, const AxisCommandEvent &event);
    bool runDeviceCommand(TCodeContext& context, const DeviceCommandEvent &event);
    bool runSetupCommand(TCodeContext& context, const SetupCommandEvent &event);
}