// TCode-Axis-Interface-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 18/11/2023
#pragma once
#ifndef TCODE_AXIS_INTERFACE_H
#define TCODE_AXIS_INTERFACE_H
#include "Constants and Enums/TCodeEnums.h"


class ITCodeAxis
{
public:
    /**
     * @brief sets the axis' dynamic parameters
     * @param target_value the target value for this axis
     * @param extentionType the extention type for the axis e.g. Time,Speed
     * @param extentionValue the extention value
     * @param rampType the ramp type
     */
    virtual void set(float targetValue, TCode_Axis_Extention_Type extentionType = TCode_Axis_Extention_Type::Time, long extentionValue = 0, TCode_Axis_Ramp_Type rampType = TCode_Axis_Ramp_Type::Linear) = 0;
    /**
     * @brief gets the current position of the axis
     * @returns current position of this axis as a float
     */
    virtual float getPosition() = 0;
    /**
     * @brief stops axis movement at its current position
     */
    virtual void stop() = 0;
    /**
     * @brief stops axis movement at its current position
     * @returns returns true if the axis has changed position since last check
     */
    virtual bool changed() = 0;
    /**
     * @brief gets the name of the axis
     * @returns returns the c_string representation of the name stored
     */
    virtual const char *getName() = 0;
    /**
     * @brief gets channel ID of the axis
     * @returns returns the channel ID of the axis
     */
    virtual TCode_ChannelID getChannelID() = 0;

    /**
     * @brief gets last time a command was inputted for this axis
     * @returns last time in millis since command was inputted
     */
    virtual unsigned long getLastCommandTime() = 0;
protected:
    unsigned long lastCommandTime;
    const char *axisName;
    TCode_ChannelID channel;
};



#endif