// TCode-Buffer-class v1.0,
// protocal by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 04/06/2022
// usage of this class can be found at (https://github.com/Dreamer2345/Arduino_TCode_Parser)
// Please copy, share, learn, innovate, give attribution.
//

#pragma once
#ifndef TCODE_BUFFER_H
#define TCODE_BUFFER_H

template <class BufferType, unsigned TCODE_BUFFER_LENGTH = 127>
class TCodeBuffer
{
private:
    unsigned front = 0;
    unsigned back = 0;
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

    bool get(const unsigned &index, BufferType &success);
};

template <class BufferType, unsigned TCODE_BUFFER_LENGTH>
bool TCodeBuffer<BufferType, TCODE_BUFFER_LENGTH>::isEmpty()
{
    return front == back;
}

template <class BufferType, unsigned TCODE_BUFFER_LENGTH>
void TCodeBuffer<BufferType, TCODE_BUFFER_LENGTH>::clear()
{
    front = back;
}

template <class BufferType, unsigned TCODE_BUFFER_LENGTH>
bool TCodeBuffer<BufferType, TCODE_BUFFER_LENGTH>::get(const unsigned &index, BufferType &success)
{
    if (index >= count())
    {
        return false;
    }

    unsigned correctedIndex = (front + index) % TCODE_BUFFER_LENGTH;
    success = buffer[correctedIndex];
    return true;
}

template <class BufferType, unsigned TCODE_BUFFER_LENGTH>
bool TCodeBuffer<BufferType, TCODE_BUFFER_LENGTH>::isFull()
{
    return (front == ((back + 1) % TCODE_BUFFER_LENGTH));
}

template <class BufferType, unsigned TCODE_BUFFER_LENGTH>
unsigned TCodeBuffer<BufferType, TCODE_BUFFER_LENGTH>::count()
{
    if (back >= front)
        return back - front;

    return TCODE_BUFFER_LENGTH - (front - back);
}

template <class BufferType, unsigned TCODE_BUFFER_LENGTH>
bool TCodeBuffer<BufferType, TCODE_BUFFER_LENGTH>::push(BufferType obj)
{
    if (isFull())
    {
        return false;
    }
    buffer[back] = obj;
    back = (back + 1) % TCODE_BUFFER_LENGTH;
    return true;
}

template <class BufferType, unsigned TCODE_BUFFER_LENGTH>
bool TCodeBuffer<BufferType, TCODE_BUFFER_LENGTH>::peekBack(BufferType &obj)
{
    if (isEmpty())
    {
        return false;
    }

    if ((int)back - 1 < 0)
        obj = buffer[TCODE_BUFFER_LENGTH];
    else
        obj = buffer[back - 1];
    return true;
}

template <class BufferType, unsigned TCODE_BUFFER_LENGTH>
bool TCodeBuffer<BufferType, TCODE_BUFFER_LENGTH>::pop(BufferType &success)
{
    if (isEmpty())
    {
        return false;
    }

    success = buffer[front];
    front = (front + 1) % TCODE_BUFFER_LENGTH;
    return true;
}

template <class BufferType, unsigned TCODE_BUFFER_LENGTH>
bool TCodeBuffer<BufferType, TCODE_BUFFER_LENGTH>::peek(BufferType &success)
{
    if (isEmpty())
    {
        return false;
    }

    success = buffer[front];
    return true;
}

template <class BufferType, unsigned TCODE_BUFFER_LENGTH>
BufferType TCodeBuffer<BufferType, TCODE_BUFFER_LENGTH>::pop()
{
    if (isEmpty())
    {
        return 0;
    }

    BufferType headItem = buffer[front];
    front = (front + 1) % TCODE_BUFFER_LENGTH;
    return headItem;
}

template <class BufferType, unsigned TCODE_BUFFER_LENGTH>
BufferType TCodeBuffer<BufferType, TCODE_BUFFER_LENGTH>::peek()
{
    if (isEmpty())
    {
        return 0;
    }

    BufferType headItem = buffer[front];
    return headItem;
}

#endif
