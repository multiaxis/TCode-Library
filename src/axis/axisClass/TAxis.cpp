// TCode-Axis-Unbuffered-CPP v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 18/11/2023
// Please copy, share, learn, innovate, give attribution.
#include "TAxis.h"
#include "../../utils/TMath.h"

namespace TCode 
{

TCodeAxis::TCodeAxis(const char *name, AxisId id, float defaultPosition)
{
    this->name = name;
    this->id = id;
    this->defaultPosition = defaultPosition;

    currentState.startPosition = defaultPosition;
    currentState.endPosition = defaultPosition;
    currentState.startTime = 0;
    currentState.endTime = 0;
    lastPosition = 0;
    minInterval = TCODE_MIN_AXIS_SMOOTH_INTERVAL;
}

void TCodeAxis::set(const AxisData &data)
{
    unsigned long currentTime = millis();
    unsigned long deltaTime = 0;

    float startValue = getPosition();
    float endValue = constrain(data.commandValue, 0.0, 1.0);
    unsigned long extentionValue = data.commandExtention;

    switch (data.extentionType)
    {
    case AxisExtentionType::Speed:
    {
        deltaTime = abs(endValue - startValue);
        deltaTime *= 100;
        if (extentionValue > 0)
        {
            deltaTime /= extentionValue;
        }
        break;
    }
    case AxisExtentionType::Time:
    default:
    {
        if (extentionValue > 0)
        {
            deltaTime = extentionValue;
        }
        else
        {
            int lastInterval = currentTime - currentState.startTime;
            if (lastInterval > minInterval && minInterval < TCODE_MIN_AXIS_SMOOTH_INTERVAL)
                minInterval += 1;
            else if (lastInterval < minInterval && minInterval > TCODE_MAX_AXIS_SMOOTH_INTERVAL)
                minInterval -= 1;

            deltaTime = minInterval;
        }
        break;
    }
    }

    currentState.startTime = currentTime;
    currentState.endTime = currentTime + deltaTime;
    currentState.startPosition = startValue;
    currentState.endPosition = endValue;
    currentState.startRamp = currentState.endRamp;
    currentState.endRamp = data.rampIn;

    if (deltaTime != 0)
    {
        float deltaValue = endValue - startValue;
        float easingTangent = 2 / PI * atan(2 * deltaValue / deltaTime); // approximates easing functions
        if (currentState.startRamp.autoTangent)
            currentState.startRamp.tangent = easingTangent;

        if (currentState.endRamp.autoTangent)
            currentState.endRamp.tangent = easingTangent;
    }

    lastCommandTime = currentTime;
}

float TCodeAxis::getPosition()
{
    unsigned long currentTime = constrain(millis(), currentState.startTime, currentState.endTime);
    float position = TMath::interpolate(currentTime, currentState.startTime, currentState.startPosition, currentState.startRamp, currentState.endTime, currentState.endPosition, currentState.endRamp);
    return constrain(position, 0.0, 1.0);
}

void TCodeAxis::stop()
{
    unsigned long currentTime = millis();

    currentState.startPosition = getPosition();
    currentState.startTime = currentTime;
    currentState.endTime = currentTime;
    currentState.endPosition = id.type == AxisType::Vibration ? defaultPosition : currentState.startPosition; // TODO:
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
    return name;
}

AxisId TCodeAxis::getId()
{
    return id;
}

unsigned long TCodeAxis::getLastCommandTime()
{
    return lastCommandTime;
}

};