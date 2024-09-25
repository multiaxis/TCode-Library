// TCode-Floating-Point-functions-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 29/11/2023
// Please copy, share, learn, innovate, give attribution.
//
#include "TMath.h"


namespace TCode::TMath {
    float lerpf(float startValue, float endvalue, float t)
    {
        return (startValue + (endvalue - startValue) * t);
    }

    float mapf(float x, float inMin, float inMax, float outMin, float outMax) {
        const float run = inMax - inMin;
        if (run == 0) {
            return -1; // AVR returns -1, SAM returns 0
        }

        const float rise = outMax - outMin;
        const float delta = x - inMin;
        return (delta * rise) / run + outMin;
    }

    double mapd(double x, double inMin, double inMax, double outMin, double outMax) {
        const double run = inMax - inMin;
        if (run == 0) {
            return -1; // AVR returns -1, SAM returns 0
        }

        const double rise = outMax - outMin;
        const double delta = x - inMin;
        return (delta * rise) / run + outMin;
    }

    float interpolate(float currentX, float startX, float startValue, Datatypes::AxisRampData r0, float endX, float endValue, Datatypes::AxisRampData r1) {
        float out = 0.0;
        if(currentX < startX)
            return startValue;
        if(currentX > endX)
            return endValue;

        if (!r0.hasTangent && !r1.hasTangent) {
            // linear
            //LogHandler::warning("TMATH","Linear");
            out = mapd(currentX, startX, endX, startValue, endValue);
        } else if (!r0.hasWeight && !r1.hasWeight) {
            // cubic hermite
            float dt = currentX - startX;
            float t = dt / (endX - startX);

            float m0 = tan((PI / 2) * constrain(r0.tangent, -doubleLimit, doubleLimit));
            float m1 = tan((PI / 2) * constrain(r1.tangent, -doubleLimit, doubleLimit));

            float i = (2*t*t*t-3*t*t+1);
            float j = (t*t*t-2*t*t+t);
            float k = (-2*t*t*t+3*t*t);
            float l = (t*t*t-t*t);

            //LogHandler::warning("TMATH","Cubic");
            out = startValue * i + m0 * j + endValue * k + m1 * l;
        } else {
            // bezier
            float dt = currentX - startX;
            float t = dt / (endX - startX);

            float a0 = (PI / 2) * constrain(r0.tangent, -doubleLimit, doubleLimit);
            float a1 = (PI / 2) * constrain(r1.tangent, -doubleLimit, doubleLimit);

            float w0 = constrain(r0.weight, 0.0f, doubleLimit);
            float w1 = constrain(r1.weight, 0.0f, doubleLimit);
            
            float py0 = constrain(sin(a0) * w0, 0.0f, doubleLimit);
            float py1 = constrain(sin(a1) * w1, 0.0f, doubleLimit);

            out = lerpf(lerpf(startValue,py0,t),lerpf(py1,endValue,t),t);
        }
        return constrain(out,0.0f,TMath::doubleLimit);
    }

    unsigned long getTCodeFromFloat(float value, int precision, uint8_t &logOut) {
        float depsilon = pow10f(-precision);
        unsigned long out = 0;
        int log = 0;

        value = constrain(value, 0.0f, 1.0f);
        if (value < 1.0f) {
            while (value > 0 && log < precision) {
                value *= 10;

                int integerPart = (int)value;
                value -= integerPart;
                out = (out * 10) + integerPart;
                log++;

                if (value < depsilon || value > 1 - depsilon)
                    break;
            }

            if (value > 0.5)
                out += 1;
        } else {
            return (unsigned long)pow10(precision);
        }

        logOut = log;
        return out;
    }

    float getFloatFromTCode(unsigned long value, int precision) {
        return (double)value / pow10f(precision);
    }

    unsigned long getTCodeEstimateLogFromFloat(float value) {
        unsigned long log = 0;
        const unsigned long maxLog = 15;
        while (value > 0 && log < maxLog) {
            value *= 10;
            value -= (int)value;
            if (value < 0.5f)
                break;
        }

        return log;
    }

}
