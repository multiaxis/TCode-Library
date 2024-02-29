// TCode-Buffer-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 04/06/2022
// Please copy, share, learn, innovate, give attribution.
//
#pragma once
#ifndef TCODE_BUFFER_H
#define TCODE_BUFFER_H
#include <vector>

template <class BufferType>
class TCodeBuffer
{
private:
    size_t size;
    size_t capacity;
    size_t front;
    size_t back;
    std::vector<BufferType> buffer;

public:
    TCodeBuffer(size_t capacity) : buffer(capacity),capacity(capacity),size(0),front(0),back(0) {}

    bool full() const;
    bool empty() const;
    void clear();

    size_t count() const;

    bool push(BufferType obj);
    bool pop(BufferType &success);
    bool peek(BufferType &success);

    BufferType pop();
    BufferType peek();
    BufferType peekBack();

    bool peekBack(BufferType &obj);

    bool get(const size_t index, BufferType &success);
    bool set(const size_t index, const BufferType &obj);

    bool resize(size_t newSize);
};


template <class BufferType>
bool TCodeBuffer<BufferType>::empty() const
{
    return size == 0;
}


template <class BufferType>
bool TCodeBuffer<BufferType>::full() const
{
    return size == capacity;
}

template <class BufferType>
size_t TCodeBuffer<BufferType>::count() const
{
    return size;
}


template <class BufferType>
void TCodeBuffer<BufferType>::clear()
{
    size = 0;
    front = 0;
    back = 0;
}

template <class BufferType>
bool TCodeBuffer<BufferType>::get(const size_t index, BufferType &success)
{
    if (index >= size)
    {
        return false;
    }

    unsigned correctedIndex = (front + index) % capacity;
    success = buffer[correctedIndex];
    return true;
}

template <class BufferType>
inline bool TCodeBuffer<BufferType>::set(const size_t index, const BufferType &obj)
{
    if (index >= size)
    {
        return false;
    }

    unsigned correctedIndex = (front + index) % capacity;
    buffer[correctedIndex] = obj;
    return true;
}

template <class BufferType>
inline bool TCodeBuffer<BufferType>::resize(size_t newSize)
{
    buffer.resize(newSize);
    capacity = newSize;
    return false;
}

template <class BufferType>
bool TCodeBuffer<BufferType>::push(BufferType obj)
{
    if (full())
    {
        return false;
    }

    buffer[back] = obj;
    back = (back + 1) % capacity;
    size++;
    return true;
}

template <class BufferType>
bool TCodeBuffer<BufferType>::peekBack(BufferType &obj)
{
    if (empty())
    {
        return false;
    }

    obj = buffer[(back == 0) ? (capacity - 1) : (back - 1)];
    return true;
}

template <class BufferType>
bool TCodeBuffer<BufferType>::pop(BufferType &success)
{
    if (empty())
    {
        return false;
    }

    success = buffer[front];
    front = (front + 1) % capacity;
    size--;
    return true;
}

template <class BufferType>
bool TCodeBuffer<BufferType>::peek(BufferType &success)
{
    if (empty())
    {
        return false;
    }

    success = buffer[front];
    return true;
}


template <class BufferType>
BufferType TCodeBuffer<BufferType>::pop()
{
    if (empty())
    {
        return BufferType();
    }

    BufferType success = buffer[front];
    front = (front + 1) % capacity;
    size--;
    return success;
}

template <class BufferType>
BufferType TCodeBuffer<BufferType>::peek()
{
    if (empty())
    {
        return BufferType();
    }

    BufferType success = buffer[front];
    return success;
}

template <class BufferType>
BufferType TCodeBuffer<BufferType>::peekBack()
{
    if (empty())
    {
        return BufferType();
    }

    return buffer[(back == 0) ? (capacity - 1) : (back - 1)];
}

#endif
