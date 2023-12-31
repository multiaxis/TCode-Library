// TCode-Floating-Point-functions-h v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 29/11/2023
// usage of this class can be found at (https://github.com/Dreamer2345/Arduino_TCode_Parser)
// Please copy, share, learn, innovate, give attribution.
//

#pragma once
#ifndef TCODE_FLOATING_POINT_H
#define TCODE_FLOATING_POINT_H

/**
 * @brief Maps a given range of values to an output range based on a linear method
 * @param in value to be mapped
 * @param inStart the starting min range
 * @param inEnd the starting max range
 * @param outStart the ending min range
 * @param outEnd the ending max range
 * @return the mapped value from the in range to out range
 */
long doubleMap(long in, long inStart, long inEnd, long outStart, long outEnd);

/**
 * @brief Maps a given range of values to an output range based on the ease in curve
 * @param in value to be mapped
 * @param inStart the starting min range
 * @param inEnd the starting max range
 * @param outStart the ending min range
 * @param outEnd the ending max range
 * @return the mapped value from the in range to out range based on the Ease in Curve
 */
long doubleMapEaseIn(long in, long inStart, long inEnd, long outStart, long outEnd);

/**
 * @brief Maps a given range of values to an output range based on the ease out curve
 * @param in value to be mapped
 * @param inStart the starting min range
 * @param inEnd the starting max range
 * @param outStart the ending min range
 * @param outEnd the ending max range
 * @return the mapped value from the in range to out range based on the Ease out Curve
 */
long doubleMapEaseOut(long in, long inStart, long inEnd, long outStart, long outEnd);

/**
 * @brief Maps a given range of values to an output range based on the ease in out curve
 * @param in value to be mapped
 * @param inStart the starting min range
 * @param inEnd the starting max range
 * @param outStart the ending min range
 * @param outEnd the ending max range
 * @return the mapped value from the in range to out range based on the Ease in out Curve
 */
long doubleMapEaseInOut(long in, long inStart, long inEnd, long outStart, long outEnd);

#endif