// TCode-Fixed-Point-H v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 05/02/2022
// usage of this class can be found at (https://github.com/Dreamer2345/Arduino_TCode_Parser)
// Please copy, share, learn, innovate, give attribution.
// fixed point Implementation for fractional calculations on hardware without an FPU
// History:
//
#pragma once
#ifndef TCODE_FIXED_POINT_H
#define TCODE_FIXED_POINT_H
#include "esp_log.h"
#include <cstdint>
#include <limits>

#define FIXED_POINT_TAG "TCodeFixedPoint"
#define FIXED_POINT_SHIFT 16
#define FIXED_POINT_ONE (1LL << FIXED_POINT_SHIFT)
typedef int64_t fixed_point_t;

enum class OverflowError
{
    NO_OVERFLOW,
    POSITIVE_OVERFLOW,
    NEGATIVE_OVERFLOW,
    DIV_BY_ZERO,
    MIN_INT_DIVIDED_BY_NEG_ONE_OVERFLOW
};

void reportError(OverflowError e);

/**
 * @brief Adds two fixed point Values Together a + b
 * @param a
 * @param b
 * @param error refrence value if there is an overflow error with the function
 * @return The addition sum of those to values
 */
fixed_point_t saturateAddFixedPoint(fixed_point_t a, fixed_point_t b, OverflowError &error);

/**
 * @brief Subtracts one fixed point Value from another a - b
 * @param a
 * @param b
 * @param error refrence value if there is an overflow error with the function
 * @return The subtraction of b from a
 */
fixed_point_t saturateSubtractFixedPoint(fixed_point_t a, fixed_point_t b, OverflowError &error);

/**
 * @brief Multiplies two fixed point Values together
 * @param a
 * @param b
 * @param error refrence value if there is an overflow error with the function
 * @return The multiplicative sum of a and b
 */
fixed_point_t saturateMultiplyFixedPoint(fixed_point_t a, fixed_point_t b, OverflowError &error);

/**
 * @brief Divides two fixed point Values a / b
 * @param a
 * @param b
 * @param error refrence value if there is an overflow error with the function
 * @return The division output of a / b
 */
fixed_point_t saturateDivideFixedPoint(fixed_point_t a, fixed_point_t b, OverflowError &error);

/**
 * @brief Adds two fixed point Values Together a + b
 * @param a
 * @param b
 * @return The addition sum of those to values
 */
fixed_point_t addFixedPoint(fixed_point_t a, fixed_point_t b);

/**
 * @brief Subtracts one fixed point Value from another a - b
 * @param a
 * @param b
 * @return The subtraction of b from a
 */
fixed_point_t subtractFixedPoint(fixed_point_t a, fixed_point_t b);

/**
 * @brief Multiplies two fixed point Values together
 * @param a
 * @param b
 * @return The multiplicative sum of a and b
 */
fixed_point_t multiplyFixedPoint(fixed_point_t a, fixed_point_t b);

/**
 * @brief Divides two fixed point Values a / b
 * @param a
 * @param b
 * @return The division output of a / b
 */
fixed_point_t divideFixedPoint(fixed_point_t a, fixed_point_t b);

/**
 * @brief constrains a fixed point to between two fixed point values min, max
 * @param fixedPointValue value to be contrained
 * @param min minimum threshold
 * @param max maximum threshold
 * @return the constrained value (if min is larger than max the values are swapped)
 */
fixed_point_t constrainFixedPoint(fixed_point_t fixedPointValue, fixed_point_t min, fixed_point_t max);

/**
 * @brief functions the same as constrain however min can be larger than max
 * @param fixedPointValue value to be clipped
 * @param min minimum threshold
 * @param max maximum threshold
 * @return the clipped value
 */
fixed_point_t clipFixedPoint(fixed_point_t fixedPointValue, fixed_point_t min, fixed_point_t max);

/**
 * @brief Converts an integer to a fixed point
 * @param intValue Value to be converted
 * @return The fixed point representation of the integer passed
 */
fixed_point_t intToFixedPoint(int intValue);
/**
 * @brief Converts an integer to a fixed point
 * @param longValue Value to be converted
 * @return The fixed point representation of the integer passed
 */
fixed_point_t longToFixedPoint(long longValue);

