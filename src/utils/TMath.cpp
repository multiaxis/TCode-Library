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
        if(currentX <= startX)
            return startValue;
        if(currentX >= endX)
            return endValue;

        if (!r0.hasTangent && !r1.hasTangent) {
            // linear
            //LogHandler::warning("TMATH","Linear");
            out = mapf(currentX, startX, endX, startValue, endValue);
        } else if (!r0.hasWeight && !r1.hasWeight) {
            // cubic hermite

            float dcx = currentX - startX;
            float dx = dcx / (endX - startX);

            float m0 = tan(HALF_PI * r0.tangent);
            float m1 = tan(HALF_PI * r1.tangent);

            float dx2 = dx * dx;
            float dx3 = dx * dx * dx;

            float i = (2*dx3-3*dx2+1);
            float j = (dx3-2*dx2+dx);
            float k = (-2*dx3+3*dx2);
            float l = (dx3-dx2);

            //LogHandler::warning("TMATH","Cubic");
            out = startValue * i + m0 * j + endValue * k + m1 * l;
        } else {
            // bezier
            float dx = endX - startX;
            float dy = endValue - startValue;

            float m0 = tan(HALF_PI * r0.tangent);
            float m1 = tan(HALF_PI * r1.tangent);

            float w0 = r0.weight;
            float w1 = r1.weight;
            float w1s = 1 - w1;

            float ts;
            float t = 0.5f;
            float tx = (currentX - startX) / dx;

            if ((abs(w0 - (1.0 / 3.0f)) < error) && (abs(w1 - (1.0 / 3.0f)) < error)) {
                t = tx;
                ts = 1 - t;
            } else {
                int count = 0;
                while (true) {
                    ts = 1 - t;

                    float t2 = t * t;
                    float ts2 = ts * ts;

                    float fg = 3 * ts2 * t * w0 + 3 * ts * t2 * w1s + t2 * t - tx;
                    if (abs(fg) < error)
                        break;
                    if (count > 5)
                        break;

                    // third order householder method
                    float fpg = 3 * ts2 * w0 + 6 * ts * t * (w1s - w0) + 3 * t2 * (1 - w1s);
                    float fppg = 6 * ts * (w1s - 2 * w0) + 6 * t * (1 - 2 * w1s + w0);
                    float fpppg = 18 * w0 - 18 * w1s + 6;

                    float fg2 = fg * fg;
                    float fpg2 = fpg * fpg;
                    t -= (6 * fg * fpg2 - 3 * fg2 * fppg) / (6 * fpg2 * fpg - 6 * fg * fpg * fppg + fg2 * fpppg);
                    count++;
                }
            }
            //LogHandler::warning("TMATH","Bezier");
            float t2 = t * t;
            out = startValue + 3 * ts * ts * t * w0 * m0 * dx + 3 * ts * t2 * (dy - w1 * m1 * dx) + t2 * t * dy;
        }
        return constrain(out,0.0f,doubleLimit);
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
