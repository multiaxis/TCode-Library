// TCode-Axis-Unbuffered-H v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 18/11/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include "../datatypes/CommonDataTypes.h"

namespace TCode::Axis {

    /**
     * @brief Minimum axis smoothing interval.
     *
     * Minimum time in ms that can be used in set to update minInterval for non-extension commands.
     */
    const int TCODE_MIN_AXIS_SMOOTH_INTERVAL = 3;

    /**
     * @brief Maximum axis smoothing interval.
     *
     * Maximum time in ms that can be used in set to update minInterval for non-extension commands.
     */
    const int TCODE_MAX_AXIS_SMOOTH_INTERVAL = 100;

    /**
     * @brief TCode Axis Class used for containing and updating axis data.
     */
    class TCodeAxis {
    public:
        /**
         * @brief Default constructor is deleted as name, id and default position are required paramaters.
         */
        TCodeAxis() = delete;

        /**
         * @brief Constructor
         * 
         * @param name name for the axis used by commands such as save and device to return verbose data.
         * @param id axis id. maps to sent axis id in axis commands.
         * @param defaultPosition used by stop and on startup to set the position of this axis.
         */
        TCodeAxis(const char *name, Datatypes::AxisId id, float defaultPosition);

        /**
         * @brief Sets the current state of the axis and updates its target position
         * 
         * @param data an axis data struct containing the information for updating the axis such as target position and extention commands
         */
        void set(const Datatypes::AxisData &data);

        /**
         * @brief Gets the current position of the axis
         * 
         * @returns the current position as a float 0.0 - 0.99...
         */
        const float getPosition();

        /**
         * @brief Stops the axis at its current position
         * 
         * As for Vibration channels this method sets the end position at its default position.
         */
        void stop();

        /**
         * @brief Gets if the position of the axis has changed since last called.
         * 
         * this command does make a call to getPosition and clobbers lastPosition if called twice it will not trigger.
         * 
         * @returns boolean true if axis has changed position.
         */
        const bool changed();

        /**
         * @brief Gets the name of the axis as a c-string.
         * 
         * @returns a constant char string of the name of the axis.
         */
        const char *getName();

        /**
         * @brief Gets the Id of the axis
         * 
         * @returns an Axis ID channel type and number struct
         */
        const Datatypes::AxisId getId();

        /**
         * @brief Gets the last time that a call to set was done.
         * 
         * @returns unsigned long of millis() when last call to set was completed.
         */
        const unsigned long getLastCommandTime();

    private:
        const char *name;
        Datatypes::AxisId id;
        Datatypes::AxisState currentState;
        float lastPosition;
        float defaultPosition;
        int minInterval;
        unsigned long lastCommandTime;
    };

}
