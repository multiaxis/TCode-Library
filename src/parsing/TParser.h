// TCode-Parser-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/06/2022
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include "../datatypes/CommandDataTypes.h"
#include "../datatypes/CommonDataTypes.h"
#include "../datatypes/EnumTypes.h"
#include <deque>

namespace TCode::TParser {

    /**
     * @brief Checks if the given character represents an axis extension.
     *
     * This inline function checks if the provided character is a valid axis extension identifier.
     *
     * @param value The character to be checked.
     * @return true if the character is a valid extension identifier, false otherwise.
     */
    inline bool isExtention(const char value);

    /**
     * @brief Checks if the given character represents a ramp type.
     *
     * This inline function checks if the provided character is a valid ramp type identifier.
     *
     * @param value The character to be checked.
     * @return true if the character is a valid ramp type identifier, false otherwise.
     */
    inline bool isRamp(const char value);

    /**
     * @brief Checks if the given character is a valid hexadecimal digit.
     *
     * This inline function checks if the provided character is a valid hexadecimal digit (0-9, A-F).
     *
     * @param value The character to be checked.
     * @return true if the character is a valid hex digit, false otherwise.
     */
    inline bool isHex(const char value);

    /**
     * @brief Retrieves the axis extension type from the provided buffer.
     *
     * This function extracts the axis extension type from the buffer starting at the given index.
     *
     * @param index A reference to the index in the buffer from which to begin parsing.
     * @param buffer The input buffer containing the axis extension command.
     * @param length The length of the buffer.
     * @return The parsed axis extension type as a `Datatypes::AxisExtentionType`.
     */
    Datatypes::AxisExtentionType getExtentionType(size_t &index, const char *buffer, const size_t length);

    /**
     * @brief Retrieves the axis ramp type from the provided buffer.
     *
     * This function extracts the axis ramp type from the buffer starting at the given index.
     *
     * @param index A reference to the index in the buffer from which to begin parsing.
     * @param buffer The input buffer containing the ramp type.
     * @param length The length of the buffer.
     * @return The parsed ramp type as a `Datatypes::AxisRampType`.
     */
    Datatypes::AxisRampType getRampType(size_t &index, const char *buffer, const size_t length);

    /**
     * @brief Retrieves the axis ID from the provided buffer.
     *
     * This function parses the axis ID from the buffer starting at the given index.
     *
     * @param index A reference to the index in the buffer from which to begin parsing.
     * @param buffer The input buffer containing the axis ID.
     * @param length The length of the buffer.
     * @return The parsed axis ID as a `Datatypes::AxisId`.
     */
    Datatypes::AxisId getAxisId(size_t &index, const char *buffer, const size_t length);

    /**
     * @brief Retrieves the axis ID from the start of the provided buffer.
     *
     * This function parses the axis ID starting from the beginning of the buffer.
     *
     * @param buffer The input buffer containing the axis ID.
     * @return The parsed axis ID as a `Datatypes::AxisId`.
     */
    Datatypes::AxisId getAxisIdFromStart(const char *buffer);

    /**
     * @brief Determines the command type from the provided buffer.
     *
     * This function analyzes the provided buffer and determines the command type contained within.
     *
     * @param buffer The input buffer containing the command.
     * @param length The length of the buffer.
     * @return The parsed command type as a `Datatypes::CommandType`.
     */
    Datatypes::CommandType getCommandType(const char *buffer, const size_t length);

    /**
     * @brief Parses an axis command from the provided buffer.
     *
     * This function parses an axis command from the buffer and populates the output `AxisCommandEvent` structure.
     *
     * @param buffer The input buffer containing the axis command.
     * @param length The length of the buffer.
     * @param out The parsed axis command event.
     * @return true if the command was successfully parsed, false otherwise.
     */
    bool parseAxisCommand(const char *buffer, const size_t length, Datatypes::AxisCommandEvent &out);

    /**
     * @brief Parses an axis extension from the provided buffer.
     *
     * This function parses an axis extension from the buffer, including the extension type and command extension.
     *
     * @param index A reference to the index in the buffer from which to begin parsing.
     * @param buffer The input buffer containing the axis extension.
     * @param length The length of the buffer.
     * @param extentionType The parsed axis extension type.
     * @param commandExtention The parsed command extension value.
     * @return true if the axis extension was successfully parsed, false otherwise.
     */
    bool parseAxisExtention(size_t &index, const char *buffer, const size_t length, Datatypes::AxisExtentionType &extentionType, unsigned long &commandExtention);

    /**
     * @brief Parses axis ramp data from the provided buffer.
     *
     * This function parses ramp data from the buffer and populates the output `AxisRampData` structure.
     *
     * @param index A reference to the index in the buffer from which to begin parsing.
     * @param buffer The input buffer containing the ramp data.
     * @param length The length of the buffer.
     * @param rampOut The parsed axis ramp data.
     * @return true if the ramp data was successfully parsed, false otherwise.
     */
    bool parseAxisRamp(size_t &index, const char *buffer, const size_t length, Datatypes::AxisRampData &rampOut);

    /**
     * @brief Parses a device command from the provided buffer.
     *
     * This function parses a device command from the buffer and populates the output `DeviceCommandEvent` structure.
     *
     * @param buffer The input buffer containing the device command.
     * @param length The length of the buffer.
     * @param out The parsed device command event.
     * @return true if the command was successfully parsed, false otherwise.
     */
    bool parseDeviceCommand(const char *buffer, const size_t length, Datatypes::DeviceCommandEvent &out);

    /**
     * @brief Parses a setup command from the provided buffer.
     *
     * This function parses a setup command from the buffer and populates the output `SetupCommandEvent` structure.
     *
     * @param buffer The input buffer containing the setup command.
     * @param length The length of the buffer.
     * @param out The parsed setup command event.
     * @return true if the command was successfully parsed, false otherwise.
     */
    bool parseSetupCommand(const char *buffer, const size_t length, Datatypes::SetupCommandEvent &out);

    /**
     * @brief Parses a firmware command from the provided buffer.
     *
     * This function parses a firmware command from the buffer and populates the output `FirmwareCommandEvent` structure.
     *
     * @param buffer The input buffer containing the firmware command.
     * @param length The length of the buffer.
     * @param out The parsed firmware command event.
     * @return true if the command was successfully parsed, false otherwise.
     */
    bool parseFirmwareCommand(const char *buffer, const size_t length, Datatypes::FirmwareCommandEvent &out);

    /**
     * @brief Parses a generic TCode event from the provided buffer.
     *
     * This function parses a generic command or event from the buffer and populates the output `TCodeEvent` structure.
     *
     * @param buffer The input buffer containing the event.
     * @param length The length of the buffer.
     * @param out The parsed TCode event.
     * @return true if the event was successfully parsed, false otherwise.
     */
    bool parseCommand(const char *buffer, const size_t length, Datatypes::TCodeEvent &out);

};