// TCode-External-Interface-Class v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 24/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once
#include "../Utils/TCodeDataContainerSet.h"

class IExternalInterface
{
public:
    virtual void update(TCodeDataContainerSet& returnValues) = 0;
};