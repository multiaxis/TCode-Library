// TCode-C-String-Utils-Class-H v1.0, 
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 29/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once

#include <Arduino.h>
#include "TMath.h"

namespace TCode::TString {

bool readInt(size_t &&index, const char *buffer, const size_t length, unsigned long &value, size_t &log);
bool readInt(size_t &index, const char *buffer, const size_t length, unsigned long &value, size_t &log);

bool readTCodeFloat(size_t &&index, const char *buffer, const size_t length, float &value, size_t &log);
bool readTCodeFloat(size_t &index, const char *buffer, const size_t length, float &value, size_t &log);

char readCharOrDefault(const size_t index, const char *buffer, const size_t length, const char defaultValue = '\0');

bool writeChar(const char value, size_t &&index, char *buffer, const size_t length);
bool writeChar(const char value, size_t &index, char *buffer, const size_t length);

bool writeString(const String &value, size_t &&index, char *buffer, const size_t length);
bool writeString(const char *value, size_t &&index, char *buffer, const size_t length);
bool writeString(const char *value, size_t count, size_t &&index, char *buffer, const size_t length);

bool writeString(const String &value, size_t &index, char *buffer, const size_t length);
bool writeString(const char *value, size_t &index, char *buffer, const size_t length);
bool writeString(const char *value, size_t count, size_t &index, char *buffer, const size_t length);

bool writeInt(long value, size_t &&index, char *buffer, const size_t length);
bool writeInt(long value, size_t &index, char *buffer, const size_t length);

unsigned long getHash(const char *str, size_t length);

char axisTypeToChar(const AxisType &type);

const char* axisTypeToVerboseString(const AxisType &type);

String axisIdToString(const AxisId &id);

}
