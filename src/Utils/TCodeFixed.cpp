// TCode-Fixed-Point-CPP v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 05/02/2022
// usage of this class can be found at (https://github.com/Dreamer2345/Arduino_TCode_Parser)
// Please copy, share, learn, innovate, give attribution.
// fixed point Implementation for fractional calculations on hardware without an FPU
// History:
//
#include "TCodeFixed.h"

void reportError(OverflowError e)
{
    
    switch (e)
    {
    case OverflowError::DIV_BY_ZERO:
        ESP_LOGE(FIXED_POINT_TAG, "Error message: %s", "Divide by 0 error occured");
        break;
    case OverflowError::POSITIVE_OVERFLOW:
        ESP_LOGE(FIXED_POINT_TAG, "Error message: %s", "Positive overflow occured");
        break;
    case OverflowError::NEGATIVE_OVERFLOW:
        ESP_LOGE(FIXED_POINT_TAG, "Error message: %s", "Negative overflow occured");
        break;
    case OverflowError::MIN_INT_DIVIDED_BY_NEG_ONE_OVERFLOW:
        ESP_LOGE(FIXED_POINT_TAG, "Error message: %s", "Min-Int Divided by 1 overflow occured");
        break;
    default : break;
    }
}

/**
 * @brief Adds two fixed point Values Together a + b
 * @param a
 * @param b
 * @param error refrence value if there is an overflow error with the function
 * @return The addition sum of those to values
 */
fixed_point_t saturateAddFixedPoint(fixed_point_t a, fixed_point_t b, OverflowError &error)
{
    fixed_point_t result = a + b;
    if ((a > 0 && b > 0 && result < a) || (a < 0 && b < 0 && result > a))
    {
        // Overflow detected, saturate the result to the maximum representable value
        error = OverflowError::POSITIVE_OVERFLOW;
        result = std::numeric_limits<fixed_point_t>::max();
    }
    else if ((a < 0 && b < 0 && result >= 0) || (a > 0 && b > 0 && result < 0))
    {
        error = OverflowError::NEGATIVE_OVERFLOW;
        // Underflow detected, saturate the result to the minimum representable value
        result = std::numeric_limits<fixed_point_t>::min();
    }
    error = OverflowError::NO_OVERFLOW;
    return result;
}

/**
 * @brief Subtracts one fixed point Value from another a - b
 * @param a
 * @param b
 * @param error refrence value if there is an overflow error with the function
 * @return The subtraction of b from a
 */
fixed_point_t saturateSubtractFixedPoint(fixed_point_t a, fixed_point_t b, OverflowError &error)
{
    fixed_point_t result = a - b;

    if ((a >= 0 && b < 0 && result < a) || (a < 0 && b >= 0 && result > a))
    {
        // Overflow detected, saturate the result to the maximum representable value
        error = OverflowError::POSITIVE_OVERFLOW;
        return std::numeric_limits<fixed_point_t>::max();
    }
    else if ((a >= 0 && b > 0 && result > a) || (a < 0 && b <= 0 && result < a))
    {
        // Overflow detected, saturate the result to the minimum representable value
        error = OverflowError::NEGATIVE_OVERFLOW;
        return std::numeric_limits<fixed_point_t>::min();
    }

    error = OverflowError::NO_OVERFLOW;
    return result;
}

/**
 * @brief Multiplies two fixed point Values together
 * @param a
 * @param b
 * @param error refrence value if there is an overflow error with the function
 * @return The multiplicative sum of a and b
 */
fixed_point_t saturateMultiplyFixedPoint(fixed_point_t a, fixed_point_t b, OverflowError &error)
{
    fixed_point_t result = (a * b) >> FIXED_POINT_SHIFT;

    // Check for overflow
    if (a != 0 && b != 0)
    {
        if ((a > 0 && b > 0 && result < 0) || (a < 0 && b < 0 && result < 0))
        {
            // Overflow detected, saturate the result to the maximum representable value
            error = OverflowError::POSITIVE_OVERFLOW;
            return (a > 0) ? std::numeric_limits<fixed_point_t>::max() : std::numeric_limits<fixed_point_t>::min();
        }
        else if ((a > 0 && b < 0 && result > 0) || (a < 0 && b > 0 && result > 0))
        {
            // Overflow detected, saturate the result to the minimum representable value
            error = OverflowError::NEGATIVE_OVERFLOW;
            return (a > 0) ? std::numeric_limits<fixed_point_t>::max() : std::numeric_limits<fixed_point_t>::min();
        }
    }

    error = OverflowError::NO_OVERFLOW;
    return result;
}

/**
 * @brief Divides two fixed point Values a / b
 * @param a
 * @param b
 * @param error refrence value if there is an overflow error with the function
 * @return The division output of a / b
 */
