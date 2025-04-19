#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <Arduino.h>
#include <ErrorType.h>

template <typename T, size_t Size>
class CircularBuffer
{
private:
    T buffer[Size];
    size_t head = 0;
    size_t count = 0;

public:
    void push(T value)
    {
        buffer[head] = value;
        head = (head + 1) % Size;
        if (count == Size)
            return;
        count++;
    }

    ErrorType pop(T &result)
    {
        if (empty())
        {
            return ErrorType::EMPTY;
        }

        result = buffer[head];
        head = (head - 1 + Size) % Size;
        count--;
        return ErrorType::OK;
    }

    ErrorType get(size_t index, T &result)
    {
        if (index >= count)
        {
            return ErrorType::OUT_OF_RANGE;
        }

        size_t actualIndex = (head - index - 1 + Size) % Size;
        result = buffer[actualIndex];
        return ErrorType::OK;
    }

    ErrorType toArray(T *&array)
    {
        if (empty())
        {
            array = nullptr;
            return ErrorType::EMPTY;
        }

        array = new T[count];
        for (size_t i = 0; i < count; i++)
        {
            // Reverse order filling
            get(i, array[count - 1 - i]);
        }
        return ErrorType::OK;
    }

    bool empty() const
    {
        return count == 0;
    }

    size_t len() const
    {
        return count;
    }
};

#endif