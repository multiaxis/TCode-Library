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

float TCodeFloatingOperations::absf(float value)
{
    if (value < 0)
        return -value;
    return value;
}

float TCodeFloatingOperations::pow10(int exponent)
{
    float outvalue = 10;

    if (exponent == 0)
        return 1.0f;

    for (int i = 0; i < TCodeFloatingOperations::absf(exponent); i++)
    {
        outvalue *= 10;
    }

    if (exponent < 0)
        outvalue = 1 / outvalue;
    return outvalue;
}

unsigned long TCodeFloatingOperations::getTCodeFromFloat(float value, int precision, unsigned char &log_out)
{
    float depsilon = TCodeFloatingOperations::pow10(-precision);
    unsigned long out = 0;
    int log = 0;
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

    while ((out % 10 == 0) && (log != 0))
    {
        out /= 10;
        log--;
    }

    log_out = log;
    return out;
}