// TCode-Axis-Unbuffered-CPP v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 18/11/2023
// Please copy, share, learn, innovate, give attribution.
#include "TAxis.h"
#include "../utils/TMath.h"

using namespace TCode;

TAxis::TAxis(const char *name, AxisId id, float defaultPosition) {
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

void TAxis::set(const AxisData &data) {
    unsigned long currentTime = millis();
    unsigned long deltaTime = 0;

    float startValue = getPosition();
    float endValue = constrain(data.commandValue, 0, 1);
    unsigned long extentionValue = data.commandExtention;

    switch (data.extentionType) {
        case AxisExtentionType::Speed:
        {
            deltaTime = abs(endValue - startValue);
            deltaTime *= 100;
            if (extentionValue > 0) {
                deltaTime /= extentionValue;
            }
            break;
        }
        case AxisExtentionType::Time:
        default:
        {
            if (extentionValue > 0) {
                deltaTime = extentionValue;
            } else {
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
    currentState.startRamp = currentState.data.rampOut;
    currentState.endRamp = data.rampIn;
    currentState.data = data;

    if (deltaTime != 0) {
        float deltaValue = endValue - startValue;
        if (!currentState.startRamp.hasTangent && currentState.startRamp.autoTangent) {
            currentState.startRamp.tangent = 2 / PI * atan(2 * deltaValue / deltaTime); // approximates easing function
            currentState.startRamp.hasTangent = true;
        }

        if (!currentState.endRamp.hasTangent && currentState.endRamp.autoTangent) {
            currentState.endRamp.tangent = 2 / PI * atan(2 * deltaValue / deltaTime); // approximates easing function
            currentState.endRamp.hasTangent = true;
        }
    }

    lastCommandTime = currentTime;
}

float TAxis::getPosition()
{
    unsigned long currentTime = constrain(millis(), currentState.startTime, currentState.endTime);
    float position = TMath::interpolate(currentTime, currentState.startTime, currentState.startPosition, currentState.startRamp, currentState.endTime, currentState.endPosition, currentState.endRamp);
    return constrain(position, 0, 1);
}

void TAxis::stop() {
    unsigned long currentTime = millis();

    currentState.startPosition = getPosition();
    currentState.startTime = currentTime;
    currentState.endTime = currentTime;
    currentState.endPosition = id.type == AxisType::Vibration ? defaultPosition : currentState.startPosition; //TODO:
}

bool TAxis::changed() {
    if (lastPosition != getPosition()) {
        lastPosition = getPosition();
        return true;
    }

    return false;
}

const char *TAxis::getName() {
    return name;
}

AxisId TAxis::getId() {
    return id;
}

unsigned long TAxis::getLastCommandTime() {
    return lastCommandTime;
}
