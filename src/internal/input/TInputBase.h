// TCode-External-Interface-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 24/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include "../../common/TCodeManager.h"

class TCodeManager;

namespace TCode {

class TInputBase
{
public:
    virtual void update(TCodeManager context) = 0;
};

}
