// TCode-Axis-Unbuffered-H v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 18/11/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once
#ifndef TCODE_AXIS_UNBUFFERED_H
#define TCODE_AXIS_UNBUFFERED_H
// #include <Arduino.h>
#include "../../Constants and Enums/TCodeConstants.h"
#include "../../Constants and Enums/TCodeEnums.h"
#include "../../Utils/TCodeFloating.h"
#include "../ITCodeAxis.h"


/**
 * @brief Class used to represent a TCode Axis
 */
class UnbufferedAxis : public ITCodeAxis
{
public:
    UnbufferedAxis() = delete;
    /**
     * @brief constructor for the TCodeAxisBuffered class
     * @param name constant char string name for the axis
     * @param channel channel ID for this axis
     */
    UnbufferedAxis(const char *name, ChannelID _channel);

    /**
     * @brief sets the axis' dynamic parameters
     * @param target_value the target value for this axis
     * @param extentionType the extention type for the axis e.g. TIME,SPEED
     * @param extentionValue the extention value
     * @param rampType the ramp type
     */
    void set(float targetValue, AxisExtentionType extentionType = AxisExtentionType::TIME, long extentionValue = 0, AxisRampType rampType = AxisRampType::LINEAR);
    
    /**
     * @brief gets the current position of the axis
     * @returns current position of this axis as an int
     */
    float getPosition();
    /**
     * @brief stops axis movement at its current position
     */
    void stop();
    /**
     * @brief stops axis movement at its current position
     * @returns returns true if the axis has changed position since last check
     */
    bool changed();
    /**
     * @brief gets the name of the axis
     * @returns returns the c_string representation of the name stored
     */
    const char *getName();
    /**
     * @brief gets channel ID of the axis
     * @returns returns the channel ID of the axis
     */
    ChannelID getChannelID();

    /**
     * @brief gets last time a command was inputted for this axis
     * @returns last time in millis since command was inputted
     */
    unsigned long getLastCommandTime();

private:
    float lastPosition;
    int minInterval;               // minimum command interval
    AxisState currentState;
};

#endif