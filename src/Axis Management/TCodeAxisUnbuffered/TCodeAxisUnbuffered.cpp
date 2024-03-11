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

void TCodeAxis::set(const TCode_Axis_Data &data)
{
    unsigned long t = millis();
    unsigned long deltaTime = 0;

    float startValue = getPosition();
    float endValue = clamp(data.commandValue, 0, 1);
    unsigned long extentionValue = data.commandExtention;

    switch (data.extentionType)
    {
        case TCode_Axis_Extention_Type::Speed:
        {
            deltaTime = abs(targetValue - startValue);
            deltaTime *= 100;
            if (extentionValue > 0)
            {
                deltaTime /= extentionValue;
            }
        }
        break;
        case TCode_Axis_Extention_Type::Time:
        default:
        {
            if (extentionValue > 0)
            {
                deltaTime = extentionValue;
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

                deltaTime = minInterval;
            }
        }
    }

    currentState.startTime = t;
    currentState.endTime = t + deltaTime;
    currentState.startValue = startValue;
    currentState.endValue = endValue;
    currentState.startRamp = currentState.data.rampOut;
    currentState.endRamp = data.rampIn;
    currentState.data = data;

    lastCommandTime = t;
}

float TCodeAxis::getPosition()
{
    unsigned long t = millis();
    if (t >= currentState.endTime)
        return currentState.endValue;
    if (t <= currentState.startTime)
        return currentState.startValue;

    //Serial.print("Test:");
    //Serial.println(t);
    //Serial.print("SV:");
    //Serial.println(currentState.startValue);
    //Serial.print("EV:");
    //Serial.println(currentState.endValue);

    float position = TCodeFloatingOperations::interpolatef(t, currentState.startTime, currentState.startValue, currentState.startRamp, currentState.endTime, currentState.endValue, currentState.endRamp);
    return clamp(position, 0, 1);
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