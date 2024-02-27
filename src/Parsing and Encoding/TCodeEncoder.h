// TCode-Encoder-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 27/06/2022
// Please copy, share, learn, innovate, give attribution.
#pragma once
#ifndef TCODE_ENCODING_H
#define TCODE_ENCODING_H
#include "../Utils/TCodeDataContainer.h"

class TCodeEncoder
{
public:
    // TODO:
    //   * DataContainer
    //   * bool
    //   * int
    //   * long
    //   * string
    //   * float
    // int, long, bool are converted to short 
    //<prefix based on contained type><name id>:<encoding based on contained type>
    static bool encodeTCodeDataContainer(TCodeDataContainer &value, char *buffer, size_t &index, const size_t length);

    //#<name id>:<value 0 - false or 1 - true>
    static bool encodeTCodeBool(const char *name, bool value, char *buffer, size_t &index, const size_t length);

    //$<name id>:<- if negative><value>
    static bool encodeTCodeInt(const char *name, int value, char *buffer, size_t &index, const size_t length);

    //@<name id>:<value>
    static bool encodeTCodeUInt(const char *name, unsigned int value, char *buffer, size_t &index, const size_t length);

    //$<name id>:<- if negative><value>
    static bool encodeTCodeLong(const char *name, long value, char *buffer, size_t &index, const size_t length);

    //@<name id>:<value>
    static bool encodeTCodeULong(const char *name, unsigned long value, char *buffer, size_t &index, const size_t length);

    //Undecided as of yet
    //&<name id>:"<value>"
    static bool encodeTCodeString(const char *name, const char *value, char *buffer, size_t &index, const size_t length);

    //Undecided as of yet
    //&<name id>:<- if negative><value integer part>.<fractional part>
    static bool encodeTCodeInt(const char *name, float value, char *buffer, size_t &index, const size_t length);
}

#endif