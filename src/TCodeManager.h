// TCode-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 10/09/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include "datatypes/CommandDataTypes.h"
#include "datatypes/CommonDataTypes.h"
#include "datatypes/EnumTypes.h"

#include "settings/ISettings.h"

#include "context/TContext.h"

#include "module/TModuleBase.h"
#include "module/TModuleManager.h"

#include "axis/TAxis.h"
#include "axis/TAxisManager.h"

#include "utils/TString.h"
#include "utils/TMath.h"

#include "encoding/TEncoder.h"
#include "datawriter/TDataWriter.h"

#include "outputstream/TOutputStreamInterface.h"

#include "events/TEventReader.h"
#include "events/TEventRunner.h"
#include "parsing/TParser.h"

namespace TCode {
    class TCodeManager {
    public:
        TCodeManager(const char *firmware = DEFAULT_FIRMWARE_NAME, const char *tcodeVersion = DEFAULT_TCODE_VERSION, const char *Filepath = DEFAULT_FILE_NAME);

        void read(const byte input);
        void read(const char input);
        void read(const String &input);
        void read(const char *input);

        void clearInputBuffer();

        bool registerAxis(const char *name, const Datatypes::AxisId &id, float defaultValue);
        bool registerAxis(Axis::TCodeAxis *axis);
        void setAxisData(const Datatypes::AxisId &id, const Datatypes::AxisData &data);
        void setAxisData(const Datatypes::AxisId &id, const float value, const Datatypes::AxisExtentionType extentionType = Datatypes::AxisExtentionType::None, const unsigned long commandExtention = 0, Datatypes::AxisRampData rampIn = {0.0,0.0,false,false,false}, Datatypes::AxisRampData rampOut = {0.0,0.0,false,false,false});
        float getAxisPosition(const Datatypes::AxisId &axisId);
        unsigned long getAxisLastCommandTime(const Datatypes::AxisId &axisId);
        void stop();

        void registerInterface(Module::TCodeModuleBase *interface);
        void updateInterfaces();

        void setSettingManager(Settings::TCodeSettingsInterface *settings);
        void setOutputStream(Output::TCodeIOutputStream *stream);

    private:
        TCodeContext context;
        Axis::TCodeAxisManager axisManager;
        Module::TCodeModuleManager moduleManager;
        Events::TCodeEventReader eventReader;

        void runProcessedCommands();
    };

}