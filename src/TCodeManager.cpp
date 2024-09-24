#include "TCodeManager.h"

namespace TCode {
    TCodeManager::TCodeManager(const char *firmware, const char *tcodeVersion, const char *Filepath) : context(firmware, tcodeVersion, Filepath) {
        context.setAxisManager(&axisManager);
        context.setModuleManager(&moduleManager);
    }

    void TCodeManager::read(const byte input) {
        eventReader.read(input);
        runProcessedCommands();
    }

    void TCodeManager::read(const char input) {
        eventReader.read(input);
        runProcessedCommands();
    }

    void TCodeManager::read(const String &input) {
        eventReader.read(input);
        runProcessedCommands();
    }

    void TCodeManager::read(const char *input) {
        eventReader.read(input);
        runProcessedCommands();
    }

    void TCodeManager::clearInputBuffer() {
        eventReader.flush();
    }

    bool TCodeManager::registerAxis(const char *name, const Datatypes::AxisId &id, float defaultValue) {
        return axisManager.registerAxis(new Axis::TCodeAxis(name, id, defaultValue));
    }

    bool TCodeManager::registerAxis(Axis::TCodeAxis *axis) {
        return axisManager.registerAxis(axis);
    }

    void TCodeManager::setAxisData(const Datatypes::AxisId &id, const Datatypes::AxisData &data) {
        axisManager.setAxisData(id, data);
    }

    void TCodeManager::setAxisData(const Datatypes::AxisId &id, const float value, const Datatypes::AxisExtentionType extentionType, const unsigned long commandExtention, Datatypes::AxisRampData rampIn, Datatypes::AxisRampData rampOut) {
        Datatypes::AxisData data = {
            value,
            commandExtention,
            extentionType,
            rampIn,
            rampOut
        };
        
        axisManager.setAxisData(id,data);
    }

    float TCodeManager::getAxisPosition(const Datatypes::AxisId &axisId) {
        return axisManager.getAxisPosition(axisId);
    }

    unsigned long TCodeManager::getAxisLastCommandTime(const Datatypes::AxisId &axisId) {
        return axisManager.getAxisLastCommandTime(axisId);
    }

    void TCodeManager::stop() {
        axisManager.stop();
    }

    void TCodeManager::updateInterfaces() {
        moduleManager.update(context);
    }

    void TCodeManager::setSettingManager(Settings::TCodeSettingsInterface *settings) {
        context.setSettingManager(settings);
    }

    void TCodeManager::setOutputStream(Output::TCodeIOutputStream *stream) {
        context.setOutputStream(stream);
    }

    void TCodeManager::runProcessedCommands() {
        Datatypes::TCodeEvent event;
        while (eventReader.getNext(event)) {
            Events::runEvent(context, event);
        }
    }

    void TCodeManager::registerInterface(Module::TCodeModuleBase *interface) {
        moduleManager.registerInterface(interface);
    }
};