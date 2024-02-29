// TCode-Data-Container-Set-Class v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 24/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once
#include <vector>
#include "TCodeTaggedDataContainer.h"

class TCodeDataContainerSet
{
private:
    std::vector<TCodeTaggedDataContainer> data;
public:
    TCodeDataContainerSet()
    {
        data.reserve(5);
    }

    bool append(TCodeTaggedDataContainer value)
    {
        for (size_t i = 0; i < data.size(); i++)
        {
            if (data[i] == value)
            {
                return false;
            }
        }

        data.push_back(value);
    }

    size_t size()
    {
        return data.size();
    }

    TCodeTaggedDataContainer &operator[](size_t i)
    {
        return data[i];
    }

    const TCodeTaggedDataContainer &operator[](size_t i) const
    {
        return data[i];
    }

};