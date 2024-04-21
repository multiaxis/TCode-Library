// TCode-Axis-Unbuffered-CPP v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 18/11/2023
// Please copy, share, learn, innovate, give attribution.

#ifndef TCODE_AXIS_UNBUFFERED_CPP
#define TCODE_AXIS_UNBUFFERED_CPP
#include "TCodeAxisUnbuffered.h"
#include <Arduino.h>

UnbufferedAxis::UnbufferedAxis(const char *name, ChannelID _channel)
{
    axisName = name;
    channel.channel = _channel.channel;
    channel.type = _channel.type;
    currentState.startValue = TCODE_DEFAULT_AXIS_RETURN_VALUE;
    currentState.endValue = TCODE_DEFAULT_AXIS_RETURN_VALUE;
    currentState.startTime = 0;
    currentState.endTime = 0;
    lastPosition = 0;
    minInterval = TCODE_MIN_AXIS_SMOOTH_INTERVAL;
}

void UnbufferedAxis::set(const AxisData &data)
{
    unsigned long currentTime = millis();
    unsigned long deltaTime = 0;

    float startValue = getPosition();
    float endValue = constrain(data.commandValue, 0, 1);
    unsigned long extentionValue = data.commandExtention;

    switch (data.extentionType)
    {
        case AxisExtentionType::SPEED:
        {
            deltaTime = abs(endValue - startValue);
            deltaTime *= 100;
            if (extentionValue > 0)
            {
                deltaTime /= extentionValue;
            }
        }
        break;
        case AxisExtentionType::TIME:
        default:
        {
            if (extentionValue > 0)
            {
                deltaTime = extentionValue;
            }
            else
            {
                int lastInterval = currentTime - currentState.startTime;
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

    currentState.startTime = currentTime;
    currentState.endTime = currentTime + deltaTime;
    currentState.startValue = startValue;
    currentState.endValue = endValue;
    currentState.startRamp = currentState.data.rampOut;
    currentState.endRamp = data.rampIn;
    currentState.data = data;

    if (deltaTime != 0)
    {
        float deltaValue = endValue - startValue;
        float easingTangent = 2 / PI * atan(2 * deltaValue / deltaTime); // approximates easing functions
        if (currentState.startRamp.autoTangent)
        {
            currentState.startRamp.tangent = easingTangent;
        }
        if (currentState.endRamp.autoTangent)
        {
            currentState.endRamp.tangent = easingTangent;
        }
    }

    lastCommandTime = currentTime;
}

float UnbufferedAxis::getPosition()
{
    unsigned long currentTime = millis();
    if (currentTime >= currentState.endTime)
        return currentState.endValue;
    if (currentTime <= currentState.startTime)
        return currentState.startValue;

    //Serial.print("Test:");
    //Serial.println(currentTime);
    //Serial.print("SV:");
    //Serial.println(currentState.startValue);
    //Serial.print("EV:");
    //Serial.println(currentState.endValue);

    float position = TCodeFloatingOperations::interpolatef(currentTime, currentState.startTime, currentState.startValue, currentState.startRamp, currentState.endTime, currentState.endValue, currentState.endRamp);
    return constrain(position, 0, 1);
}

void UnbufferedAxis::stop()
{
    unsigned long currentTime = millis(); // This is the time now
    currentState.startValue = getPosition();
    currentState.endValue = currentState.startValue;
    currentState.startTime = currentTime;
    currentState.endTime = currentTime;

    if (channel.type == ChannelType::VIBRATION)
    {
        currentState.endValue = 0;
        currentState.endTime = currentTime + TCODE_MIN_AXIS_SMOOTH_INTERVAL;
    }
}

bool UnbufferedAxis::changed()
{
    if (lastPosition != getPosition())
    {
        lastPosition = getPosition();
        return true;
    }
    return false;
}

const char *UnbufferedAxis::getName()
{
    return axisName;
}

ChannelID UnbufferedAxis::getChannelID()
{
    return channel;
}

unsigned long UnbufferedAxis::getLastCommandTime()
{
    return lastCommandTime;
}

#endif