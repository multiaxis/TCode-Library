// TCode-Observer-Manager-H v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/09/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include "TObserver.h"
#include <vector>

namespace TCode::Observer {
    /**
     * @brief Manages a collection of observers in the observer pattern.
     *
     * The `TCodeObserverManager` class is a template class responsible for managing and notifying
     * observers that implement the `TCodeIObserver` interface. It allows observers to be registered
     * and broadcasts messages to all registered observers when a state change or event occurs.
     *
     * @tparam T The type of message or event that the observers will be notified with.
     */
    template <typename T>
    class TCodeObserverManager {
    private:
        std::vector<TCodeIObserver<T> *> registeredObservers; ///< List of registered observers.

    public:
        /**
         * @brief Notifies all registered observers with a message.
         *
         * This method sends a notification to all observers currently registered with the manager.
         * Each observer's `notify` method is called with the provided message or event.
         *
         * @param message The message or event of type `T` to be broadcasted to all observers.
         */
        void notify(T message);

        /**
         * @brief Registers an observer to receive notifications.
         *
         * Adds an observer to the list of registered observers, if it is not already registered.
         * Once registered, the observer will receive notifications through the `notify` method.
         *
         * @param observer A pointer to the `TCodeIObserver` interface to be registered.
         * @return true if the observer was successfully registered, false if the observer was null or already registered.
         */
        bool registerObserver(TCodeIObserver<T> *observer);
    };

    /**
     * @brief Notifies all registered observers with a message.
     *
     * This method iterates through the list of registered observers and calls their `notify`
     * method, passing the provided message to each observer.
     *
     * @tparam T The type of message or event that the observers will be notified with.
     * @param message The message or event to be broadcasted to all observers.
     */
    template <typename T>
    inline void TCodeObserverManager<T>::notify(T message) {
        for (size_t i = 0; i < registeredObservers.size(); i++) {
            registeredObservers[i]->notify(message);
        }
    }

    /**
     * @brief Registers an observer to receive notifications.
     *
     * This method checks if the observer is null or already registered. If not, it adds the observer
     * to the list of registered observers, allowing it to receive future notifications.
     *
     * @tparam T The type of message or event that the observers will be notified with.
     * @param observer A pointer to the `TCodeIObserver` to be registered.
     * @return true if the observer was successfully registered, false otherwise.
     */
    template <typename T>
    inline bool TCodeObserverManager<T>::registerObserver(TCodeIObserver<T> *observer) {
        if (observer == nullptr)
            return false;

        // Check if the observer is already registered.
        auto position = std::find_if(registeredObservers.begin(), registeredObservers.end(), [observer](TCodeIObserver<T> *in) {
            return in == observer;
        });

        if (position != registeredObservers.end())
            return false;

        registeredObservers.push_back(observer);
        return true;
    }
};