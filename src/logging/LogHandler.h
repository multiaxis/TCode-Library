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
#include <mutex>
#include <vector>

enum class LogLevel {
    NONE,
    INFO,
    WARNING,
    ERROR,
    DEBUG,
    VERBOSE
};

using LOG_FUNCTION_PTR_T = void (*)(const char* input, size_t length, LogLevel level);
class LogHandler {
public:
	static const int internal_buffer_length = 1024;
	LogHandler();

    void setLogLevel(LogLevel logLevel);
    void setFilterDuplicates(bool enabled);
    LogLevel getLogLevel();
	
	bool addInclude(const char* tag);
    void setIncludes(std::vector<String> tags);
    const std::vector<String> getIncludes();
	bool removeInclude(const char* tag);
    void clearIncludes();

	bool addExclude(const char* tag);
    void setExcludes(std::vector<String> tags);
    const std::vector<String> getExcludes();
    bool removeExclude(const char* tag);
	void clearExcludes();

    void verbose(const char *tag, const char *format, ...);
    void debug(const char *tag, const char *format, ...);
    void info(const char *tag, const char *format, ...);
    void warning(const char *tag, const char *format, ...);
    void error(const char *tag, const char *format, ...);

    const char* getLastError();
	void setMessageCallback(LOG_FUNCTION_PTR_T f);
	
private:
	
    LOG_FUNCTION_PTR_T m_message_callback;
    LogLevel m_currentLogLevel;
	SemaphoreHandle_t m_xMutex;
    std::vector<String> m_tags;
    std::vector<String> m_filters;
    char m_lastVerbose[internal_buffer_length];
    char m_lastDebug[internal_buffer_length];
    char m_lastError[internal_buffer_length];
	bool m_filterDuplicates;
	
    void parseMessage(const char* valueFormat, const char* level, const char* tag, LogLevel logLevel, va_list vArgs);
    bool isTagged(const char* tag);
    bool isFiltered(const char* tag);
    bool isLogged(const char* tag);
};

static LogHandler logging;