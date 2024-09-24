// TCode-External-Interface-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 24/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

// #include "../../context/TCodeContext.h"

namespace TCode {
class TCodeContext;
}

namespace TCode::Module {
    class TCodeModuleBase {
    public:
        virtual void update(TCodeContext &context) = 0;
    };
}
