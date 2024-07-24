#include "TCodeContext.h"


namespace TCode {
    void TCodeContext::setAxisManager(TCodeAxisManager *inputAxisManager)
    {
        axisManager = inputAxisManager;
    }

    void TCodeContext::setInterfaceManager(TCodeInterfaceManager *inputInterfaceManager)
    {
        interfaceManager = inputInterfaceManager;
    }

    void TCodeContext::setSettingManager(ISettings *settings)
    {
        settingManager = settings;
    }

    void TCodeContext::setOutputStream(Print *stream)
    {
        outputStream = stream;
    }

    bool TCodeContext::getAxisManager(TCodeAxisManager *&manager)
    {
        if(axisManager == nullptr)
            return false;
        manager = axisManager;
        return true;
    }

    bool TCodeContext::getInterfaceManager(TCodeInterfaceManager *&manager)
    {
        if(interfaceManager == nullptr)
            return false;
        manager = interfaceManager;
        return true;
    }

    bool TCodeContext::getSettingManager(ISettings *&settings)
    {
        if(settingManager == nullptr)
            return false;
        settings = settingManager;
        return true;
    }

    bool TCodeContext::getOutputStream(Print *&stream)
    {
        if(outputStream == nullptr)
            return false;
        stream = outputStream;
        return true;
    }

}