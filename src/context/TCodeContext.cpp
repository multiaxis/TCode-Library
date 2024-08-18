#include "TCodeContext.h"

namespace TCode {
    void TCodeContext::setAxisManager(TCodeAxisManager *inputAxisManager) {
        axisManager = inputAxisManager;
        LogHandler::verbose("TCodeContext", "Context Axis Manager Set");
    }

    void TCodeContext::setInterfaceManager(TCodeInterfaceManager *inputInterfaceManager) {
        interfaceManager = inputInterfaceManager;
        LogHandler::verbose("TCodeContext", "Context Interface Manager Set");
    }

    void TCodeContext::setSettingManager(Settings::ISettings *settings) {
        settingManager = settings;
        LogHandler::verbose("TCodeContext", "Context Settings Manager Set");
    }

    void TCodeContext::setOutputStream(OutputStreamInterface *stream) {
        outputStream = stream;
        LogHandler::info("TCodeContext", "Context Output Stream Set");
    }

    bool TCodeContext::getAxisManager(TCodeAxisManager *&manager) {
        if (axisManager == nullptr)
            return false;
        manager = axisManager;
        return true;
    }

    bool TCodeContext::getInterfaceManager(TCodeInterfaceManager *&manager) {
        if (interfaceManager == nullptr)
            return false;
        manager = interfaceManager;
        return true;
    }

    bool TCodeContext::getSettingManager(Settings::ISettings *&settings) {
        if (settingManager == nullptr)
            return false;
        settings = settingManager;
        return true;
    }

    bool TCodeContext::getOutputStream(OutputStreamInterface *&stream) {
        if (outputStream == nullptr)
            return false;
        stream = outputStream;
        return true;
    }

}