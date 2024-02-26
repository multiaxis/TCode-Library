// TCode-External-Interface-Class v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 24/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once
#include "../Utils/TCodeVariantSet.h"

class IExternalInterface
{
public:
    virtual void update(TCodeVariantSet& returnValues) = delete;
};