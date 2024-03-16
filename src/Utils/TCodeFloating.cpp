// TCode-Floating-Point-functions-CPP v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 29/11/2023
// Please copy, share, learn, innovate, give attribution.
//
#include "TCodeFloating.h"

double lerpDouble(double start, double stop, double t)
{
    return (start * (1 - t)) + (stop * t);
}

double doubleEaseIn(double t)
{
    if (t < 0)
        return 0.0f;
    if (t > 1)
        return 1.0f;
    return t * t;
}

double doubleEaseOut(double t)
{
    if (t < 0)
        return 1.0f;
    if (t > 1)
        return 0.0f;

    t = 1.0 - t;
    t = t * t;
    t = 1.0 - t;
    return t;
}

double constrainDouble(double doubleValue, double min, double max)
{
    if (min > max)
    {
        double temp = min;
        min = max;
        max = temp;
    }

    if (doubleValue < min)
        return min;
    if (doubleValue > max)
        return max;
    return doubleValue;
}

long TCodeFloatingOperations::doubleMap(long in, long inStart, long inEnd, long outStart, long outEnd)
{
    double t = in - inStart;
    t = t / (inEnd - inStart);
    t = constrainDouble(t, 0.0, 1.0);
    t = t * (outEnd - outStart);
    t += outStart;
    t = constrainDouble(t, outStart, outEnd);
    return t;
}

long TCodeFloatingOperations::doubleMapEaseIn(long in, long inStart, long inEnd, long outStart, long outEnd)
{
    double t = in - inStart;
    t = t / (inEnd - inStart);
    t = doubleEaseIn(t);
    t = constrainDouble(t, 0.0, 1.0);
    t = t * (outEnd - outStart);
    t += outStart;
    t = constrainDouble(t, outStart, outEnd);
    return t;
}

long TCodeFloatingOperations::doubleMapEaseOut(long in, long inStart, long inEnd, long outStart, long outEnd)
{
    double t = in - inStart;
    t = t / (inEnd - inStart);
    t = doubleEaseOut(t);
    t = constrainDouble(t, 0.0, 1.0);
    t = t * (outEnd - outStart);
    t += outStart;
    t = constrainDouble(t, outStart, outEnd);
    return t;
}

long TCodeFloatingOperations::doubleMapEaseInOut(long in, long inStart, long inEnd, long outStart, long outEnd)
{
    double t = in - inStart;
    t = t / (inEnd - inStart);
    t = lerpDouble(doubleEaseIn(t), doubleEaseOut(t), t);
    t = constrainDouble(t, 0.0, 1.0);
    t = t * (outEnd - outStart);
    t += outStart;
    t = constrainDouble(t, outStart, outEnd);
    return t;
}

float TCodeFloatingOperations::doubleMapf(double in, double inStart, double inEnd, double outStart, double outEnd)
{
    double t = in - inStart;
    t = t / (inEnd - inStart);
    t = constrainDouble(t, 0.0, 1.0);
    t = t * (outEnd - outStart);
    t += outStart;
    t = constrainDouble(t, outStart, outEnd);
    return t;
}

float TCodeFloatingOperations::doubleMapEaseInf(double in, double inStart, double inEnd, double outStart, double outEnd)
{
    double t = in - inStart;
    t = t / (inEnd - inStart);
    t = doubleEaseIn(t);
    t = constrainDouble(t, 0.0, 1.0);
    t = t * (outEnd - outStart);
    t += outStart;
    t = constrainDouble(t, outStart, outEnd);
    return t;
}

float TCodeFloatingOperations::doubleMapEaseOutf(double in, double inStart, double inEnd, double outStart, double outEnd)
{
    double t = in - inStart;
    t = t / (inEnd - inStart);
    t = doubleEaseOut(t);
    t = constrainDouble(t, 0.0, 1.0);
    t = t * (outEnd - outStart);
    t += outStart;
    t = constrainDouble(t, outStart, outEnd);
    return t;
}

float TCodeFloatingOperations::doubleMapEaseInOutf(double in, double inStart, double inEnd, double outStart, double outEnd)
{
    double t = in - inStart;
    t = t / (inEnd - inStart);
    t = lerpDouble(doubleEaseIn(t), doubleEaseOut(t), t);
    t = constrainDouble(t, 0.0, 1.0);
    t = t * (outEnd - outStart);
    t += outStart;
    t = constrainDouble(t, outStart, outEnd);
    return t;
}

