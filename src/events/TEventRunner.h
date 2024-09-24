// TCode-Event-Runner-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 07/07/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once
#include "../context/TContext.h"
#include "../datatypes/CommandDataTypes.h"
#include "../datatypes/CommonDataTypes.h"
#include "../datatypes/EnumTypes.h"
#include "../outputstream/TOutputStreamInterface.h"

namespace TCode::Events {
    void runEvent(TCodeContext &context, const Datatypes::TCodeEvent &event);
    bool runAxisCommand(TCodeContext &context, const Datatypes::AxisCommandEvent &event);
    bool runDeviceCommand(TCodeContext &context, const Datatypes::DeviceCommandEvent &event);
    bool runSetupCommand(TCodeContext &context, const Datatypes::SetupCommandEvent &event);
}