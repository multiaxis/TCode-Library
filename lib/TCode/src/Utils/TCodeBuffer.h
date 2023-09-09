// TCode-Buffer-class v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 04/06/2022
// usage of this class can be found at (https://github.com/Dreamer2345/Arduino_TCode_Parser)
// Please copy, share, learn, innovate, give attribution.
// Decodes T-code commands
// It can handle:
//   x linear channels (L0, L1, L2... L9)
//   x rotation channels (R0, R1, R2... L9)
//   x vibration channels (V0, V1, V2... V9)
//   x auxilliary channels (A0, A1, A2... A9)
// History:
//

#pragma once
#ifndef TCODE_BUFFER_H
#define TCODE_BUFFER_H

template <class BufferType,unsigned TCODE_BUFFER_LENGTH = 127>
class TCodeBuffer
{
private:
    unsigned Front = 0;
    unsigned Back = 0;
    BufferType buffer[TCODE_BUFFER_LENGTH];

public:
    bool isFull();
    bool isEmpty();
    void clear();

    unsigned count();

    bool push(BufferType obj);
    bool pop(BufferType &success);
    bool peek(BufferType &success);
    BufferType pop();
    BufferType peek();

    bool peekBack(BufferType &obj);

    bool get(const unsigned &index,BufferType &success);
};

template <class BufferType,unsigned TCODE_BUFFER_LENGTH>
bool TCodeBuffer<BufferType,TCODE_BUFFER_LENGTH>::isEmpty()
{
    return Front == Back;
}

template <class BufferType,unsigned TCODE_BUFFER_LENGTH>
void TCodeBuffer<BufferType,TCODE_BUFFER_LENGTH>::clear()
{
    Front = Back;
}

template <class BufferType,unsigned TCODE_BUFFER_LENGTH>
bool TCodeBuffer<BufferType,TCODE_BUFFER_LENGTH>::get(const unsigned &index,BufferType &success)
{
    if(index >= count())
    {
        return false;
    }

    
    unsigned correctedIndex = (Front + index) % TCODE_BUFFER_LENGTH;
    success = buffer[correctedIndex];
    return true;
}

template <class BufferType,unsigned TCODE_BUFFER_LENGTH>
bool TCodeBuffer<BufferType,TCODE_BUFFER_LENGTH>::isFull()
{
    return (Front == ((Back + 1) % TCODE_BUFFER_LENGTH));
}

template <class BufferType,unsigned TCODE_BUFFER_LENGTH>
unsigned TCodeBuffer<BufferType,TCODE_BUFFER_LENGTH>::count()
{
    if (Back >= Front)
        return Back - Front;

    return TCODE_BUFFER_LENGTH - (Front - Back);
}

template <class BufferType,unsigned TCODE_BUFFER_LENGTH>
bool TCodeBuffer<BufferType,TCODE_BUFFER_LENGTH>::push(BufferType obj)
{
    if (isFull())
    {
        return false;
    }
    buffer[Back] = obj;
    Back = (Back + 1) % TCODE_BUFFER_LENGTH;
    return true;
}

template <class BufferType,unsigned TCODE_BUFFER_LENGTH>
bool TCodeBuffer<BufferType,TCODE_BUFFER_LENGTH>::peekBack(BufferType &obj)
{
    if (isEmpty())
    {
        return false;
    }

    
    if((int)Back - 1 < 0)
        obj = buffer[TCODE_BUFFER_LENGTH];
    else
        obj = buffer[Back - 1];
    return true;
}

template <class BufferType,unsigned TCODE_BUFFER_LENGTH>
bool TCodeBuffer<BufferType,TCODE_BUFFER_LENGTH>::pop(BufferType &success)
{
    if (isEmpty())
    {
        return false;
    }

    success = buffer[Front];
    Front = (Front + 1) % TCODE_BUFFER_LENGTH;
    return true;
}

template <class BufferType,unsigned TCODE_BUFFER_LENGTH>
bool TCodeBuffer<BufferType,TCODE_BUFFER_LENGTH>::peek(BufferType &success)
{
    if (isEmpty())
    {
        return false;
    }


    success = buffer[Front];
    return true;
}

template <class BufferType,unsigned TCODE_BUFFER_LENGTH>
BufferType TCodeBuffer<BufferType,TCODE_BUFFER_LENGTH>::pop()
{
    if (isEmpty())
    {
        return 0;
    }

    BufferType headItem = buffer[Front];
    Front = (Front + 1) % TCODE_BUFFER_LENGTH;
    return headItem;
}

template <class BufferType,unsigned TCODE_BUFFER_LENGTH>
BufferType TCodeBuffer<BufferType,TCODE_BUFFER_LENGTH>::peek()
{
    if (isEmpty())
    {
        return 0;
    }

    BufferType headItem = buffer[Front];
    return headItem;
}

#endif
