// TCode-Observer-Interface-H v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 26/09/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

namespace TCode::Observer {

    template <typename T>
    class TCodeIObserver {
        public:
            virtual void notify(T message) = 0;
    };

};