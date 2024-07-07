#pragma once

#include <Arduino.h>

namespace TCode {

enum class VariantType {
    Empty,
    Int8,
    String,
    Boolean,
    Int32,
    Int64,
    UInt32,
    UInt64,
    Float32,
    Float64
};

union VariantValue {
    char int8;
    const char *string;
    bool boolean;
    int int32;
    long int64;
    unsigned int uint32;
    unsigned long uint64;
    float float32;
    double float64;
};

struct variant_t {
    VariantType type;
    VariantValue value;

    template <typename T>
    variant_t &operator=(const T &value) = delete;

    template <typename T>
    T &operator=(const variant_t &value) = delete;

    variant_t &operator=(char &value);
    variant_t &operator=(const char *value);
    variant_t &operator=(bool &value);
    variant_t &operator=(int &value);
    variant_t &operator=(long &value);
    variant_t &operator=(unsigned int &value);
    variant_t &operator=(unsigned long &value);
    variant_t &operator=(float &value);
    variant_t &operator=(double &value);

    template <typename T>
    bool get(T &value) = delete;

    bool get(char &value);
    bool get(const char *value);
    bool get(bool &value);
    bool get(int &value);
    bool get(long &value);
    bool get(unsigned int &value);
    bool get(unsigned long &value);
    bool get(float &value);
    bool get(double &value);
};

}
