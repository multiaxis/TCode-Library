// TCode-Observer-Manager-H v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/09/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include "TObserver.h"
#include <vector>

namespace TCode::Observer {
    template <typename T>
    class TCodeObserverManager {
    private:
        std::vector<TCodeIObserver<T> *> registeredObservers;

    public:
        void notify(T message);
        bool registerObserver(TCodeIObserver<T> *);
    };

    template <typename T>
    inline void TCodeObserverManager<T>::notify(T message) {
        for (size_t i = 0; i < registeredObservers.size(); i++) {
            registeredObservers[i]->notify(message);
        }
    }

    template <typename T>
    inline bool TCodeObserverManager<T>::registerObserver(TCodeIObserver<T> *observer) {
        if (observer == nullptr)
            return false;

        auto position = std::find_if(registeredObservers.begin(), registeredObservers.end(), [observer](TCodeIObserver<T> *in) {
            return in == observer;
        });

        if (position != registeredObservers.end())
            return false;

        registeredObservers.push_back(observer);
        return true;
    }

};