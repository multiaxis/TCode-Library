// TCode-Axis-Unbuffered-CPP v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 18/11/2023
// Please copy, share, learn, innovate, give attribution.
#include "TAxis.h"
#include "../utils/TMath.h"

using namespace TCode;

TAxis::TAxis(const char *name, ChannelID channel, float defaultValue)
{
    this->name = name;
    this->channel = channel;

    currentState.startValue = defaultValue;
    currentState.endValue = defaultValue;
    currentState.startTime = 0;
    currentState.endTime = 0;
    lastPosition = 0;
    minInterval = TCODE_MIN_AXIS_SMOOTH_INTERVAL;
}

void TAxis::set(const AxisData &data)
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

float TAxis::getPosition()
{
    unsigned long currentTime = constrain(millis(), currentState.startTime, currentState.endTime);
    float position = TMath::interpolate(currentTime, currentState.startTime, currentState.startValue, currentState.startRamp, currentState.endTime, currentState.endValue, currentState.endRamp);
    return constrain(position, 0, 1);
}

void TAxis::stop()
{
    unsigned long currentTime = millis();

    currentState.startValue = getPosition();
    currentState.endValue = currentState.startValue;
    currentState.startTime = currentTime;
    currentState.endTime = currentTime;

    if (channel.type == ChannelType::VIBRATION) //TODO:
    {
        currentState.endValue = 0;
        currentState.endTime = currentTime + TCODE_MIN_AXIS_SMOOTH_INTERVAL;
    }
}

bool TAxis::changed()
{
    if (lastPosition != getPosition())
    {
        lastPosition = getPosition();
        return true;
    }
    return false;
}

const char *TAxis::getName()
{
    return name;
}

ChannelID TAxis::getChannelID()
{
    return channel;
}

unsigned long TAxis::getLastCommandTime()
{
    return lastCommandTime;
}