fixed_point_t saturateDivideFixedPoint(fixed_point_t a, fixed_point_t b, OverflowError &error)
{
    // Check for division by zero
    if (b == 0)
    {
        // Handle division by zero, return the maximum representable value
        error = OverflowError::DIV_BY_ZERO;
        return (a > 0) ? std::numeric_limits<fixed_point_t>::max() : std::numeric_limits<fixed_point_t>::min();
    }

    // Calculate the result of division
    fixed_point_t result = (a << FIXED_POINT_SHIFT) / b;

    // Check for overflow (MIN_INT / -1)
    if (a == std::numeric_limits<fixed_point_t>::min() && b == -1)
    {
        // Overflow detected, saturate the result
        error = OverflowError::MIN_INT_DIVIDED_BY_NEG_ONE_OVERFLOW;
        return std::numeric_limits<fixed_point_t>::max();
    }

    error = OverflowError::NO_OVERFLOW;
    return result;
}

/**
 * @brief Adds two fixed point Values Together a + b
 * @param a
 * @param b
 * @return The addition sum of those to values
 */
fixed_point_t addFixedPoint(fixed_point_t a, fixed_point_t b)
{
    OverflowError error;
    fixed_point_t result = saturateAddFixedPoint(a, b, error);
    reportError(error);
    return result;
}

/**
 * @brief Subtracts one fixed point Value from another a - b
 * @param a
 * @param b
 * @return The subtraction of b from a
 */
fixed_point_t subtractFixedPoint(fixed_point_t a, fixed_point_t b)
{
    OverflowError error;
    fixed_point_t result = saturateSubtractFixedPoint(a, b, error);
    reportError(error);
    return result;
}

/**
 * @brief Multiplies two fixed point Values together
 * @param a
 * @param b
 * @return The multiplicative sum of a and b
 */
fixed_point_t multiplyFixedPoint(fixed_point_t a, fixed_point_t b)
{
    OverflowError error;
    fixed_point_t result = saturateMultiplyFixedPoint(a, b, error);
    reportError(error);
    return result;
}

/**
 * @brief Divides two fixed point Values a / b
 * @param a
 * @param b
 * @return The division output of a / b
 */
fixed_point_t divideFixedPoint(fixed_point_t a, fixed_point_t b)
{
    OverflowError error;
    fixed_point_t result = saturateDivideFixedPoint(a, b, error);
    reportError(error);
    return result;
}

/**
 * @brief constrains a fixed point to between two fixed point values min, max
 * @param fixedPointValue value to be contrained
 * @param min minimum threshold
 * @param max maximum threshold
 * @return the constrained value (if min is larger than max the values are swapped)
 */
fixed_point_t constrainFixedPoint(fixed_point_t fixedPointValue, fixed_point_t min, fixed_point_t max)
{
    if (min > max)
    {
        fixed_point_t temp = min;
        min = max;
        max = temp;
    }

    if (fixedPointValue < min)
        return min;
    if (fixedPointValue > max)
        return max;
    return fixedPointValue;
}

/**
 * @brief functions the same as constrain however min can be larger than max
 * @param fixedPointValue value to be clipped
 * @param min minimum threshold
 * @param max maximum threshold
 * @return the clipped value
 */
fixed_point_t clipFixedPoint(fixed_point_t fixedPointValue, fixed_point_t min, fixed_point_t max)
{
    if (fixedPointValue < min)
        return min;
    if (fixedPointValue > max)
        return max;
    return fixedPointValue;
}

/**
 * @brief Converts an integer to a fixed point
 * @param intValue Value to be converted
 * @return The fixed point representation of the integer passed
 */
fixed_point_t intToFixedPoint(int intValue)
{
    return static_cast<fixed_point_t>(intValue) << FIXED_POINT_SHIFT;
}

/**
 * @brief Converts an integer to a fixed point
 * @param longValue Value to be converted
 * @return The fixed point representation of the integer passed
 */
fixed_point_t longToFixedPoint(long longValue)
{
    return static_cast<fixed_point_t>(longValue) << FIXED_POINT_SHIFT;
}

/**
 * @brief Converts an integer to a fixed point
 * @param longlongValue Value to be converted
 * @return The fixed point representation of the integer passed
 */
fixed_point_t longlongToFixedPoint(long long longlongValue)
{
    return static_cast<fixed_point_t>(longlongValue) << FIXED_POINT_SHIFT;
}

/**
 * @brief Converts an float to a fixed point
 * @param floatValue Value to be converted
 * @return The fixed point representation of the float passed
 */
fixed_point_t floatToFixedPoint(float floatValue)
{
    return static_cast<fixed_point_t>(floatValue * FIXED_POINT_ONE);
}

/**
 * @brief Converts an double to a fixed point
 * @param doubleValue Value to be converted
 * @return The fixed point representation of the double passed
 */
