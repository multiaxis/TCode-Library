// TCode-Subject-Interface-Class v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 10/09/2023
// Please copy, share, learn, innovate, give attribution.
#pragma once

class IWritable
{
    virtual void write(unsigned char byte);
    virtual void write(unsigned char* buffer, size_t length);
};