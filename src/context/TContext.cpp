#include "TContext.h"

namespace TCode {
    void TCodeContext::setAxisManager(Axis::TCodeAxisManager *inputAxisManager) {
        axisManager = inputAxisManager;
        //LogHandler::verbose("TCodeContext", "Context Axis Manager Set");
    }

    void TCodeContext::setModuleManager(Module::TCodeModuleManager *inputInterfaceManager) {
        moduleManager = inputInterfaceManager;
        //LogHandler::verbose("TCodeContext", "Context Interface Manager Set");
    }

    void TCodeContext::setSettingManager(Settings::TCodeSettingsInterface *settings) {
        settingManager = settings;
        //LogHandler::verbose("TCodeContext", "Context Settings Manager Set");
    }

    void TCodeContext::setOutputStream(Output::TCodeIOutputStream *stream) {
        outputStream = stream;
        //LogHandler::info("TCodeContext", "Context Output Stream Set");
    }

    bool TCodeContext::getAxisManager(Axis::TCodeAxisManager *&manager) {
        if (axisManager == nullptr)
            return false;
        manager = axisManager;
        return true;
    }

    bool TCodeContext::getModuleManager(Module::TCodeModuleManager *&manager) {
        if (moduleManager == nullptr)
            return false;
        manager = moduleManager;
        return true;
    }

    bool TCodeContext::getSettingManager(Settings::TCodeSettingsInterface *&settings) {
        if (settingManager == nullptr)
            return false;
        settings = settingManager;
        return true;
    }

    bool TCodeContext::getOutputStream(Output::TCodeIOutputStream *&stream) {
        if (outputStream == nullptr)
            return false;
        stream = outputStream;
        return true;
    }

}