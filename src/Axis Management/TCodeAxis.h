// TCode-Axis-H v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 05/02/2022
// usage of this class can be found at (https://github.com/Dreamer2345/Arduino_TCode_Parser)
// Please copy, share, learn, innovate, give attribution.
// Container for TCode Axis's
// History:
//
#pragma once
#ifndef TCODE_AXIS_H
#define TCODE_AXIS_H
// #include <Arduino.h>
#include "../Constants and Enums/TCodeConstants.h"
#include "../Constants and Enums/TCodeEnums.h"
#include "../Utils/TCodeFixed.h"
#include "../Utils/TCodeBuffer.h"

/**
 * @brief Value used to define the default value for a TCode Axis
 */
#define TCODE_DEFAULT_AXIS_RETURN_VALUE 5000;

/**
 * @brief Value used to define the auto-smooth interval minimum range used in live commands (ms)
 */
#define TCODE_MIN_AXIS_SMOOTH_INTERVAL 3 // Minimum auto-smooth ramp interval for live commands (ms)

/**
 * @brief Value used to define the auto-smooth interval maximum range used in live commands (ms)
 */
#define TCODE_MAX_AXIS_SMOOTH_INTERVAL 100 // Maximum auto-smooth ramp interval for live commands (ms)

/**
 * @brief Value used to define the length the buffer used in the TCode Axis class
 */
#define TCODE_MAX_AXIS_BUFFER_LENGTH 10

/**
 * @brief Class used to represent a TCode Axis
 */
class TCodeAxis
{
public:
    TCodeAxis() = delete;
    /**
     * @brief constructor for the TCodeAxis class
     * @param name constant char string name for the axis
     * @param channel channel ID for this axis
     */
    TCodeAxis(const char *name, TCode_ChannelID channel);

    /**
     * @brief sets the axis' dynamic parameters
     * @param target_value the target value for this axis
     * @param extentionType the extention type for the axis e.g. Time,Speed
     * @param extentionValue the extention value
     * @param rampType the ramp type
     */
    void set(int targetValue, TCode_Axis_Extention_Type extentionType = TCode_Axis_Extention_Type::Time, long extentionValue = 0, TCode_Axis_Ramp_Type rampType = TCode_Axis_Ramp_Type::Linear);
    /**
     * @brief gets the current position of the axis
     * @returns current position of this axis as an int
     */
    int getPosition();
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
    TCode_ChannelID getChannelID();

    /**
     * @brief gets last time a command was inputted for this axis
     * @returns last time in millis since command was inputted
     */
    unsigned long getLastCommandTime();

private:
    unsigned long getLastMillis();
    int getLastTargetPosition();
    int getPositionMapped(TCode_Axis_State &state, bool &inRange);
    unsigned long getDeltaTime(int startPosition, int targetValue, TCode_Axis_Extention_Type extentionType, long extentionValue, TCode_Axis_Ramp_Type rampType);
    const char *axisName;
    int lastPosition;
    int minInterval;               // minimum command interval
    unsigned long lastCommandTime; // The last time this Axis was updated represented in millis
    TCode_Axis_State currentState;
    TCode_ChannelID channel;
    TCodeBuffer<TCode_Axis_State, TCODE_MAX_AXIS_BUFFER_LENGTH> dataQueue;
};

#endif
