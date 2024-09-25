// TCode-Axis-Manager-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 07/07/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include "TAxis.h"
#include <Arduino.h>
#include <vector>

namespace TCode::Axis {
    /**
     * @brief Container for TCodeAxis
     */
    class TCodeAxisManager {
    private:
        std::vector<TCodeAxis *> registeredAxes;

    public:
        /**
         * @brief stops all axis contained by this class.
         */
        void stop();

        /**
         * @brief gets the axis contained by this class with id and performs the set command with provided data.
         *
         * @param id Id of the axis to set position data of.
         * @param data data to set to the axis
         */
        void setAxisData(const Datatypes::AxisId &id, const Datatypes::AxisData &data);

        /**
         * @brief gets the axis contained by this class with id and returns the position as a float 0.0 - 0.99...
         *
         * @param id Id of the axis to get position of.
         *
         * @returns returns the position as a float 0.0 - 0.99.. if no axis is found NAN is returned.
         */
        const float getAxisPosition(const Datatypes::AxisId &id);

        /**
         * @brief gets the axis contained by this class with id and returns the last position.
         *
         * @param id Id of the axis to get the last command time of.
         *
         * @returns returns the last time set was performed by the axis with ID if no axis was found LONG_MAX is returned.
         */
        const unsigned long getAxisLastCommandTime(const Datatypes::AxisId &id);

        /**
         * @brief registers this axis with the class.
         *
         * Registers a TCode axis with the container, if axis is nullptr or if id in axis is already contained this function will return false.
         *
         * @param axis pointer to an axis object.
         *
         * @returns true if axis was registered successfully.
         */
        const bool registerAxis(TCodeAxis *axis);

        /**
         * @brief gets the axis from container and returns it in provided reference
         *
         * @param id Id of the axis.
         *
         * @param axis reference pointer of type TCodeAxis which is set if axis with id was found.
         *
         * @returns true if axis was found.
         */
        const bool getAxisFromId(const Datatypes::AxisId &id, TCodeAxis *&axis);

        /**
         * @brief finds if container has axis with id registered.
         *
         * @param id Id of the axis to find.
         *
         * @returns true if axis was found.
         */
        const bool hasAxisFromId(const Datatypes::AxisId &id);

        /**
         * @brief gets the axis from container at index.
         *
         * @param index index of the axis to get.
         *
         * @returns TCodeAxis pointer or nullptr if index out of bounds.
         */
        TCodeAxis *getAxisIndex(const size_t &index);

        /**
         * @brief gets the axis from container at index.
         *
         * @param index index of the axis to get.
         *
         * @returns TCodeAxis pointer or nullptr if index out of bounds.
         */
        TCodeAxis *operator[](const size_t &index);

        /**
         * @brief returns the number of axis registered with this container.
         *
         * @returns number of axis registered.
         */
        const size_t count();

        TCodeAxisManager();
        ~TCodeAxisManager();
    };
};