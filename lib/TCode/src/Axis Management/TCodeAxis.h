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
//#include <Arduino.h>
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

#define TCODE_MAX_AXIS_BUFFER_LENGTH 10

/**
 * @brief Class used to represent a TCode Axis
 */
class TCodeAxis
{
public:
    TCodeAxis() = delete;
    TCodeAxis(const char* name);

    //void addCommand(TCode_Axis_Command command);

    void set(int target_value, TCode_Axis_Extention_Type extention_type = TCode_Axis_Extention_Type::Time, long extention_value = 0, TCode_Axis_Ramp_Type ramp_type = TCode_Axis_Ramp_Type::Linear); // Function to set the axis dynamic parameters
    int getPosition();                                      // Function to return the current position of this axis
    void stop();                                            // Function to stop axis movement at current position

    bool changed(); // Function to check if an axis has changed since last getPosition
    const char* getName();           // Function to get the name for this axis

private:
    unsigned long getLastMillis();
    int getLastTargetPosition();
    int getPositionMapped(TCode_Axis_State &state, bool& inRange);
    unsigned long getDeltaTime(int start_position, int target_value, TCode_Axis_Extention_Type extention_type, long extention_value,TCode_Axis_Ramp_Type ramp_type);
    const char* axisName;
    int lastPosition;
    int minInterval; //minimum command interval 
    unsigned long last_command_time; // The last time this Axis was updated represented in millis 
    TCode_Axis_State currentState;
    TCodeBuffer<TCode_Axis_State,TCODE_MAX_AXIS_BUFFER_LENGTH> dataQueue;
    //TODO: implement queue system for commands to buffer them
};

#endif
