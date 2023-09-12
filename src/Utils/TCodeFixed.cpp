// TCode-Fixed-Point-CPP v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 05/02/2022
// usage of this class can be found at (https://github.com/Dreamer2345/Arduino_TCode_Parser)
// Please copy, share, learn, innovate, give attribution.
// Q16 Implementation for fractional calculations on hardware without an FPU
// History:
//
#ifndef TCODE_FIXED_POINT_CPP
#define TCODE_FIXED_POINT_CPP
#include "TCodeFixed.h"
#include <Arduino.h>

Q16Fixed addQ16(Q16Fixed a, Q16Fixed b)
{
    Q16FixedL tmp = static_cast<Q16FixedL>(a) + static_cast<Q16FixedL>(b);
    return tmp;
}

Q16Fixed subQ16(Q16Fixed a, Q16Fixed b)
{
    return a - b;
}

Q16Fixed multQ16(Q16Fixed a, Q16Fixed b)
{
    Q16FixedL tmp = (static_cast<Q16FixedL>(a) * static_cast<Q16FixedL>(b)) >> static_cast<Q16FixedL>(Q);
    return tmp;
}

Q16Fixed divQ16(Q16Fixed a, Q16Fixed b)
{
    return (((static_cast<Q16FixedL>(a)) << static_cast<Q16FixedL>(Q)) / static_cast<Q16FixedL>(b));
}

Q16Fixed constrainQ16(Q16Fixed v, Q16Fixed min, Q16Fixed max)
{
    if (min > max)
    {
        Q16Fixed temp = min;
        min = max;
        max = temp;
    }

    if (v < min)
        return min;
    if (v > max)
        return max;
    return v;
}

Q16Fixed clipQ16(Q16Fixed v, Q16Fixed min, Q16Fixed max)
{
    if (v < min)
        return min;
    if (v > max)
        return max;
    return v;
}

Q16Fixed Q16fromInt(long i)
{
    return (static_cast<Q16FixedL>(i)) << static_cast<Q16FixedL>(Q);
}

Q16Fixed Q16fromFloat(float i)
{
    float newValuef = i * (static_cast<Q16FixedL>(1) << static_cast<Q16FixedL>(Q));
    return static_cast<Q16Fixed>(newValuef);
}

Q16Fixed Q16fromDouble(double i)
{
    double newValued = i * (static_cast<Q16FixedL>(1) << static_cast<Q16FixedL>(Q));
    return static_cast<Q16Fixed>(newValued);
}

Q16Fixed IntfromQ16(Q16Fixed i)
{
    i = addQ16(i, Q16fromDouble(0.5));
    return static_cast<Q16Fixed>(i >> static_cast<Q16FixedL>(Q));
}

float FloatfromQ16(Q16Fixed i)
{
    return ((static_cast<float>(i)) / (static_cast<Q16FixedL>(1) << static_cast<Q16FixedL>(Q)));
}

double DoublefromQ16(Q16Fixed i)
{
    return ((static_cast<double>(i)) / (static_cast<Q16FixedL>(1) << static_cast<Q16FixedL>(Q)));
}

Q16Fixed lerpQ16(Q16Fixed start, Q16Fixed stop, Q16Fixed t)
{
    // lerp is defined as mixed = ((1.0 - t) * a) + (t * b)
    t = constrainQ16(t, 0, Q16fromInt(1));
    Q16Fixed tn = subQ16(Q16fromInt(1), t);
    Q16Fixed a = multQ16(tn, start);
    Q16Fixed b = multQ16(t, stop);
    return addQ16(a, b);
}

Q16Fixed easeIn(Q16Fixed t)
{
    t = constrainQ16(t, 0, Q16fromInt(1));
    t = multQ16(t, t);
    return t;
}

Q16Fixed easeOut(Q16Fixed t)
{
    t = constrainQ16(t, 0, Q16fromInt(1));
    t = subQ16(Q16fromInt(1), t);
    t = multQ16(t, t);
    return subQ16(Q16fromInt(1), t);
}

