// TCode-Interface-Manager-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 07/07/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include "TModuleBase.h"
#include <Arduino.h>
#include <vector>
// #include "../../context/TCodeContext.h"



namespace TCode::Module {
    

    class TCodeModuleManager {
    private:
        std::vector<TCodeModuleBase *> registeredInterfaces;

    public:
        void registerInterface(TCodeModuleBase *interface);
        void update(TCodeContext &context);

        TCodeModuleManager();
        ~TCodeModuleManager();
    };
}