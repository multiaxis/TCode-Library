// TCode-Encoder-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 27/06/2022
// Please copy, share, learn, innovate, give attribution.
#pragma once
#ifndef TCODE_ENCODING_H
#define TCODE_ENCODING_H
#include <string>
#include "../Utils/TCodeTaggedDataContainer.h"


class TCodeEncoder
{
public:
    //#<name id>:<value 0 - false or 1 - true> - bool
    //$<name id>:<- if negative><value> - int,long
    //@<name id>:<value> - uint,ulong
    // float,string are not supported currently for encoding
    static bool encodeValue(TCodeTaggedDataContainer &data, char *buffer, const size_t length);

    //#<name id>:<value 0 - false or 1 - true>
    static bool encodeValue(const char *name, bool value, char *buffer, const size_t length);

    //$<name id>:<- if negative><value>
    static bool encodeValue(const char *name, short value, char *buffer, const size_t length);

    //@<name id>:<value>
    static bool encodeValue(const char *name, unsigned short value, char *buffer, const size_t length);
};

#endif