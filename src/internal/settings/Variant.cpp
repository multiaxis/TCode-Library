#include "Variant.h"

namespace TCode {

variant_t &variant_t::operator=(char &v) {
    value.int8 = v;
    type = VariantType::Int8;
    return *this;
}

variant_t &variant_t::operator=(const char *v) {
    value.string = v;
    type = VariantType::String;
    return *this;
}

variant_t &variant_t::operator=(bool &v) {
    value.boolean = v;
    type = VariantType::Boolean;
    return *this;
}

variant_t &variant_t::operator=(int &v) {
    value.int32 = v;
    type = VariantType::Int32;
    return *this;
}

variant_t &variant_t::operator=(long &v) {
    value.int64 = v;
    type = VariantType::Int64;
    return *this;
}

variant_t &variant_t::operator=(unsigned int &v) {
    value.uint32 = v;
    type = VariantType::UInt32;
    return *this;
}

variant_t &variant_t::operator=(unsigned long &v) {
    value.uint64 = v;
    type = VariantType::UInt64;
    return *this;
}

variant_t &variant_t::operator=(float &v) {
    value.float32 = v;
    type = VariantType::Float32;
    return *this;
}

variant_t &variant_t::operator=(double &v) {
    value.float64 = v;
    type = VariantType::Float64;
    return *this;
}

bool variant_t::get(char &v) {
    if (type != VariantType::Int8)
        return false;

    v = value.int8;
    return true;
}

bool variant_t::get(const char *v) {
    if (type != VariantType::String)
        return false;

    v = value.string;
    return true;
}

bool variant_t::get(bool &v) {
    if (type != VariantType::Boolean)
        return false;

    v = value.boolean;
    return true;
}

bool variant_t::get(int &v) {
    if (type != VariantType::Int32)
        return false;

    v = value.int32;
    return true;
}

bool variant_t::get(long &v) {
    if (type != VariantType::Int64)
        return false;

    v = value.int64;
    return true;
}

bool variant_t::get(unsigned int &v) {
    if (type != VariantType::UInt32)
        return false;

    v = value.uint32;
    return true;
}

bool variant_t::get(unsigned long &v) {
    if (type != VariantType::UInt64)
        return false;

    v = value.uint64;
    return true;
}

bool variant_t::get(float &v) {
    if (type != VariantType::Float32)
        return false;

    v = value.float32;
    return true;
}

bool variant_t::get(double &v) {
    if (type != VariantType::Float64)
        return false;

    v = value.float64;
    return true;
}

}
