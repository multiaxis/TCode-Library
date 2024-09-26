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
    /**
     * @brief Executes a TCode event in the given context.
     *
     * This method processes a generic `TCodeEvent` and triggers the appropriate commands
     * in the provided `TCodeContext`. It acts as the main entry point for handling
     * events based on the type of command they represent (axis, device, or setup).
     *
     * @param context The `TCodeContext` that contains references to required managers (e.g., axis, device, settings).
     * @param event The `TCodeEvent` to be processed and executed.
     */
    void runEvent(TCodeContext &context, const Datatypes::TCodeEvent &event);

    /**
     * @brief Executes an axis command event in the given context.
     *
     * This method handles the execution of an `AxisCommandEvent` within the provided `TCodeContext`.
     * It is responsible for updating or controlling the axes of a device (e.g., movement or position changes).
     *
     * @param context The `TCodeContext` that contains the axis manager for handling axis-related commands.
     * @param event The `AxisCommandEvent` that specifies the axis-related command to be executed.
     * @return true if the axis command was successfully executed, false otherwise.
     */
    bool runAxisCommand(TCodeContext &context, const Datatypes::AxisCommandEvent &event);

    /**
     * @brief Executes a device command event in the given context.
     *
     * This method processes a `DeviceCommandEvent` in the context of a device, enabling control
     * over specific device operations, such as power, mode switching, or other device-specific commands.
     *
     * @param context The `TCodeContext` that contains the relevant managers for executing device-related commands.
     * @param event The `DeviceCommandEvent` containing the device command to be executed.
     * @return true if the device command was successfully executed, false otherwise.
     */
    bool runDeviceCommand(TCodeContext &context, const Datatypes::DeviceCommandEvent &event);

    /**
     * @brief Executes a setup command event in the given context.
     *
     * This method handles the execution of a `SetupCommandEvent`, which involves configuring or
     * initializing certain settings or properties within the device or application environment.
     * It is used to adjust setup configurations like calibration, initialization, or system settings.
     *
     * @param context The `TCodeContext` that contains the setup or settings manager for applying setup-related commands.
     * @param event The `SetupCommandEvent` specifying the setup operation to be performed.
     * @return true if the setup command was successfully executed, false otherwise.
     */
    bool runSetupCommand(TCodeContext &context, const Datatypes::SetupCommandEvent &event);

}