long mapQ16(long in, long inStart, long inEnd, long outStart, long outEnd)
{
    Q16Fixed t = Q16fromInt(in);
    t = constrainQ16(t, Q16fromInt(inStart), Q16fromInt(inEnd));
    t = t - Q16fromInt(inStart);                                      // Find the distance of the in value from inStart
    t = divQ16(t, subQ16(Q16fromInt(inEnd), Q16fromInt(inStart)));    // Find where the inValue lies within the range between inStart and inEnd should be (0.0 - 1.0) for values where in is in the range specified
    t = constrainQ16(t, 0, Q16fromInt(1));                            // constrain the value from 0.0 to 1.0
    t = multQ16(t, subQ16(Q16fromInt(outEnd), Q16fromInt(outStart))); // multiply the value of how far along the range between outStart and outEnd
    t = addQ16(t, Q16fromInt(outStart));                              // Get the value to lie between outStart and outEnd
    t = constrainQ16(t, Q16fromInt(outStart), Q16fromInt(outEnd));    // Constrain the value so it does not go out of bounds
    t = IntfromQ16(t);
    return t; // Convert the value back to an int
}

long mapEaseIn(long in, long inStart, long inEnd, long outStart, long outEnd)
{
    Q16Fixed t = Q16fromInt(in - inStart);                         // Find the distance of the in value from inStart
    t = divQ16(t, Q16fromInt(inEnd - inStart));                    // Find where the inValue lies within the range between inStart and inEnd should be (0.0 - 1.0) for values where in is in the range specified
    t = easeIn(t);                                                 // get the ease in curve value
    t = constrainQ16(t, 0, Q16fromInt(1));                         // constrain the value from 0.0 to 1.0
    t = multQ16(t, Q16fromInt(outEnd - outStart));                 // multiply the value of how far along the range between outStart and outEnd
    t = addQ16(t, Q16fromInt(outStart));                           // Get the value to lie between outStart and outEnd
    t = constrainQ16(t, Q16fromInt(outStart), Q16fromInt(outEnd)); // Constrain the value so it does not go out of bounds
    return IntfromQ16(t);                                          // Convert the value back to an int
}

long mapEaseOut(long in, long inStart, long inEnd, long outStart, long outEnd)
{
    Q16Fixed t = Q16fromInt(in - inStart);                         // Find the distance of the in value from inStart
    t = divQ16(t, Q16fromInt(inEnd - inStart));                    // Find where the inValue lies within the range between inStart and inEnd should be (0.0 - 1.0) for values where in is in the range specified
    t = easeOut(t);                                                // get the ease out curve value
    t = constrainQ16(t, 0, Q16fromInt(1));                         // constrain the value from 0.0 to 1.0
    t = multQ16(Q16fromInt(outEnd - outStart), t);                 // multiply the value of how far along the range between outStart and outEnd
    t = addQ16(t, Q16fromInt(outStart));                           // Get the value to lie between outStart and outEnd
    t = constrainQ16(t, Q16fromInt(outStart), Q16fromInt(outEnd)); // Constrain the value so it does not go out of bounds
    return IntfromQ16(t);                                          // Convert the value back to an int
}

long mapEaseInOut(long in, long inStart, long inEnd, long outStart, long outEnd)
{
    Q16Fixed t = Q16fromInt(in - inStart);                         // Find the distance of the in value from inStart
    t = divQ16(t, Q16fromInt(inEnd - inStart));                    // Find where the inValue lies within the range between inStart and inEnd should be (0.0 - 1.0) for values where in is in the range specified
    t = lerpQ16(easeIn(t), easeOut(t), t);                         // Find the lerped value of the curve based on where the in value lies
    t = constrainQ16(t, 0, Q16fromInt(1));                         // Constrain the curve value between 0.0 and 1.0
    t = multQ16(Q16fromInt(outEnd - outStart), t);                 // multiply the value of how far along the range between outStart and outEnd
    t = addQ16(t, Q16fromInt(outStart));                           // Get the value to lie between outStart and outEnd
    t = constrainQ16(t, Q16fromInt(outStart), Q16fromInt(outEnd)); // Constrain the value so it does not go out of bounds
    return IntfromQ16(t);                                          // Convert the value back to an int
}

#endif