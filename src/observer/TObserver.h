// TCode-Observer-Interface-H v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/09/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

namespace TCode::Observer {

    /**
     * @brief Interface for an observer in the observer pattern.
     *
     * The `TCodeIObserver` class is a generic interface for implementing the observer pattern.
     * It defines a pure virtual `notify` method that must be implemented by any concrete observer
     * class. The observer will receive updates (notifications) when a subject changes state or
     * when an event occurs.
     *
     * @tparam T The type of message or event that the observer will be notified with.
     */
    template <typename T>
    class TCodeIObserver {
    public:
        /**
         * @brief Notifies the observer with a message or event.
         *
         * This pure virtual method must be implemented by derived classes. It is called
         * to notify the observer of an event or state change, passing the relevant message
         * or data to the observer.
         *
         * @param message The message or event of type `T` that the observer will be notified with.
         */
        virtual void notify(T message) = 0;
    };

};