// TCode-Axis-Unbuffered-CPP v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 18/11/2023
// Please copy, share, learn, innovate, give attribution.

#ifndef TCODE_AXIS_UNBUFFERED_CPP
#define TCODE_AXIS_UNBUFFERED_CPP
#include "TCodeAxisUnbuffered.h"
#include <Arduino.h>

TCodeAxis::TCodeAxis(const char *name, TCode_ChannelID _channel)
{
    axisName = name;
    channel.channel = _channel.channel;
    channel.type = _channel.type;
    currentState.rampType = TCode_Axis_Ramp_Type::Linear;
    currentState.startValue = TCODE_DEFAULT_AXIS_RETURN_VALUE;
    currentState.endValue = TCODE_DEFAULT_AXIS_RETURN_VALUE;
    currentState.startTime = 0;
    currentState.endTime = 0;
    lastPosition = 0;
    minInterval = TCODE_MIN_AXIS_SMOOTH_INTERVAL;
}

void TCodeAxis::set(float targetValue, TCode_Axis_Extention_Type extentionType, long extentionValue, TCode_Axis_Ramp_Type rampType)
{
    unsigned long t = millis();
    unsigned long delta_time = 0;
    float startPosition = getPosition();
    switch (extentionType)
    {
        case TCode_Axis_Extention_Type::Speed:
        {
            delta_time = abs(targetValue - startPosition);
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
            {
                delta_time = extentionValue;
            }
            else
            {
                int lastInterval = t - currentState.startTime;
                if ((lastInterval > minInterval) && (minInterval < TCODE_MIN_AXIS_SMOOTH_INTERVAL))
                {
                    minInterval += 1;
                }
                else if ((lastInterval < minInterval) && (minInterval > TCODE_MAX_AXIS_SMOOTH_INTERVAL))
                {
                    minInterval -= 1;
                }

                delta_time = minInterval;
            }
        }
    }
    currentState.startTime = t;
    currentState.endTime = t + delta_time;
    currentState.rampType = rampType;
    currentState.startValue = startPosition;
    currentState.endValue = targetValue;
    lastCommandTime = t;
}

float TCodeAxis::getPosition()
{
    float x; // This is the current axis position, 0-9999
    unsigned long t = millis();


    if(t >= currentState.endTime)
    {
        return currentState.endValue;
    }

    if(t <= currentState.startTime)
    {
        return currentState.startValue;
    }
    //Serial.print("Test:");
    //Serial.println(t);
    //Serial.print("SV:");
    //Serial.println(currentState.startValue);
    //Serial.print("EV:");
    //Serial.println(currentState.endValue);

    switch (currentState.rampType)
    {
    case TCode_Axis_Ramp_Type::Linear:
        x = doubleMapf(t, currentState.startTime, currentState.endTime, currentState.startValue, currentState.endValue);
        break;
    case TCode_Axis_Ramp_Type::EaseIn:
        x = doubleMapEaseInf(t, currentState.startTime, currentState.endTime, currentState.startValue, currentState.endValue);
        break;
    case TCode_Axis_Ramp_Type::EaseOut:
        x = doubleMapEaseOutf(t, currentState.startTime, currentState.endTime, currentState.startValue, currentState.endValue);
        break;
    case TCode_Axis_Ramp_Type::EaseInOut:
        x = doubleMapEaseInOutf(t, currentState.startTime, currentState.endTime, currentState.startValue, currentState.endValue);
        break;
    default:
        x = doubleMapf(t, currentState.startTime, currentState.endTime, currentState.startValue, currentState.endValue);
    }

    if (x < 0)
        x = 0;

    return x;
}

void TCodeAxis::stop()
{
    unsigned long t = millis(); // This is the time now
    currentState.startValue = getPosition();
    currentState.endValue = currentState.startValue;
    currentState.startTime = t;
    currentState.endTime = t;

    if (channel.type == TCode_Channel_Type::Vibration)
    {
        currentState.endValue = 0;
        currentState.endTime = t + TCODE_MIN_AXIS_SMOOTH_INTERVAL;
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

unsigned long TCodeAxis::getLastCommandTime()
{
    return lastCommandTime;
}

#endif