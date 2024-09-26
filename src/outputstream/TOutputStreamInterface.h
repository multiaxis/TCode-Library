// TCode-Output-Stream-Interface-H v1.0,
// protocol by TempestMAx (https://www.patreon.com/tempestvr)
// implemented by Eve 16/08/2023
// Please copy, share, learn, innovate, give attribution.
//
#pragma once
#include <Arduino.h>

namespace TCode::Output {

    /**
     * @brief Interface for output stream operations in the TCode system.
     *
     * The `TCodeIOutputStream` class defines a virtual interface for writing data to an output
     * stream. It provides various methods for writing and printing different types of data,
     * including single characters, strings, and Arduino-specific `__FlashStringHelper` objects.
     * Each method must be implemented by any concrete class that inherits from `TCodeIOutputStream`.
     */
    class TCodeIOutputStream {
    public:
        /**
         * @brief Writes a single character to the output stream.
         *
         * This pure virtual method must be implemented by derived classes to write a single character
         * to the output stream.
         *
         * @param value The character to be written.
         */
        virtual void write(const char value) const = 0;

        /**
         * @brief Writes a C-style string to the output stream.
         *
         * This pure virtual method must be implemented by derived classes to write a null-terminated
         * C-style string to the output stream.
         *
         * @param value A pointer to the null-terminated string to be written.
         */
        virtual void write(const char *value) const = 0;

        /**
         * @brief Writes a flash memory string (Arduino specific) to the output stream.
         *
         * This method must be implemented by derived classes to write an `__FlashStringHelper` object
         * (typically used for storing constant strings in flash memory on Arduino devices) to the output stream.
         *
         * @param value A pointer to the `__FlashStringHelper` string to be written.
         */
        virtual void write(const __FlashStringHelper *value) const = 0;

        /**
         * @brief Writes an Arduino `String` object to the output stream.
         *
         * This pure virtual method must be implemented by derived classes to write an Arduino `String`
         * object to the output stream.
         *
         * @param value The `String` object to be written.
         */
        virtual void write(const String &value) const = 0;

        /**
         * @brief Prints a single character to the output stream.
         *
         * Similar to `write`, but meant for use cases where print formatting may be applied. This method
         * must be implemented by derived classes.
         *
         * @param value The character to be printed.
         */
        virtual void print(const char value) const = 0;

        /**
         * @brief Prints a C-style string to the output stream.
         *
         * This method must be implemented by derived classes to print a null-terminated C-style string
         * to the output stream.
         *
         * @param value A pointer to the null-terminated string to be printed.
         */
        virtual void print(const char *value) const = 0;

        /**
         * @brief Prints a flash memory string (Arduino specific) to the output stream.
         *
         * This method must be implemented by derived classes to print an `__FlashStringHelper` object
         * (used on Arduino devices) to the output stream.
         *
         * @param value A pointer to the `__FlashStringHelper` string to be printed.
         */
        virtual void print(const __FlashStringHelper *value) const = 0;

        /**
         * @brief Prints an Arduino `String` object to the output stream.
         *
         * This method must be implemented by derived classes to print an Arduino `String` object
         * to the output stream.
         *
         * @param value The `String` object to be printed.
         */
        virtual void print(const String &value) const = 0;

        /**
         * @brief Prints a single character followed by a newline to the output stream.
         *
         * This method must be implemented by derived classes to print a character and append a newline
         * character after it.
         *
         * @param value The character to be printed.
         */
        virtual void println(const char value) const = 0;

        /**
         * @brief Prints a C-style string followed by a newline to the output stream.
         *
         * This method must be implemented by derived classes to print a null-terminated C-style string
         * followed by a newline character.
         *
         * @param value A pointer to the null-terminated string to be printed.
         */
        virtual void println(const char *value) const = 0;

        /**
         * @brief Prints a flash memory string (Arduino specific) followed by a newline.
         *
         * This method must be implemented by derived classes to print an `__FlashStringHelper` object
         * followed by a newline character.
         *
         * @param value A pointer to the `__FlashStringHelper` string to be printed.
         */
        virtual void println(const __FlashStringHelper *value) const = 0;

        /**
         * @brief Prints an Arduino `String` object followed by a newline to the output stream.
         *
         * This method must be implemented by derived classes to print an Arduino `String` object
         * followed by a newline character.
         *
         * @param value The `String` object to be printed.
         */
        virtual void println(const String &value) const = 0;
    };
};