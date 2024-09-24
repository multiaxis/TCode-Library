#include "TEventRunner.h"
#include <Arduino.h>
#include <utils/TString.h>

namespace TCode::Events {

    void runEvent(TCodeContext &context, const Datatypes::TCodeEvent &event) {
        switch (event.commandType) {
        case Datatypes::CommandType::Axis: {
            // LogHandler::info("TEventRunner","Running Axis Command");
            runAxisCommand(context, event.axisCommand);
            break;
        }
        case Datatypes::CommandType::Device: {
            // LogHandler::info("TEventRunner","Running Device Command");
            runDeviceCommand(context, event.deviceCommand);
            break;
        }
        case Datatypes::CommandType::Setup: {
            // LogHandler::info("TEventRunner","Running Setup Command");
            runSetupCommand(context, event.setupCommand);
            break;
        }
        case Datatypes::CommandType::Firmware: {
            // LogHandler::error("TEventRunner", "Default Event Runner Does not Implement Firmware Commands: Received Firmware ID: %d",event.firmwareCommand.firmwareID);
            Serial.printf("[INFO] EVENT RUNNER:\"%s\"\n", event.firmwareCommand.value);
            break;
        }
        default:
            break;
        }
    }

    bool runAxisCommand(TCodeContext &context, const Datatypes::AxisCommandEvent &event) {
        Axis::TCodeAxisManager *axisManager;
        if (!context.getAxisManager(axisManager)) {
            // LogHandler::warning("TEventRunner", "Cannot Run Axis Command Axis manager is not set");
            return false;
        }

        axisManager->setAxisData(event.id, event.data);
        return true;
    }

    void printSavedAxisValues(Axis::TCodeAxisManager *axisManager, Settings::TCodeSettingsInterface *settingManager, Output::TCodeIOutputStream *outputStream) {
        for (size_t i = 0; i < axisManager->count(); i++) {
            Axis::TCodeAxis *axis = axisManager->getAxisIndex(i);

            float min = 0.0f;
            int minLog = 4;
            float max = 1.0;
            int maxLog = 4;

            String idString = TString::axisIdToString(axis->getId());
            String settingName = "AXIS-MIN-" + idString;
            if (!settingManager->getSetting(settingName.c_str(), min))
                settingManager->setSetting(settingName.c_str(), 0.0f);

            settingName = "AXIS-MIN-LOG-" + idString;
            if (!settingManager->getSetting(settingName.c_str(), minLog))
                settingManager->setSetting(settingName.c_str(), 4);

            settingName = "AXIS-MAX-" + idString;
            if (!settingManager->getSetting(settingName.c_str(), max))
                settingManager->setSetting(settingName.c_str(), 1.0f);

            settingName = "AXIS-MAX-LOG-" + idString;
            if (!settingManager->getSetting(settingName.c_str(), maxLog))
                settingManager->setSetting(settingName.c_str(), 4);

            uint8_t logOut;
            unsigned long tcodeMin = TMath::getTCodeFromFloat(min, minLog, logOut);
            unsigned long tcodeMax = TMath::getTCodeFromFloat(max, maxLog, logOut);

            outputStream->print(idString);
            outputStream->print(' ');
            outputStream->print(String(tcodeMin));
            outputStream->print(' ');
            outputStream->print(String(tcodeMax));
            outputStream->print(' ');
            outputStream->println(axis->getName());
        }
    }

    bool setSaveValues(Axis::TCodeAxisManager *axisManager, Settings::TCodeSettingsInterface *settingManager, Output::TCodeIOutputStream *outputStream, const Datatypes::AxisId &id, float minimum, float maximum, uint8_t minLog, uint8_t maxLog) {
        if (settingManager == nullptr) {
            // LogHandler::error("TEventRunner", "Setting manager is not set");
            return false;
        }

        if (!axisManager->hasAxisFromId(id)) {
            return false;
        }

        minimum = constrain(minimum, 0.f, 1.f);
        maximum = constrain(maximum, 0.f, 1.f);

        const uint8_t TCODE_MAX_LOG = 15;
        minLog = min(minLog, TCODE_MAX_LOG);
        maxLog = min(maxLog, TCODE_MAX_LOG);

        String idString = TString::axisIdToString(id);
        String settingName = "AXIS-MIN-" + idString;
        settingManager->setSetting(settingName.c_str(), minimum);

        settingName = "AXIS-MAX-" + idString;
        settingManager->setSetting(settingName.c_str(), maximum);

        settingName = "AXIS-MIN-LOG-" + idString;
        settingManager->setSetting(settingName.c_str(), (int)minLog);

        settingName = "AXIS-MAX-LOG-" + idString;
        settingManager->setSetting(settingName.c_str(), (int)maxLog);
        return true;
    }

    bool runDeviceCommand(TCodeContext &context, const Datatypes::DeviceCommandEvent &event) {
        Axis::TCodeAxisManager *axisManager;
        if (!context.getAxisManager(axisManager)) {
            // LogHandler::warning("TEventRunner", "Cannot Run Device Command, Axis manager is not set");
            return false;
        }

        Output::TCodeIOutputStream *outputStream;
        if (!context.getOutputStream(outputStream)) {
            // LogHandler::warning("TEventRunner", "Cannot Run Device Command, Output Stream is not set");
            return false;
        }

        switch (event.type) {
        case Datatypes::DeviceCommandType::None:
            break;
        case Datatypes::DeviceCommandType::StopDevice: {
            axisManager->stop();
            outputStream->println("STOP");
        } break;
        case Datatypes::DeviceCommandType::GetTCodeVersion: {
            outputStream->println(context.getTCodeVersion());
            break;
        }
        case Datatypes::DeviceCommandType::GetSoftwareVersion: {
            outputStream->println(context.getFirmware());
            break;
        }
        case Datatypes::DeviceCommandType::GetAssignedAxisValues: {
            Settings::TCodeSettingsInterface *settingManager;
            if (!context.getSettingManager(settingManager)) {
                // LogHandler::error("TEventRunner", "Cannot Get Values Setting manager is not set");
                return false;
            }
            printSavedAxisValues(axisManager, settingManager, outputStream);
            break;
        }
        }
        return true;
    }

    bool runSetupCommand(TCodeContext &context, const Datatypes::SetupCommandEvent &command) {
        Axis::TCodeAxisManager *axisManager;
        if (!context.getAxisManager(axisManager)) {
            // LogHandler::warning("TEventRunner", "Cannot Run Setup Command, Axis manager is not set");
            return false;
        }

        Settings::TCodeSettingsInterface *settingManager;
        if (!context.getSettingManager(settingManager)) {
            // LogHandler::warning("TEventRunner", "Cannot Run Setup Command, Setting manager is not set");
            return false;
        }

        Output::TCodeIOutputStream *outputStream;
        if (!context.getOutputStream(outputStream)) {
            // LogHandler::warning("TEventRunner", "Cannot Run Setup Command, Output Stream is not set");
            return false;
        }

        setSaveValues(axisManager, settingManager, outputStream, command.id, command.saveEntryData.min, command.saveEntryData.max, command.saveEntryData.minLog, command.saveEntryData.maxLog);
        printSavedAxisValues(axisManager, settingManager, outputStream);
        return true;
    }

}