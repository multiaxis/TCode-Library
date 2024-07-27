#include "TCodeManager.h"

namespace TCode {
    TCodeManager::TCodeManager(const char *firmware, const char *tcodeVersion, const char *Filepath) : context(firmware, tcodeVersion, Filepath) {
        context.setAxisManager(&axisManager);
        context.setInterfaceManager(&interfaceManager);
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

    bool TCodeManager::registerAxis(const char *name, const AxisId &id, float defaultValue) {
        return axisManager.registerAxis(new TCodeAxis(name, id, defaultValue));
    }

    bool TCodeManager::registerAxis(TCodeAxis *axis) {
        return axisManager.registerAxis(axis);
    }

    void TCodeManager::setAxisData(const AxisId &id, const AxisData &data) {
        axisManager.setAxisData(id, data);
    }

    float TCodeManager::getAxisPosition(const AxisId &axisId) {
        return axisManager.getAxisPosition(axisId);
    }

    unsigned long TCodeManager::getAxisLastCommandTime(const AxisId &axisId) {
        return axisManager.getAxisLastCommandTime(axisId);
    }

    void TCodeManager::stop() {
        axisManager.stop();
    }

    void TCodeManager::updateInterfaces() {
        interfaceManager.update(context);
    }

    void TCodeManager::setSettingManager(Settings::ISettings *settings) {
        context.setSettingManager(settings);
    }

    void TCodeManager::setOutputStream(Print *stream) {
        context.setOutputStream(stream);
    }

    void TCodeManager::runProcessedCommands() {
        TCodeEvent event;
        while (eventReader.getNext(event)) {
            TEvents::runEvent(context, event);
        }
    }

    void TCodeManager::registerInterface(TInterfaceBase *interface) {
        interfaceManager.registerInterface(interface);
    }
};