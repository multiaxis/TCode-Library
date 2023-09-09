// TCode-Fixed-Point-H v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 05/02/2022
// usage of this class can be found at (https://github.com/Dreamer2345/Arduino_TCode_Parser)
// Please copy, share, learn, innovate, give attribution.
// Q16 Implementation for fractional calculations on hardware without an FPU
// History:
//
#pragma once
#ifndef TCODE_FIXED_POINT_H
#define TCODE_FIXED_POINT_H


using InputType = long;
using Q16Fixed = long;
using Q16FixedL = long long;  
const Q16Fixed Q = 16;

/*
int32_t sat_Q16(int64_t x)
{
    if (x > 0x7FFFFF) return 0x7FFFFF;
    else if (x < -0x800000) return -0x800000;
    else return (int32_t)x;
}
*/

/**
 * @brief Adds two Q16 Values Together a + b
 * @param a
 * @param b
 * @return The addition sum of those to values
 */
Q16Fixed addQ16(Q16Fixed a, Q16Fixed b);

/**
 * @brief Subtracts one Q16 Value from another a - b
 * @param a
 * @param b
 * @return The subtraction of b from a
 */
Q16Fixed subQ16(Q16Fixed a, Q16Fixed b);

/**
 * @brief Multiplies two Q16 Values together
 * @param a
 * @param b
 * @return The multiplicative sum of a and b
 */
Q16Fixed multQ16(Q16Fixed a, Q16Fixed b);


/**
 * @brief Divides two Q16 Values a / b
 * @param a
 * @param b
 * @return The division output of a / b
 */
Q16Fixed divQ16(Q16Fixed a, Q16Fixed b);

/**
 * @brief constrains a Q16 to between two Q16 values min, max
 * @param v value to be contrained
 * @param min minimum threshold
 * @param max maximum threshold
 * @return the constrained value (if min is larger than max the values are swapped)
 */
Q16Fixed constrainQ16(Q16Fixed v, Q16Fixed min, Q16Fixed max);

Q16Fixed clipQ16(Q16Fixed v, Q16Fixed min, Q16Fixed max);

/**
 * @brief Converts an integer to a Q16
 * @param i Value to be converted
 * @return The Q16 representation of the integer passed
 */
Q16Fixed Q16fromInt(long i);

/**
 * @brief Converts an float to a Q16
 * @param i Value to be converted
 * @return The Q16 representation of the float passed
 */
Q16Fixed Q16fromFloat(float i);

/**
 * @brief Converts an double to a Q16
 * @param i Value to be converted
 * @return The Q16 representation of the double passed
 */
Q16Fixed Q16fromDouble(double i);

/**
 * @brief Converts an Q16 to a Integer
 * @param i Value to be converted
 * @return The Integer Value of the Q16 rounded
 */
Q16Fixed IntfromQ16(Q16Fixed i);

/**
 * @brief Converts an Q16 to a Float
 * @param i Value to be converted
 * @return The Float Value of the Q16 rounded
 */
float FloatfromQ16(Q16Fixed i);

/**
 * @brief Converts an Q16 to an Double
 * @param i Value to be converted
 * @return The Double Value of the Q16 rounded
 */
double DoublefromQ16(Q16Fixed i);


/**
 * @brief Linearly interpolates between two Q16 values based on a third Q16 value which is from 0.0 to 1.0
 * @param start the starting value
 * @param stop the targer value
 * @param t the position between the start and stop values
 * @return The mixed values based on the t variable represented as a Q16
 */
Q16Fixed lerpQ16(Q16Fixed start, Q16Fixed stop, Q16Fixed t);

/**
 * @brief Gives an eased value based on an input t value which is from 0.0 to 1.0
 * @param t the position value to be converted to an exponential curve
 * @return the interpolated value of t from 0.0 to 1.0
 */
Q16Fixed easeIn(Q16Fixed t);

/**
 * @brief Gives an inverse eased value based on an input t value which is from 0.0 to 1.0
 * @param t the position value to be converted to an exponential curve
 * @return the inverse interpolated value of t from 0.0 to 1.0
 */
Q16Fixed easeOut(Q16Fixed t);

/**
 * @brief Maps a given range of values to an output range based on a linear method
 * @param in value to be mapped
 * @param inStart the starting min range
 * @param inEnd the starting max range
 * @param outStart the ending min range
 * @param outEnd the ending max range
 * @return the mapped value from the in range to out range based on the Ease in Curve
 */
long mapQ16(long in, long inStart, long inEnd, long outStart, long outEnd);

/**
 * @brief Maps a given range of values to an output range based on the ease in curve
 * @param in value to be mapped
 * @param inStart the starting min range
 * @param inEnd the starting max range
 * @param outStart the ending min range
 * @param outEnd the ending max range
 * @return the mapped value from the in range to out range based on the Ease in Curve
 */
long mapEaseIn(long in, long inStart, long inEnd, long outStart, long outEnd);

/**
 * @brief Maps a given range of values to an output range based on the ease out curve
 * @param in value to be mapped
 * @param inStart the starting min range
 * @param inEnd the starting max range
 * @param outStart the ending min range
 * @param outEnd the ending max range
 * @return the mapped value from the in range to out range based on the Ease out Curve
 */
long mapEaseOut(long in, long inStart, long inEnd, long outStart, long outEnd);

/**
 * @brief Maps a given range of values to an output range based on the ease in out curve
 * @param in value to be mapped
 * @param inStart the starting min range
 * @param inEnd the starting max range
 * @param outStart the ending min range
 * @param outEnd the ending max range
 * @return the mapped value from the in range to out range based on the Ease in out Curve
 */
long mapEaseInOut(long in, long inStart, long inEnd, long outStart, long outEnd);

#endif