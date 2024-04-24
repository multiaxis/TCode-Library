// TCode-C-String-Utils-Class-H v1.0, 
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 29/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include "TMath.h"
#include "../settings/TaggedDataContainer.h"

namespace TCode::TString {

bool getNextInt(size_t &index, const char *buffer, const size_t length, unsigned long &value, size_t &log);

bool getNextTCodeFloat(size_t &index, const char *buffer, const size_t length, float &value, size_t &log);

char getCharAtOrDefault(const size_t index, const char *buffer, const size_t length, const char defaultValue = '\0');

bool setCharAt(char value, char *buffer, const size_t length, size_t index);

size_t uintToStr(unsigned long value, char *buffer, const size_t length, const size_t placeIndex);

size_t uintToStrLen(unsigned long value);

bool encodeValue(const char *name, bool value, char *buffer, const size_t length);

bool encodeValue(const char *name, short value, char *buffer, const size_t length);

bool encodeValue(const char *name, unsigned short value, char *buffer, const size_t length);

bool encodeValue(TaggedDataContainer &data, char *buffer, const size_t length);

unsigned long getHash(const char *str, size_t length);

char axisTypeToChar(const AxisType &type);

String axisIdToString(const AxisId &id);

}
