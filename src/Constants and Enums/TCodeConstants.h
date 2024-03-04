// TCode-Constants-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 05/02/2022
#ifndef TCODE_CONSTANTS
#define TCODE_CONSTANTS


#define TCODE_MAX_AXIS 0.9999f


/**
 * @brief Value used to define the max log count
 */
#define TCODE_MAX_LOG 15

/**
 * @brief Value used to define the max channel count
 */
#define TCODE_MAX_CHANNEL_COUNT 10

/**
 * @brief Value used to define the max number of channel types
 */
#define TCODE_CHANNEL_TYPES 4

/**
 * @brief Value used to define the default value for a TCode Axis
 */
#define TCODE_DEFAULT_AXIS_RETURN_VALUE 5000;

/**
 * @brief Value used to define the auto-smooth interval minimum range used in live commands (ms)
 */
#define TCODE_MIN_AXIS_SMOOTH_INTERVAL 3 // Minimum auto-smooth ramp interval for live commands (ms)

/**
 * @brief Value used to define the auto-smooth interval maximum range used in live commands (ms)
 */
#define TCODE_MAX_AXIS_SMOOTH_INTERVAL 100 // Maximum auto-smooth ramp interval for live commands (ms)

/**
 * @brief Value used to define the length the buffer used in the TCode Axis class
 */
#define TCODE_MAX_AXIS_BUFFER_LENGTH 10

#endif