fixed_point_t doubleToFixedPoint(double doubleValue)
{
    return static_cast<fixed_point_t>(doubleValue * FIXED_POINT_ONE);
}

/**
 * @brief Converts an fixed point to an int
 * @param fixedPointValue Value to be converted
 * @return The Integer Value of the fixed point rounded
 */
int fixedPointToInt(fixed_point_t fixedPointValue)
{
    return static_cast<int>(fixedPointValue >> FIXED_POINT_SHIFT);
}

/**
 * @brief Converts an fixed point to a long
 * @param fixedPointValue Value to be converted
 * @return The Integer Value of the fixed point rounded
 */
long fixedPointToLong(fixed_point_t fixedPointValue)
{
    return static_cast<long>(fixedPointValue >> FIXED_POINT_SHIFT);
}

/**
 * @brief Converts an fixed point to a long long
 * @param fixedPointValue Value to be converted
 * @return The Integer Value of the fixed point rounded
 */
long long fixedPointToLongLong(fixed_point_t fixedPointValue)
{
    return static_cast<long long>(fixedPointValue >> FIXED_POINT_SHIFT);
}

/**
 * @brief Converts an fixed point to a Float
 * @param fixedPointValue Value to be converted
 * @return The Float Value of the fixed point rounded
 */
float fixedPointToFloat(fixed_point_t fixedPointValue)
{
    return static_cast<float>(fixedPointValue) / FIXED_POINT_ONE;
}

/**
 * @brief Converts an fixed point to an Double
 * @param fixedPointValue Value to be converted
 * @return The Double Value of the fixed point rounded
 */
double fixedPointToDouble(fixed_point_t fixedPointValue)
{
    return static_cast<double>(fixedPointValue) / FIXED_POINT_ONE;
}

/**
 * @brief Linearly interpolates between two fixed point values based on a third fixed point value which is from 0.0 to 1.0
 * @param start the starting value
 * @param stop the targer value
 * @param t the position between the start and stop values
 * @return The mixed values based on the t variable represented as a fixed point
 */
fixed_point_t lerpFixedPoint(fixed_point_t start, fixed_point_t stop, fixed_point_t t)
{
    if (t >= FIXED_POINT_ONE)
    {
        return stop;
    }

    if (t <= 0)
    {
        return start;
    }

    fixed_point_t et = subtractFixedPoint(FIXED_POINT_ONE, t);
    fixed_point_t a = multiplyFixedPoint(start, t);
    fixed_point_t b = multiplyFixedPoint(stop, et);
    return addFixedPoint(a, b);
}

/**
 * @brief Gives an eased value based on an input t value which is from 0.0 to 1.0
 * @param t the position value to be converted to an exponential curve
 * @return the interpolated value of t from 0.0 to 1.0
 */
fixed_point_t fixedPointEaseIn(fixed_point_t t)
{
    if (t > FIXED_POINT_ONE)
    {
        t = FIXED_POINT_ONE;
    }

    if (t < 0)
    {
        t = 0;
    }

    return multiplyFixedPoint(t, t);
}

/**
 * @brief Gives an inverse eased value based on an input t value which is from 0.0 to 1.0
 * @param t the position value to be converted to an exponential curve
 * @return the inverse interpolated value of t from 0.0 to 1.0
 */
fixed_point_t fixedPointEaseOut(fixed_point_t t)
{
    if (t > FIXED_POINT_ONE)
    {
        t = FIXED_POINT_ONE;
    }

    if (t < 0)
    {
        t = 0;
    }

    fixed_point_t invertedt = subtractFixedPoint(FIXED_POINT_ONE, t);
    return multiplyFixedPoint(invertedt, invertedt);
}

/**
 * @brief Maps a given range of values to an output range based on a linear method
 * @param in value to be mapped
 * @param inStart the starting min range
 * @param inEnd the starting max range
 * @param outStart the ending min range
 * @param outEnd the ending max range
 * @return the mapped value from the in range to out range based on the Ease in Curve
 */
long fixedPointMap(long in, long inStart, long inEnd, long outStart, long outEnd)
{
    fixed_point_t t = longToFixedPoint(in - inStart);                                 // Find the distance of the in value from inStart
    t = divideFixedPoint(t, longToFixedPoint(inEnd - inStart));                       // Find where the inValue lies within the range between inStart and inEnd should be (0.0 - 1.0) for values where in is in the range specified
    t = constrainFixedPoint(t, 0, FIXED_POINT_ONE);                                   // constrain the value from 0.0 to 1.0
    t = multiplyFixedPoint(t, longToFixedPoint(outEnd - outStart));                   // multiply the value of how far along the range between outStart and outEnd
    t = addFixedPoint(t, longToFixedPoint(outStart));                                 // Get the value to lie between outStart and outEnd
    t = constrainFixedPoint(t, longToFixedPoint(outStart), longToFixedPoint(outEnd)); // Constrain the value so it does not go out of bounds
    return fixedPointToLong(t);                                                       // Convert the value back to an int
}

