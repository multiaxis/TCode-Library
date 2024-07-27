// TCode-DataTypes-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 05/02/2022
// Please copy, share, learn, innovate, give attribution.
#pragma once
#include "EnumTypes.h"
#include <Arduino.h>

namespace TCode {
    /**
     * @brief representation of a TCode channel type and channel number
     * @param type type of channel
     * @param channel channel number
     */
    struct AxisId {
        AxisType type;
        uint8_t channel;

        bool isValid() {
            return type != AxisType::None && channel >= 0 && channel <= 9;
        }
    };

    inline bool operator==(const AxisId &lhs, const AxisId &rhs) {
        return lhs.type == rhs.type && lhs.channel == rhs.channel;
    }

    /**
     * @brief representation of the data for an TCode Axis ramp
     */
    struct AxisRampData {
        float tangent;
        float weight;
        bool hasTangent;
        bool hasWeight;
        bool autoTangent;
    };

    /**
     * @brief Representation of a raw TCode Command
     */
    struct AxisData {
        float commandValue;
        unsigned long commandExtention;
        AxisExtentionType extentionType;
        AxisRampData rampIn;
        AxisRampData rampOut;
    };

    /**
     * @brief Axis state used to represent processed command data internally
     */
    struct AxisState {
        unsigned long startTime;
        unsigned long endTime;
        float startPosition;
        float endPosition;
        AxisRampData startRamp;
        AxisRampData endRamp;
    };

}