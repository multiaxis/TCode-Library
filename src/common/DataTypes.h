// TCode-Enums-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 05/02/2022
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <Arduino.h>

/**
 * @brief representation of a TCode channel type as an Enum
 * @param Linear
 * @param Rotation
 * @param Vibration
 * @param Auxiliary,
 * @param None
 */
enum class AxisType
{
    Linear,
    Rotation,
    Vibration,
    Auxiliary,
    None,
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
 * @brief representation of a TCode channel type and channel number
 * @param type type of channel
 * @param channel channel number
 */
struct AxisId
{
    AxisType type;
    uint8_t channel;
};

inline bool operator==(const AxisId& lhs, const AxisId& rhs)
{
    return lhs.type == rhs.type && lhs.channel == rhs.channel;
}

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