/**
 * @brief Maps a given range of values to an output range based on the ease in curve
 * @param in value to be mapped
 * @param inStart the starting min range
 * @param inEnd the starting max range
 * @param outStart the ending min range
 * @param outEnd the ending max range
 * @return the mapped value from the in range to out range based on the Ease in Curve
 */
long fixedPointMapEaseIn(long in, long inStart, long inEnd, long outStart, long outEnd)
{
    fixed_point_t t = longToFixedPoint(in - inStart);                                 // Find the distance of the in value from inStart
    t = divideFixedPoint(t, longToFixedPoint(inEnd - inStart));                       // Find where the inValue lies within the range between inStart and inEnd should be (0.0 - 1.0) for values where in is in the range specified
    t = fixedPointEaseIn(t);                                                          // get the ease in curve value
    t = constrainFixedPoint(t, 0, FIXED_POINT_ONE);                                   // constrain the value from 0.0 to 1.0
    t = multiplyFixedPoint(t, longToFixedPoint(outEnd - outStart));                   // multiply the value of how far along the range between outStart and outEnd
    t = addFixedPoint(t, longToFixedPoint(outStart));                                 // Get the value to lie between outStart and outEnd
    t = constrainFixedPoint(t, longToFixedPoint(outStart), longToFixedPoint(outEnd)); // Constrain the value so it does not go out of bounds
    return fixedPointToLong(t);                                                       // Convert the value back to an int
}

/**
 * @brief Maps a given range of values to an output range based on the ease out curve
 * @param in value to be mapped
 * @param inStart the starting min range
 * @param inEnd the starting max range
 * @param outStart the ending min range
 * @param outEnd the ending max range
 * @return the mapped value from the in range to out range based on the Ease out Curve
 */
long fixedPointMapEaseOut(long in, long inStart, long inEnd, long outStart, long outEnd)
{
    fixed_point_t t = longToFixedPoint(in - inStart);                                 // Find the distance of the in value from inStart
    t = divideFixedPoint(t, longToFixedPoint(inEnd - inStart));                       // Find where the inValue lies within the range between inStart and inEnd should be (0.0 - 1.0) for values where in is in the range specified
    t = fixedPointEaseOut(t);                                                         // get the ease out curve value
    t = constrainFixedPoint(t, 0, FIXED_POINT_ONE);                                   // constrain the value from 0.0 to 1.0
    t = multiplyFixedPoint(t, longToFixedPoint(outEnd - outStart));                   // multiply the value of how far along the range between outStart and outEnd
    t = addFixedPoint(t, longToFixedPoint(outStart));                                 // Get the value to lie between outStart and outEnd
    t = constrainFixedPoint(t, longToFixedPoint(outStart), longToFixedPoint(outEnd)); // Constrain the value so it does not go out of bounds
    return fixedPointToLong(t);
}

/**
 * @brief Maps a given range of values to an output range based on the ease in out curve
 * @param in value to be mapped
 * @param inStart the starting min range
 * @param inEnd the starting max range
 * @param outStart the ending min range
 * @param outEnd the ending max range
 * @return the mapped value from the in range to out range based on the Ease in out Curve
 */
long fixedPointMapEaseInOut(long in, long inStart, long inEnd, long outStart, long outEnd)
{
    fixed_point_t t = longToFixedPoint(in - inStart);                                 // Find the distance of the in value from inStart
    t = divideFixedPoint(t, longToFixedPoint(inEnd - inStart));                       // Find where the inValue lies within the range between inStart and inEnd should be (0.0 - 1.0) for values where in is in the range specified
    t = constrainFixedPoint(t, 0, FIXED_POINT_ONE);                                   // Constrain the curve value between 0.0 and 1.0
    t = lerpFixedPoint(fixedPointEaseIn(t), fixedPointEaseOut(t), t);                 // Find the lerped value of the curve based on where the in value lies
    t = constrainFixedPoint(t, 0, FIXED_POINT_ONE);                                   // Constrain the curve value between 0.0 and 1.0
    t = multiplyFixedPoint(t, longToFixedPoint(outEnd - outStart));                   // multiply the value of how far along the range between outStart and outEnd
    t = addFixedPoint(t, longToFixedPoint(outStart));                                 // Get the value to lie between outStart and outEnd
    t = constrainFixedPoint(t, longToFixedPoint(outStart), longToFixedPoint(outEnd)); // Constrain the value so it does not go out of bounds
    return fixedPointToLong(t);                                                       // Convert the value back to an int
}
