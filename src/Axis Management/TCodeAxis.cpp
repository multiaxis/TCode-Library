#ifndef TCODE_AXIS_CPP
#define TCODE_AXIS_CPP
#include "TCodeAxis.h"
#include <Arduino.h>

TCodeAxis::TCodeAxis(const char *name, TCode_ChannelID _channel)
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

unsigned long TCodeAxis::getDeltaTime(int start_position, int target_value, TCode_Axis_Extention_Type extentionType, long extentionValue, TCode_Axis_Ramp_Type rampType)
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

void TCodeAxis::set(int target_value, TCode_Axis_Extention_Type extentionType, long extentionValue, TCode_Axis_Ramp_Type rampType)
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

unsigned long TCodeAxis::getLastCommandTime()
{
    return lastCommandTime;
}

unsigned long TCodeAxis::getLastMillis()
{
    unsigned long currentTime = millis();
    if (dataQueue.isEmpty())
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

int TCodeAxis::getLastTargetPosition()
{
    if (dataQueue.isEmpty())
    {
        return currentState.endValue;
    }

    TCode_Axis_State backState;
    dataQueue.peekBack(backState);
    return backState.endValue;
}

int TCodeAxis::getPosition()
{
    bool isInRange = false;
    int value = getPositionMapped(currentState, isInRange);
    if (isInRange)
    {
        return value;
    }

    if (dataQueue.isEmpty())
    {
        return value;
    }

    while (!isInRange)
    {
        dataQueue.pop(currentState);
        value = getPositionMapped(currentState, isInRange);
        if (dataQueue.isEmpty())
            break;
    }

    return value;
}

int TCodeAxis::getPositionMapped(TCode_Axis_State &state, bool &inRange)
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
        x = mapQ16(t, state.startTime, state.endTime, state.startValue, state.endValue);
        break;
    case TCode_Axis_Ramp_Type::EaseIn:
        x = mapEaseIn(t, state.startTime, state.endTime, state.startValue, state.endValue);
        break;
    case TCode_Axis_Ramp_Type::EaseOut:
        x = mapEaseOut(t, state.startTime, state.endTime, state.startValue, state.endValue);
        break;
    case TCode_Axis_Ramp_Type::EaseInOut:
        x = mapEaseInOut(t, state.startTime, state.endTime, state.startValue, state.endValue);
        break;
    default:
        x = mapQ16(t, state.startTime, state.endTime, state.startValue, state.endValue);
    }

    if (x > TCODE_MAX_AXIS)
        x = TCODE_MAX_AXIS;
    if (x < 0)
        x = 0;

    return x;
}

void TCodeAxis::stop()
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
}

bool TCodeAxis::changed()
{
    if (lastPosition != getPosition())
    {
        lastPosition = getPosition();
        return true;
    }
    return false;
}

const char *TCodeAxis::getName()
{
    return axisName;
}

TCode_ChannelID TCodeAxis::getChannelID()
{
    return channel;
}

#endif