// TCode-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 10/09/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <Arduino.h>
#include <vector>

#include "datatypes/CommandDataTypes.h"
#include "datatypes/CommonDataTypes.h"
#include "datatypes/EnumTypes.h"

#include "settings/Variant/Variant.h"
#include "settings/Variant/DataContainer.h"
#include "settings/Variant/TaggedDataContainer.h"

#include "settings/settingsClasses/ISettings.h"
#include "settings/settingsClasses/SettingsESP32.h"

#include "interface/interfaceBaseClass/TInterfaceBase.h"
#include "interface/interfaceManagement/interfaceManager.h"
#include "interface/buttonInterface/TButton.h"

#include "events/EventReader/TCodeEventReader.h"
#include "events/EventRunner/TEventRunner.h"
#include "events/Parsing/TParser.h"

#include "context/TCodeContext.h"

#include "axis/axisClass/TAxis.h"
#include "axis/axisManagement/TCodeAxisManager.h"

namespace TCode
{

    class TCodeManager
    {
    public:
        TCodeManager(const char *firmware = DEFAULT_FIRMWARE_NAME, const char *tcodeVersion = DEFAULT_TCODE_VERSION, const char *Filepath = DEFAULT_FILE_NAME);

        void read(const byte input);
        void read(const char input);
        void read(const String &input);
        void read(const char *input);

        void clearInputBuffer();

        bool registerAxis(const char* name, const AxisId &id, float defaultValue);
        bool registerAxis(TCodeAxis* axis);
        void setAxisData(const AxisId &id, const AxisData &data);
        float getAxisPosition(const AxisId &axisId);               
        unsigned long getAxisLastCommandTime(const AxisId &axisId);
        void stop();

        void registerInterface(TInterfaceBase *interface);
        void updateInterfaces();

        void setSettingManager(ISettings *settings);
        void setOutputStream(Print *stream);

    private:
        TCodeContext context;
        TCodeAxisManager axisManager;
        TCodeInterfaceManager interfaceManager;
        TEvents::TCodeEventReader eventReader;

        void runProcessedCommands();
    };

}