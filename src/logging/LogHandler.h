/* MIT License

Copyright (c) 2024 Jason C. Fain

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */
#pragma once
#include <Arduino.h>
#include <cstdarg>
#include <mutex>
#include <vector>

namespace LogHandler {

    enum class LogLevel { NONE,
                          ERROR,
                          WARNING,
                          INFO,
                          DEBUG,
                          VERBOSE };

    using LOG_FUNCTION_PTR_T = void (*)(const char *input, size_t length,
                                        LogLevel level);

    static const int internal_buffer_length = 64;

    static LOG_FUNCTION_PTR_T m_message_callback = 0;
    static LogLevel m_currentLogLevel = LogLevel::INFO;
    static SemaphoreHandle_t m_xMutex = xSemaphoreCreateMutex();

    static std::vector<const char *> m_tags;
    static std::vector<const char *> m_filters;
    static char m_lastVerbose[internal_buffer_length];
    static char m_lastDebug[internal_buffer_length];
    static char m_lastError[internal_buffer_length];
    static bool m_filterDuplicates = false;

    static void setLogLevel(LogLevel logLevel) {
        if (logLevel != m_currentLogLevel) {
            Serial.printf("LogHandler: Log level changed to: %d\n", (uint8_t)logLevel);
            m_currentLogLevel = logLevel;
        }
    }

    static void setFilterDuplicates(bool enabled) {
        m_filterDuplicates = enabled;
    }

    static void clearIncludes() { m_tags.clear(); }
    static void clearExcludes() { m_filters.clear(); }

    static LogLevel getLogLevel() { return m_currentLogLevel; }

    static bool isTagged(const char *tag) {

        if (m_tags.empty())
            return true; // tag all by default
        std::vector<const char *>::iterator position = std::find_if(m_tags.begin(), m_tags.end(), [tag](const char *tagIn) {
            return !strcmp(tag, tagIn);
        });
        return position != m_tags.end();
    }

    static bool isFiltered(const char *tag) {

        if (m_filters.empty())
            return false;
        std::vector<const char *>::iterator position = std::find_if(m_filters.begin(), m_filters.end(), [tag](const char *tagIn) {
            return !strcmp(tag, tagIn);
        });
        return position != m_filters.end();
    }

    static bool isLogged(const char *tag) {
        bool tagged = isTagged(tag);
        bool filtered = isFiltered(tag);
        return tagged && !filtered;
    }

    static bool addInclude(const char *tag) {
        std::vector<const char *>::iterator position = std::find_if(m_tags.begin(), m_tags.end(), [tag](const char *tagIn) {
            return !strcmp(tag, tagIn);
        });
        if (position == m_tags.end()) {
            m_tags.push_back(tag);
            Serial.printf("LogHandler: add include: %s\n", tag);
        } else
            return false;
        return true;
    }

    static void setIncludes(std::vector<const char *> tags) {
        clearIncludes();
        for (size_t i = 0; i < tags.size(); i++) {
            m_tags.push_back(tags[i]);
        }
    }

    static const std::vector<const char *> getIncludes() {
        return m_tags;
    }

    static bool removeInclude(const char *tag) {
        std::vector<const char *>::iterator position = std::find_if(m_tags.begin(), m_tags.end(), [tag](const char *tagIn) {
            return !strcmp(tag, tagIn);
        });
        if (position != m_tags.end()) {
            m_tags.erase(position);
            Serial.printf("LogHandler: remove include: %s\n", tag);
        } else
            return false;
        return true;
    }

    static bool addExclude(const char *tag) {
        std::vector<const char *>::iterator position = std::find_if(m_filters.begin(), m_filters.end(), [tag](const char *tagIn) {
            return !strcmp(tag, tagIn);
        });
        if (position == m_filters.end()) { // == myVector.end() means the element was not found
            m_filters.push_back(tag);
            Serial.printf("LogHandler: add exclude: %s\n", tag);
        } else {
            return false;
        }
        return true;
    }

    static void setExcludes(std::vector<const char *> tags) {
        clearExcludes();
        for (size_t i = 0; i < tags.size(); i++) {
            m_filters.push_back(tags[i]);
        }
    }

    static const std::vector<const char *> getExcludes() {
        return m_filters;
    }

    static bool removeExclude(const char *tag) {

        std::vector<const char *>::iterator position = std::find_if(m_filters.begin(), m_filters.end(), [tag](const char *tagIn) {
            return !strcmp(tag, tagIn);
        });
        if (position != m_filters.end()) { // == myVector.end() means the element was not found
            m_filters.erase(position);
            Serial.printf("LogHandler: remove exclude: %s\n", tag);
        } else {
            return false;
        }
        return true;
    }