float TCodeFloatingOperations::interpolatef(float x, float x0, float y0, TCode_Axis_Ramp_Data r0, float x1, float y1, TCode_Axis_Ramp_Data r1)
{
    if (!r0.hasTangent && !r1.hasTangent)
    {
        //linear
        return doubleMap(x, x0, x1, y0, y1);
    }
    else if (!r0.hasWeight && !r1.hasWeight)
    {
        //cubic hermite
        float d = x1 - x0;
        float dx = x - x0;
        float t = dx / d;
        float r = 1 - t;

        float m0 = tan(PI / 2 * constrain(r0.tangent, -0.999f, 0.999f));
        float m1 = tan(PI / 2 * constrain(r1.tangent, -0.999f, 0.999f));

        return r * r * (y0 * (1 + 2 * t) + m0 * dx)
             + t * t * (y1 * (3 - 2 * t) - d * m1 * r);
    }
    else
    {
        //bezier
        float dx = x1 - x0;
        float dy = y1 - y0;

        float m0 = tan(PI / 2 * constrain(r0.tangent, -0.999f, 0.999f));
        float m1 = tan(PI / 2 * constrain(r1.tangent, -0.999f, 0.999f));

        float w0 = constrain(r0.weight, 0.f, 0.999f);
        float w1 = constrain(r1.weight, 0.f, 0.999f);
        float w1s = 1 - w1;

        float ts;
        float t = 0.5f;
        float tx = (x - x0) / dx;

        if (w0 == 1 / 3.0f && w1 == 1 / 3.0f) // TODO: float compare
        {
            t = tx;
            ts = 1 - t;
        }
        else
        {
            while (true)
            {
                ts = 1 - t;

                float t2 = t * t;
                float ts2 = ts * ts;

                float fg = 3 * ts2 * t * w0 + 3 * ts * t2 * w1s + t2 * t - tx;
                if (abs(fg) < 1e-6f)
                    break;

                // third order householder method
                float fpg = 3 * ts2 * w0 + 6 * ts * t * (w1s - w0) + 3 * t2 * (1 - w1s);
                float fppg = 6 * ts * (w1s - 2 * w0) + 6 * t * (1 - 2 * w1s + w0);
                float fpppg = 18 * w0 - 18 * w1s + 6;

                float fg2 = fg * fg;
                float fpg2 = fpg * fpg;
                t -= (6 * fg * fpg2 - 3 * fg2 * fppg) / (6 * fpg2 * fpg - 6 * fg * fpg * fppg + fg2 * fpppg);
            }
        }
        
        float t2 = t * t;
        return y0 + 3 * ts * ts * t * w0 * m0 * dx + 3 * ts * t2 * (dy - w1 * m1 * dx) + t2 * t * dy;
    }
}

float TCodeFloatingOperations::absf(float value)
{
    if (value < 0)
        return -value;
    return value;
}

unsigned long TCodeFloatingOperations::getTCodeFromFloat(float value, int precision, unsigned char &log_out)
{
    float depsilon = pow10f(-precision);
    unsigned long out = 0;
    int log = 0;

    value = constrain(value, 0, 1);
    if (value < 1.0f){
        while ((value > 0) && (log < precision))
        {
            value *= 10;
            int integer_part = static_cast<int>(value);
            value -= integer_part;
            out = (out * 10) + integer_part;
            log++;

            if ((value < depsilon) || (value > 1 - depsilon))
                break;
        }
        if (value > 0.5)
            out += 1;
    }
    else
    {
        return (unsigned long)pow10(precision);
    }

    log_out = log;
    return out;
}

unsigned long TCodeFloatingOperations::getTCodeEstimateLogFromFloat(float value)
{
    unsigned long log = 0;
    const unsigned long max_log = 15;
    while ((value > 0) && (log < max_log))
    {
        value *= 10;
        int integer_part = static_cast<int>(value);
        value -= integer_part;
        if(value < 0.5f)
            break;
    }
    return log;
}
