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
    currentState.rampType = AxisRampType::LINEAR;
    currentState.startValue = TCODE_DEFAULT_AXIS_RETURN_VALUE;
    currentState.endValue = TCODE_DEFAULT_AXIS_RETURN_VALUE;
    currentState.startTime = 0;
    currentState.endTime = 0;
    lastPosition = 0;
    minInterval = TCODE_MIN_AXIS_SMOOTH_INTERVAL;
}

void UnbufferedAxis::set(float targetValue, AxisExtentionType extentionType, long extentionValue, AxisRampType rampType)
{
    unsigned long currentTime = millis();
    unsigned long deltaTime = 0;
    float startPosition = getPosition();
    switch (extentionType)
    {
        case AxisExtentionType::SPEED:
        {
            deltaTime = abs(targetValue - startPosition);
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

    if(targetValue > 1.0)
        targetValue = 1.0;
    if(targetValue < 0.0)
        targetValue = 0.0;

    currentState.startTime = currentTime;
    currentState.endTime = currentTime + deltaTime;
    currentState.rampType = rampType;
    currentState.startValue = startPosition;
    currentState.endValue = targetValue;
    lastCommandTime = currentTime;
}

float UnbufferedAxis::getPosition()
{
    float x; // This is the current axis position, 0.0..1.0
    unsigned long currentTime = millis();


    if(currentTime >= currentState.endTime)
    {
        return currentState.endValue;
    }

    if(currentTime <= currentState.startTime)
    {
        return currentState.startValue;
    }
    //Serial.print("Test:");
    //Serial.println(currentTime);
    //Serial.print("SV:");
    //Serial.println(currentState.startValue);
    //Serial.print("EV:");
    //Serial.println(currentState.endValue);

    switch (currentState.rampType)
    {
    case AxisRampType::LINEAR:
        x = TCodeFloatingOperations::doubleMapf(currentTime, currentState.startTime, currentState.endTime, currentState.startValue, currentState.endValue);
        break;
    case AxisRampType::EASEIN:
        x = TCodeFloatingOperations::doubleMapEaseInf(currentTime, currentState.startTime, currentState.endTime, currentState.startValue, currentState.endValue);
        break;
    case AxisRampType::EASEOUT:
        x = TCodeFloatingOperations::doubleMapEaseOutf(currentTime, currentState.startTime, currentState.endTime, currentState.startValue, currentState.endValue);
        break;
    case AxisRampType::EASEINOUT:
        x = TCodeFloatingOperations::doubleMapEaseInOutf(currentTime, currentState.startTime, currentState.endTime, currentState.startValue, currentState.endValue);
        break;
    default:
        x = TCodeFloatingOperations::doubleMapf(currentTime, currentState.startTime, currentState.endTime, currentState.startValue, currentState.endValue);
    }

    if (x < 0)
        x = 0;

    return x;
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