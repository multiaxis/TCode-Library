// TCode-Floating-Point-functions-h v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 29/11/2023
// usage of this class can be found at (https://github.com/Dreamer2345/Arduino_TCode_Parser)
// Please copy, share, learn, innovate, give attribution.
//
#include "TCodeFloating.h"

double lerpDouble(double start, double stop, double t)
{
    return (start * (1 - t)) + (stop * t);
}

double doubleEaseIn(double t)
{
    if(t < 0)
        return 0.0f;
    if(t > 1)
        return 1.0f;
    return t * t;
}

double doubleEaseOut(double t)
{
    if(t < 0)
        return 1.0f;
    if(t > 1)
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

long doubleMap(long in, long inStart, long inEnd, long outStart, long outEnd)
{
    double t = in - inStart;
    t = t / (inEnd - inStart);
    t = constrainDouble(t,0.0,1.0);
    t = t * (outEnd - outStart);
    t += outStart;
    t = constrainDouble(t,outStart,outEnd);
    return t;
}

long doubleMapEaseIn(long in, long inStart, long inEnd, long outStart, long outEnd)
{
    double t = in - inStart;
    t = t / (inEnd - inStart);
    t = doubleEaseIn(t);
    t = constrainDouble(t,0.0,1.0);
    t = t * (outEnd - outStart);
    t += outStart;
    t = constrainDouble(t,outStart,outEnd);
    return t;
}

long doubleMapEaseOut(long in, long inStart, long inEnd, long outStart, long outEnd)
{
    double t = in - inStart;
    t = t / (inEnd - inStart);
    t = doubleEaseOut(t);
    t = constrainDouble(t,0.0,1.0);
    t = t * (outEnd - outStart);
    t += outStart;
    t = constrainDouble(t,outStart,outEnd);
    return t;
}

long doubleMapEaseInOut(long in, long inStart, long inEnd, long outStart, long outEnd)
{
    double t = in - inStart;
    t = t / (inEnd - inStart);
    t = lerpDouble(doubleEaseIn(t),doubleEaseOut(t),t);
    t = constrainDouble(t,0.0,1.0);
    t = t * (outEnd - outStart);
    t += outStart;
    t = constrainDouble(t,outStart,outEnd);
    return t;
}