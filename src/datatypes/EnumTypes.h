// TCode-Enums-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 05/02/2022
// Please copy, share, learn, innovate, give attribution.
#pragma once

namespace TCode
{
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

    enum class CommandType {
        Axis,
        Device,
        Setup,
        None,
    };

    enum class DeviceCommandType {
        GetSoftwareVersion,
        GetTCodeVersion,
        GetAssignedAxisValues,
        StopDevice,
        None,
    };

    enum class AxisRampType {
        In,
        Out,
        InOut,
        None
    };
}