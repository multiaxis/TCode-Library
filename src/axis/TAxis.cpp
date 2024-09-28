// TCode-Axis-Unbuffered-CPP v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 18/11/2023
// Please copy, share, learn, innovate, give attribution.
#include "TAxis.h"
#include "../utils/TMath.h"

namespace TCode::Axis {

    TCodeAxis::TCodeAxis(const char *name, Datatypes::AxisId id, float defaultPosition) {
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

    void TCodeAxis::set(const Datatypes::AxisData &data) {
        unsigned long currentTime = millis();
        unsigned long deltaTime = 0;

        float startValue = getPosition();
        float endValue = constrain(data.commandValue, 0.0, 1.0);
        unsigned long extentionValue = data.commandExtention;

        switch (data.extentionType) {
        case Datatypes::AxisExtentionType::Speed: {
            deltaTime = abs((endValue*10000) - (startValue*10000));
            deltaTime *= 100;
            if (extentionValue > 0) {
                deltaTime /= extentionValue;
            }
            break;
        }
        case Datatypes::AxisExtentionType::Time:
        default: {
            if (extentionValue > 0) {
                deltaTime = extentionValue;
            } else {
                int lastInterval = currentTime - currentState.startTime;

                if (lastInterval > minInterval && minInterval < TCODE_MAX_AXIS_SMOOTH_INTERVAL)
                    minInterval += 1;
                else if (lastInterval < minInterval && minInterval > TCODE_MIN_AXIS_SMOOTH_INTERVAL)
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

        if (deltaTime != 0) {
            float deltaValue = endValue - startValue;
            float easingTangent = (2 / PI) * atan(2 * deltaValue / deltaTime); // approximates easing functions
            if (currentState.startRamp.autoTangent)
                currentState.startRamp.tangent = easingTangent;

            if (currentState.endRamp.autoTangent)
                currentState.endRamp.tangent = easingTangent;
        }

        lastCommandTime = currentTime;
    }

    const float TCodeAxis::getPosition() {
        unsigned long currentTimeMicros = micros();
        unsigned long currentTime = millis();
        float microsFraction;
        modf(((float)currentTimeMicros / 1000.0f),&microsFraction);
        float currentTimeFloat = currentTime + microsFraction;
        float position = TMath::interpolate(currentTimeFloat, currentState.startTime, currentState.startPosition, currentState.startRamp, currentState.endTime, currentState.endPosition, currentState.endRamp);
        return position;
    }

    void TCodeAxis::stop() {
        unsigned long currentTime = millis();

        currentState.startRamp.hasWeight = false;
        currentState.startRamp.hasTangent = false;

        currentState.endRamp.hasWeight = false;
        currentState.endRamp.hasTangent = false;

        currentState.startPosition = getPosition();
        currentState.startTime = currentTime;
        currentState.endTime = currentTime + 30;
        currentState.endPosition = id.type == Datatypes::AxisType::Vibration ? defaultPosition : currentState.startPosition;
    }

    const bool TCodeAxis::changed() {
        if (lastPosition != getPosition()) {
            lastPosition = getPosition();
            return true;
        }

        return false;
    }

    const char *TCodeAxis::getName() {
        return name;
    }

    const Datatypes::AxisId TCodeAxis::getId() {
        return id;
    }

    const unsigned long TCodeAxis::getLastCommandTime() {
        return lastCommandTime;
    }

};