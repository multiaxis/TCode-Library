// TCode-C-String-Utils-Class-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 29/02/2024
// Please copy, share, learn, innovate, give attribution.
#pragma once
#ifndef TCODE_CSTRING_UTILS_H
#define TCODE_CSTRING_UTILS_H

class TCodeCStringUtils
{
public:
    /**
     * @brief Converts Lowecase Ascii to Uppercase
     * @param value is the value of the char which needs converting to uppercase
     * @return returns the uppercase representation of the inputted ascii char
     */
    static char toupper(const char value)
    {
        return ((value >= 'a') && (value <= 'z')) ? ('A' + (value - 'a')) : (value);
    }

    /**
     * @brief Checks if the inputed ascii char is a number
     * @param value is the char value which needs checking
     * @return returns true for if the inputted char is a number
     */
    static bool isnumber(const char value)
    {
        switch (value) // Check for numbers
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return true;
        default:
            return false;
        }
    }

    /**
     * @brief Checks if an inputted char is one used for an extention Command in the decoding of TCode
     * @param value is the char value which needs checking
     * @return returns true if the inputted char is used as an extention character
     */
    static bool isextention(const char value)
    {
        switch (toupper(value)) // checks if the inputted char is used as an extention character
        {
        case 'I':
        case 'S':
            return true;
        default:
            return false;
        }
    }

    /**
     * @brief Checks if an inputted char is one used for an ramp Command in the decoding of TCode
     * @param value is the char value which needs checking
     * @return returns true if the inputted char is used as an ramp character
     */
    static bool isramp(const char value)
    {
        switch (toupper(value)) // checks if the inputted char is used as an extention character
        {
        case '<':
        case '>':
        case '=':
            return true;
        default:
            return false;
        }
    }

    /**
     * @brief In a string at the index pointed to by the index paramater gets the Integer contained at and after incrementing the index value
     * @param buffer string to be processed
     * @param length length of buffer
     * @param index the index pointing to a position in the string
     * @return returns a long the integer found in the string represented as base 10 unsigned if a integer string is found which is less than 4 characters long then it will be multiplied til it reaches that minimum e.g. "1" = 1000 ,"01" = 100
     */
    static long getNextInt(char *buffer, const size_t length, size_t &index)
    {
        size_t count = 0;
        long accum = 0;
        while (isnumber(*(buffer + index))) // while there is a number at the index we are at in the string
        {
            accum *= 10;                                                  // multiply the accumulator first to get the correct output value
            accum += static_cast<long>(toupper(*(buffer + index)) - '0'); // get next int value '0' - '9' subtracting '0' gets the integer value of the next unit
            index++;
            count++; // increase the indeces count to count the digits
        }

        if (count == 0) // no chars were found with 0 - 9
            return -1;

        return accum;
    }

    /**
     * @brief In a string at the index pointed to by the index paramater gets the Integer contained at and after incrementing the index value
     * @param buffer string to be processed
     * @param length length of buffer
     * @param index the index pointing to a position in the string
     * @return returns a long the integer found in the string represented as base 10 unsigned if a integer string is found which is less than 4 characters long then it will be multiplied til it reaches that minimum e.g. "1" = 1000 ,"01" = 100
     */
    static long getNextTCodeInt(char *buffer, const size_t length, size_t &index)
    {
        size_t count = 0;
        long accum = 0;
        while (isnumber(*(buffer + index))) // while there is a number at the index we are at in the string
        {
            accum *= 10;                                                  // multiply the accumulator first to get the correct output value
            accum += static_cast<long>(toupper(*(buffer + index)) - '0'); // get next int value '0' - '9' subtracting '0' gets the integer value of the next unit
            index++;
            count++; // increase the indeces count to count the digits
        }

        if (count == 0) // no chars were found with 0 - 9
            return -1;

        while (count < 4) // if less than 4 digits were found make up for it by multiplying eg 1 = 1000, 90 = 9000, 001 = 10
        {
            accum *= 10;
            count++;
        }

        return accum;
    }

    /**
     * @brief Gets the char at an index in an inputted buffer
     * @param buffer pointer to buffer to be processed
     * @param length length of buffer
     * @param index index to get char from
     * @return returns a char pointed to at the index position in the buffer (returns '\0' if out of range)
     **/
    static char getCharAt(char *buffer, const size_t length, size_t index)
    {
        if (index >= length)
            return '\0';
        return *(buffer + index);
    }

    /**
     * @brief sets the char at an index in an inputted buffer
     * @param value char to be placed at the index
     * @param buffer pointer to buffer to be processed
     * @param length length of buffer
     * @param index index to get char from
     * @return returns true if the value was written;
     **/
    static bool setCharAt(char value,char *buffer, const size_t length, size_t index)
    {
        if (index >= length)
            return false;
        buffer[index] = value;
        return true;
    }

    /**
     * @brief Puts a converted int to string into a buffer
     * @param value integer to be converted
     * @param buffer pointer to buffer where string is going to be put
     * @param length length of buffer
     * @param placeIndex index to put the converted int
     * @return returns length of writen value
     **/
    static size_t uintToStr(unsigned long value, char *buffer, const size_t length, const size_t placeIndex)
    {
        const int base = 10;
        char buf[31];
        int i = 30;

        for (int k = 0; k < 30; k++)
            buf[k] = '\0';

        if (value == 0)
        {
            i = 29;
            buf[29] = '0';
        }

        for (; (value > 0) && (i > 0); --i, value /= base)
        {
            buf[i - 1] = "0123456789abcdef"[value % base];
        }

        size_t count = 0;
        for (; i < 30; i++, count++)
        {
            if ((count + placeIndex) > length)
            {
                break;
            }
            buffer[count + placeIndex] = buf[i];
        }
        return count;
    }

    static size_t uintToStrLen(unsigned long value)
    {
        const int base = 10;
        if (value == 0)
        {
            return 1;
        }
        size_t i = 0;
        for (; (value > 0) && (i <= 30); ++i, value /= base) {}
        return i;
    }
};


#endif