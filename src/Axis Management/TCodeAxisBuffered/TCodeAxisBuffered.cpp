// TCode-Axis-Buffered-CPP v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 05/02/2022
// usage of this class can be found at (https://github.com/Dreamer2345/Arduino_TCode_Parser)
// Please copy, share, learn, innovate, give attribution.
// Container for TCode Axis's
// History:
//
#ifndef TCODE_AXIS_BUFFERED_CPP
#define TCODE_AXIS_BUFFERED_CPP
#include "TCodeAxisBuffered.h"
#include <Arduino.h>

TCodeAxisBuffered::TCodeAxisBuffered(const char *name, TCode_ChannelID _channel)
{
    currentState.rampType = TCode_Axis_Ramp_Type::Linear;
    currentState.startTime = 0;
    currentState.endTime = 0;
    currentState.startValue = TCODE_DEFAULT_AXIS_RETURN_VALUE;
    currentState.endValue = TCODE_DEFAULT_AXIS_RETURN_VALUE;
    axisName = name;
    lastCommandTime = 0;
    minInterval = TCODE_MIN_AXIS_SMOOTH_INTERVAL;
    channel = _channel;
}

unsigned long TCodeAxisBuffered::getDeltaTime(int start_position, int target_value, TCode_Axis_Extention_Type extentionType, long extentionValue, TCode_Axis_Ramp_Type rampType)
{
    unsigned long delta_time = 0;

    switch (extentionType)
    {
    case TCode_Axis_Extention_Type::Speed:
    {
        delta_time = abs(target_value - start_position);
        delta_time *= 100;
        if (extentionValue > 0)
        {
            delta_time /= extentionValue;
        }
    }
    break;
    case TCode_Axis_Extention_Type::Time:
    default:
    {
        if (extentionValue > 0)
            delta_time = extentionValue;
        else
            delta_time = minInterval;
    }
    }
    return delta_time;
}

void TCodeAxisBuffered::set(int target_value, TCode_Axis_Extention_Type extentionType, long extentionValue, TCode_Axis_Ramp_Type rampType)
{
    unsigned long t = getLastMillis(); // This is for adding this state
    target_value = constrain(target_value, 0, TCODE_MAX_AXIS);
    extentionValue = constrain(extentionValue, 0, TCODE_MAX_AXIS_MAGNITUDE);

    int last_target = getLastTargetPosition();
    unsigned long delta_time = getDeltaTime(last_target, target_value, extentionType, extentionValue, rampType);

    TCode_Axis_State new_state;
    new_state.startTime = t;
    new_state.endTime = t + delta_time;
    new_state.startValue = last_target;
    new_state.endValue = target_value;
    new_state.rampType = rampType;
    dataQueue.push(new_state);

    int lastInterval = millis() - lastCommandTime;
    int difference = abs(lastInterval - minInterval);
    if ((lastInterval > minInterval) && (minInterval < TCODE_MAX_AXIS_SMOOTH_INTERVAL))
    {
        minInterval += difference / 2;
    }
    else if ((lastInterval < minInterval) && (minInterval > TCODE_MIN_AXIS_SMOOTH_INTERVAL))
    {
        minInterval -= difference / 2;
    }
    minInterval = constrain(minInterval, TCODE_MIN_AXIS_SMOOTH_INTERVAL, TCODE_MAX_AXIS_SMOOTH_INTERVAL);
    lastCommandTime = millis();
}

unsigned long TCodeAxisBuffered::getLastCommandTime()
{
    return lastCommandTime;
}

unsigned long TCodeAxisBuffered::getLastMillis()
{
    unsigned long currentTime = millis();
    if (dataQueue.empty())
    {
        if (currentTime > currentState.endTime)
            return currentTime;
        else
            return currentState.endTime;
    }

    TCode_Axis_State backState;
    dataQueue.peekBack(backState);
    return backState.endTime;
}

int TCodeAxisBuffered::getLastTargetPosition()
{
    if (dataQueue.empty())
    {
        return currentState.endValue;
    }

    TCode_Axis_State backState;
    dataQueue.peekBack(backState);
    return backState.endValue;
}

int TCodeAxisBuffered::getPosition()
{
    bool isInRange = false;
    int value = getPositionMapped(currentState, isInRange);
    if (isInRange)
    {
        return value;
    }

    if (dataQueue.empty())
    {
        return value;
    }

    while (!isInRange)
    {
        dataQueue.pop(currentState);
        value = getPositionMapped(currentState, isInRange);
        if (dataQueue.empty())
            break;
    }

    return value;
}

int TCodeAxisBuffered::getPositionMapped(TCode_Axis_State &state, bool &inRange)
{
    int x; // This is the current axis position, 0-9999
    unsigned long t = millis();
    inRange = true;
    if (t >= state.endTime)
    {
        inRange = false;
        return state.endValue;
    }

    if (t < state.startTime)
    {
        inRange = false;
        return state.startValue;
    }

    switch (state.rampType)
    {
    case TCode_Axis_Ramp_Type::Linear:
        x = doubleMap(t, state.startTime, state.endTime, state.startValue, state.endValue);
        break;
    case TCode_Axis_Ramp_Type::EaseIn:
        x = doubleMapEaseIn(t, state.startTime, state.endTime, state.startValue, state.endValue);
        break;
    case TCode_Axis_Ramp_Type::EaseOut:
        x = doubleMapEaseOut(t, state.startTime, state.endTime, state.startValue, state.endValue);
        break;
    case TCode_Axis_Ramp_Type::EaseInOut:
        x = doubleMapEaseInOut(t, state.startTime, state.endTime, state.startValue, state.endValue);
        break;
    default:
        x = doubleMap(t, state.startTime, state.endTime, state.startValue, state.endValue);
    }

    if (x > TCODE_MAX_AXIS)
        x = TCODE_MAX_AXIS;
    if (x < 0)
        x = 0;

    return x;
}

void TCodeAxisBuffered::stop()
{
    unsigned long t = millis(); // This is the time now
    bool inRange;
    currentState.startValue = getPositionMapped(currentState, inRange);
    currentState.endValue = currentState.startValue;
    currentState.startTime = t;
    currentState.endTime = t;
    dataQueue.clear();

    if (channel.type == TCode_Channel_Type::Vibration)
    {
        currentState.endValue = 0;
        currentState.endTime = t + 100;
    }
    lastPosition = -1;
}

bool TCodeAxisBuffered::changed()
{
    if (lastPosition != getPosition())
    {
        lastPosition = getPosition();
        return true;
    }
    return false;
}

const char *TCodeAxisBuffered::getName()
{
    return axisName;
}

TCode_ChannelID TCodeAxisBuffered::getChannelID()
{
    return channel;
}

#endif