/**
 * @brief Converts an integer to a fixed point
 * @param longlongValue Value to be converted
 * @return The fixed point representation of the integer passed
 */
fixed_point_t longlongToFixedPoint(long long longlongValue);

/**
 * @brief Converts an float to a fixed point
 * @param floatValue Value to be converted
 * @return The fixed point representation of the float passed
 */
fixed_point_t floatToFixedPoint(float floatValue);

/**
 * @brief Converts an double to a fixed point
 * @param doubleValue Value to be converted
 * @return The fixed point representation of the double passed
 */
fixed_point_t doubleToFixedPoint(double doubleValue);

/**
 * @brief Converts an fixed point to an int
 * @param fixedPointValue Value to be converted
 * @return The Integer Value of the fixed point rounded
 */
int fixedPointToInt(fixed_point_t fixedPointValue);

/**
 * @brief Converts an fixed point to a long
 * @param fixedPointValue Value to be converted
 * @return The Integer Value of the fixed point rounded
 */
long fixedPointToLong(fixed_point_t fixedPointValue);

/**
 * @brief Converts an fixed point to a long long
 * @param fixedPointValue Value to be converted
 * @return The Integer Value of the fixed point rounded
 */
long long fixedPointToLongLong(fixed_point_t fixedPointValue);

/**
 * @brief Converts an fixed point to a Float
 * @param fixedPointValue Value to be converted
 * @return The Float Value of the fixed point rounded
 */
float fixedPointToFloat(fixed_point_t fixedPointValue);

/**
 * @brief Converts an fixed point to an Double
 * @param fixedPointValue Value to be converted
 * @return The Double Value of the fixed point rounded
 */
double fixedPointToDouble(fixed_point_t fixedPointValue);

/**
 * @brief Linearly interpolates between two fixed point values based on a third fixed point value which is from 0.0 to 1.0
 * @param start the starting value
 * @param stop the targer value
 * @param t the position between the start and stop values
 * @return The mixed values based on the t variable represented as a fixed point
 */
fixed_point_t lerpFixedPoint(fixed_point_t start, fixed_point_t stop, fixed_point_t t);

/**
 * @brief Gives an eased value based on an input t value which is from 0.0 to 1.0
 * @param t the position value to be converted to an exponential curve
 * @return the interpolated value of t from 0.0 to 1.0
 */
fixed_point_t fixedPointEaseIn(fixed_point_t t);

/**
 * @brief Gives an inverse eased value based on an input t value which is from 0.0 to 1.0
 * @param t the position value to be converted to an exponential curve
 * @return the inverse interpolated value of t from 0.0 to 1.0
 */
fixed_point_t fixedPointEaseOut(fixed_point_t t);

/**
 * @brief Maps a given range of values to an output range based on a linear method
 * @param in value to be mapped
 * @param inStart the starting min range
 * @param inEnd the starting max range
 * @param outStart the ending min range
 * @param outEnd the ending max range
 * @return the mapped value from the in range to out range based on the Ease in Curve
 */
long fixedPointMap(long in, long inStart, long inEnd, long outStart, long outEnd);

/**
 * @brief Maps a given range of values to an output range based on the ease in curve
 * @param in value to be mapped
 * @param inStart the starting min range
 * @param inEnd the starting max range
 * @param outStart the ending min range
 * @param outEnd the ending max range
 * @return the mapped value from the in range to out range based on the Ease in Curve
 */
long fixedPointMapEaseIn(long in, long inStart, long inEnd, long outStart, long outEnd);

/**
 * @brief Maps a given range of values to an output range based on the ease out curve
 * @param in value to be mapped
 * @param inStart the starting min range
 * @param inEnd the starting max range
 * @param outStart the ending min range
 * @param outEnd the ending max range
 * @return the mapped value from the in range to out range based on the Ease out Curve
 */
long fixedPointMapEaseOut(long in, long inStart, long inEnd, long outStart, long outEnd);

/**
 * @brief Maps a given range of values to an output range based on the ease in out curve
 * @param in value to be mapped
 * @param inStart the starting min range
 * @param inEnd the starting max range
 * @param outStart the ending min range
 * @param outEnd the ending max range
 * @return the mapped value from the in range to out range based on the Ease in out Curve
 */
long fixedPointMapEaseInOut(long in, long inStart, long inEnd, long outStart, long outEnd);

#endif