    static inline void parseMessage(const char *valueFormat, const char *level, const char *tag, LogLevel logLevel, va_list vArgs) {
        static char local_buffer[internal_buffer_length] = {'\0'};
        char *temp = local_buffer;
        int len = 0;

        va_list vArgsCopy;
        va_copy(vArgsCopy, vArgs);
        len = vsnprintf(NULL, 0, valueFormat, vArgsCopy);
        va_end(vArgsCopy);

        if (len >= sizeof(local_buffer)) {
            temp = (char *)malloc(len + 1);
            if (temp == NULL) {
                return;
            }
        }

        vsnprintf(temp, len + 1, valueFormat, vArgs);
        temp[len] = '\0';

        bool filtered = false;
        if (m_filterDuplicates) {
            switch (logLevel) {
            case LogLevel::ERROR:
                if (strcmp(m_lastError, temp) == 0)
                    filtered = true;
                break;
            case LogLevel::VERBOSE:
                if (strcmp(m_lastVerbose, temp) == 0)
                    filtered = true;
                break;
            case LogLevel::DEBUG:
                if (strcmp(m_lastDebug, temp) == 0)
                    filtered = true;
                break;
            default:
                break;
            }
        }

        if (!filtered)
            Serial.printf("%s %s: %s\n", level, tag, temp);
        switch (logLevel) {
        case LogLevel::ERROR:
            strncpy(m_lastError, temp, internal_buffer_length);
            break;
        case LogLevel::VERBOSE:
            strncpy(m_lastVerbose, temp, internal_buffer_length);
            break;
        case LogLevel::DEBUG:
            strncpy(m_lastDebug, temp, internal_buffer_length);
            break;
        default:
            break;
        }

        if (m_message_callback)
            m_message_callback(temp, len, logLevel);

        if (len >= sizeof(local_buffer)) {
            free(temp);
        }
    }

    static void info(const char *tag, const char *format, ...) {
        xSemaphoreTake(m_xMutex, portMAX_DELAY);
        if (m_currentLogLevel >= LogLevel::INFO) {

            if (isLogged(tag)) {
                va_list vArgs;
                va_start(vArgs, format);
                parseMessage(format, "[INFO]", tag, LogLevel::INFO, vArgs);
                va_end(vArgs);
            }
        }
        xSemaphoreGive(m_xMutex);
    }

    static void warning(const char *tag, const char *format, ...) {
        xSemaphoreTake(m_xMutex, portMAX_DELAY);
        if (m_currentLogLevel >= LogLevel::WARNING) {

            if (isLogged(tag)) {
                va_list vArgs;
                va_start(vArgs, format);
                parseMessage(format, "[WARNING]", tag, LogLevel::WARNING, vArgs);
                va_end(vArgs);
            }
        }
        xSemaphoreGive(m_xMutex);
    }

    static void error(const char *tag, const char *format, ...) {
        xSemaphoreTake(m_xMutex, portMAX_DELAY);
        if (m_currentLogLevel >= LogLevel::ERROR) {
            if (isLogged(tag)) {
                va_list vArgs;
                va_start(vArgs, format);
                parseMessage(format, "[ERROR]", tag, LogLevel::ERROR, vArgs);
                va_end(vArgs);
            }
        }
        xSemaphoreGive(m_xMutex);
    }

    static void debug(const char *tag, const char *format, ...) {
        xSemaphoreTake(m_xMutex, portMAX_DELAY);
        if (m_currentLogLevel >= LogLevel::DEBUG) {
            if (isLogged(tag)) {
                va_list vArgs;
                va_start(vArgs, format);
                parseMessage(format, "[DEBUG]", tag, LogLevel::DEBUG, vArgs);
                va_end(vArgs);
            }
        }
        xSemaphoreGive(m_xMutex);
    }

    static void verbose(const char *tag, const char *format, ...) {
        xSemaphoreTake(m_xMutex, portMAX_DELAY);
        if (m_currentLogLevel >= LogLevel::VERBOSE) {
            if (isLogged(tag)) {
                va_list vArgs;
                va_start(vArgs, format);
                parseMessage(format, "[VERBOSE]", tag, LogLevel::VERBOSE, vArgs);
                va_end(vArgs);
            }
        }
        xSemaphoreGive(m_xMutex);
    }

    static const char *getLastError() { return m_lastError; }

    static void setMessageCallback(LOG_FUNCTION_PTR_T f) {
        m_message_callback = (f == nullptr) ? 0 : f;
    }

}
