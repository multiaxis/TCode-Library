// TCode-Enums-H v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 05/02/2022
// usage of this class can be found at (https://github.com/Dreamer2345/Arduino_TCode_Parser)
// Please copy, share, learn, innovate, give attribution.
// Decodes T-code commands
// It can handle:
//   x linear channels (L0, L1, L2... L9)
//   x rotation channels (R0, R1, R2... L9)
//   x vibration channels (V0, V1, V2... V9)
//   x auxilliary channels (A0, A1, A2... A9)
// History:
//
#ifndef TCODE_ENUMS_H
#define TCODE_ENUMS_H
#include <Arduino.h>

/**
 * @brief representation of a TCode channel type as an Enum
 * @param Linear
 * @param Rotation
 * @param Vibration
 * @param Auxiliary
 * @param None
 */
enum class TCode_Channel_Type
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
struct TCode_ChannelID
{
    TCode_Channel_Type type;
    unsigned char channel;
};

/**
 * @brief representation of a TCode Axis Extention as an Enum
 * @param Speed Controls the Rate of change of the Axis value
 * @param Time Controls the Rate of change based on a target time for the Axis Value
 * @param None
 */
enum class TCode_Axis_Extention_Type
{
    Speed,
    Time,
    None,
};

/**
 * @brief representation of a TCode Channel type as an Enum
 * @param Linear Uses Linear interpolation to go from the current value to the target
 * @param EaseIn Uses Easing In ramp function to go from the current value to the target meaning the begining is eased using quadratics and the end is Linear
 * @param EaseOut Uses Easing Out ramp function to go from the current value to the target meaning the end is eased using quadratics and the begining is Linear
 * @param EaseInOut Uses Easing In and Out ramp function to go from the current value to the target meaning it is eased both in and out
 * @param None
 */
enum class TCode_Axis_Ramp_Type
{
    Linear,
    EaseIn,
    EaseOut,
    EaseInOut,
    None,
};

/**
 * @brief representation of a TCode Command type as an Enum
 * @param Axis Is an axis command which changes the values of the axis
 * @param Device Commands the Device itself to do something
 * @param Setup Sets different values within the Device which are used externaly
 * @param None
 */
enum class TCode_Command_Type
{
    Axis,
    Device,
    Setup,
    External,
    None,
};

/**
 * @brief representation of a TCode Device Command type as an Enum
 * @param GetSoftwareVersion gets the software version string stored on the device
 * @param GetTCodeVersion gets the TCode version string stored on the device
 * @param GetAssignedAxisValues gets the assigned axis and their min and max values aswell as their names
 * @param StopDevice stops the device causing it to stay at its current position
 * @param None
 */
enum class TCode_Device_Command_Type
{
    GetSoftwareVersion,
    GetTCodeVersion,
    GetAssignedAxisValues,
    StopDevice,
    None,
};

/**
 * @brief representation of the data for an TCode Axis Command
 * @param extentionType the extention type of the command used for time and speed functions when setting axis values
 * @param rampType used to control the ramp type of extention commands
 * @param command_value the target value of the axis
 * @param commandExtention the extention value used by the axis for time and speed functions 
*/
struct TCode_Axis_Data
{
    TCode_Axis_Extention_Type extentionType;
    TCode_Axis_Ramp_Type rampType;
    int commandValue;
    long commandExtention;
};

/**
 * @brief representation of a TCode Device Command
 * @param type the type of command to be executed
 */
struct TCode_Device_Command
{
    TCode_Device_Command_Type type;
};

/**
 * @brief representation of a TCode Axis Command
 * @param ID the ID to be edited by the command
 * @param Data stores the timing and value data for the command
 */
struct TCode_Axis_Command
{
    TCode_ChannelID ID;
    TCode_Axis_Data Data;
};

/**
 * @brief representation of the data for an TCode Axis State
 * @param startTime the time in ms when the state is active
 * @param stopTime the time in ms when the state is inactive
 * @param rampType used to control the ramp type of extention commands
 * @param startValue the starting value of the state
 * @param endValue the target value for this state
*/
struct TCode_Axis_State
{
    unsigned long startTime;
    unsigned long endTime;
    TCode_Axis_Ramp_Type rampType;
    int startValue;
    int endValue;
};

/**
 * @brief representation of a TCode Save Entry used to store the minimum and maximum values for an axis
 * @param min minimum axis value
 * @param max maximum axis value
 */
struct TCode_Save_Entry
{
    int min;
    int max;
};

/**
 * @brief representation of a TCode Setup Command
 * @param ID the ID to be edited by the command
 * @param Save the save entry values to be stored
 */
struct TCode_Setup_Command
{
    TCode_ChannelID ID;
    TCode_Save_Entry Save;
};

/**
 * @brief representation of a TCode External Command
 * @param length the length of the command
 * @param command the body of the command
 */
struct TCode_External_Command
{
    unsigned int length;
    const char *command;

    ~TCode_External_Command()
    {
        if(command != nullptr)
            delete[] command;
    }
};

#endif