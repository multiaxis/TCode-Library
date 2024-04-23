// TCode-Enums-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 05/02/2022
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <Arduino.h>

namespace TCode {

/**
 * @brief representation of a TCode channel type as an Enum
 * @param Linear
 * @param Rotation
 * @param Vibration
 * @param Auxiliary,
 * @param None
 */
enum class ChannelType
{
    Linear,
    Rotation,
    Vibration,
    Auxiliary,
    None,
};

/**
 * @brief representation of a TCode channel type and channel number
 * @param type type of channel
 * @param channel channel number
 */
struct ChannelID
{
    ChannelType type;
    uint8_t channel;
};

/**
 * @brief representation of a TCode Axis Extention as an Enum
 * @param Speed Controls the Rate of change of the Axis value
 * @param Time Controls the Rate of change based on a target time for the Axis Value
 * @param None
 */
enum class AxisExtentionType
{
    Speed,
    Time,
    None,
};

/**
 * @brief representation of a TCode Axis ramp type as an Enum
 * @param In
 * @param Out
 * @param InOut
 * @param None
 */
enum class AxisRampType
{
    In,
    Out,
    InOut,
    None,
};

/**
 * @brief representation of a TCode Command type as an Enum
 * @param Axis Is an axis command which changes the values of the axis
 * @param Device Commands the Device itself to do something
 * @param Setup Sets different values within the Device which are used externaly
 * @param None
 */
enum class CommandType
{
    Axis,
    Device,
    Setup,
    None,
};

/**
 * @brief representation of a TCode Device Command type as an Enum
 * @param GetSoftwareVersion gets the software version string stored on the device
 * @param GetTCodeVersion gets the TCode version string stored on the device
 * @param GetAssignedAxisValues gets the assigned axis and their min and max values aswell as their names
 * @param StopDevice stops the device causing it to stay at its current position
 * @param NONE
 */
enum class DeviceCommandType
{
    GetSoftwareVersion,
    GetTCodeVersion,
    GetAssignedAxisValues,
    StopDevice,
    None,
};

/**
 * @brief representation of the data for an TCode Axis ramp 
*/
struct AxisRampData 
{
    float tangent;
    bool hasTangent;
    float weight;
    bool hasWeight;
    bool autoTangent;
};

/**
 * @brief representation of the data for an TCode Axis Command
*/
struct AxisData
{
    float commandValue;
    unsigned long commandExtention;
    AxisExtentionType extentionType;
    AxisRampType rampType;
    AxisRampData rampIn;
    AxisRampData rampOut;
};

/**
 * @brief representation of a TCode Save Entry used to store the minimum and maximum values for an axis
 * @param min minimum axis value
 * @param max maximum axis value
 */
struct SaveEntry
{
    float min;
    float max;
    uint8_t minLog;
    uint8_t maxLog;
};

/**
 * @brief representation of the data for an TCode Axis State
 * @param startTime the time in ms when the state is active
 * @param stopTime the time in ms when the state is inactive
 * @param startValue the starting value of the state
 * @param endValue the target value for this state
*/
struct AxisState
{
    unsigned long startTime;
    unsigned long endTime;
    float startValue;
    float endValue;
    AxisRampData startRamp;
    AxisRampData endRamp;
    AxisData data;
};

/**
 * @brief representation of a TCode Device Command
 * @param type the type of command to be executed
 */
struct DeviceCommand
{
    DeviceCommandType type;
};

/**
 * @brief representation of a TCode Axis Command
 * @param ID the ID to be edited by the command
 * @param Data stores the timing and value data for the command
 */
struct AxisCommand
{
    ChannelID id;
    AxisData data;
};

/**
 * @brief representation of a TCode Setup Command
 * @param ID the ID to be edited by the command
 * @param Save the save entry values to be stored
 */
struct SetupCommand
{
    ChannelID id;
    SaveEntry saveEntryData;
};

}
