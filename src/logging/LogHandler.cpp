#include "LogHandler.h"
	
LogHandler::LogHandler() {
	m_xMutex = xSemaphoreCreateMutex();
	m_currentLogLevel = LogLevel::INFO;
	m_filterDuplicates = true;
}

void LogHandler::setLogLevel(LogLevel logLevel) {
	m_currentLogLevel = logLevel;
}

void LogHandler::setFilterDuplicates(bool enabled) {
	m_filterDuplicates = enabled;
}
    
LogLevel LogHandler::getLogLevel() {
	return m_currentLogLevel;
}

void LogHandler::setIncludes(std::vector<String> tags) {
	clearIncludes();
	for(size_t i = 0; i < tags.size(); i++) {
		m_tags.push_back(tags[i]);
	}
}
	
const std::vector<String> LogHandler::getIncludes() {
	return m_tags;
}

void LogHandler::clearIncludes() {
	m_tags.clear();
}

bool LogHandler::addInclude(const char* tag) {
	std::vector<String>::iterator position = std::find(m_tags.begin(), m_tags.end(), tag);
	if (position == m_tags.end())
		m_tags.push_back(tag);
	else
		return false;
	return true;
}

bool LogHandler::removeInclude(const char* tag) {
	std::vector<String>::iterator position = std::find(m_tags.begin(), m_tags.end(), tag);
	if (position != m_tags.end())
		m_tags.erase(position);
	else
		return false;
	return true;
}

void LogHandler::setExcludes(std::vector<String> tags) {
	clearExcludes();
	for(size_t i = 0; i < tags.size(); i++) {
		m_filters.push_back(tags[i]);
	}
}

const std::vector<String> LogHandler::getExcludes() {
	return m_filters;
}

void LogHandler::clearExcludes() {
	m_filters.clear();
}

bool LogHandler::addExclude(const char* tag) {
	std::vector<String>::iterator position = std::find(m_filters.begin(), m_filters.end(), tag);
	if (position == m_filters.end()) {
		m_filters.push_back(tag);
	} else {
		return false;
	}
	return true;
}

bool LogHandler::removeExclude(const char* tag) {
	std::vector<String>::iterator position = std::find(m_filters.begin(), m_filters.end(), tag);
	if (position != m_filters.end()) {
		m_filters.erase(position);
	} else {
		return false;
	}
	return true;
}

void LogHandler::verbose(const char *tag, const char *format, ...) {
	if(m_currentLogLevel >= LogLevel::VERBOSE) {
		xSemaphoreTake(m_xMutex, portMAX_DELAY);
		if(isLogged(tag)) {
			va_list vArgs;
			va_start(vArgs, format);
			parseMessage(format, "VERBOSE", tag, LogLevel::VERBOSE, vArgs);
			va_end(vArgs);
		}
		xSemaphoreGive(m_xMutex);
	}
}

void LogHandler::debug(const char *tag, const char *format, ...) {
	if(m_currentLogLevel >= LogLevel::DEBUG) {
		xSemaphoreTake(m_xMutex, portMAX_DELAY);
		if(isLogged(tag)) {
			va_list vArgs;
			va_start(vArgs, format);
			parseMessage(format, "DEBUG", tag, LogLevel::DEBUG, vArgs);
			va_end(vArgs);
		}
		xSemaphoreGive(m_xMutex);
	}
}

void LogHandler::error(const char *tag, const char *format, ...) {
	if(m_currentLogLevel >= LogLevel::ERROR) {
		xSemaphoreTake(m_xMutex, portMAX_DELAY);
		if(isLogged(tag)) {
			va_list vArgs;
			va_start(vArgs, format);
			parseMessage(format, "ERROR", tag, LogLevel::ERROR, vArgs);
			va_end(vArgs);
		}
		xSemaphoreGive(m_xMutex);
	}
}

void LogHandler::warning(const char *tag, const char *format, ...) {
	if(m_currentLogLevel >= LogLevel::WARNING) {
		xSemaphoreTake(m_xMutex, portMAX_DELAY);
		if(isLogged(tag)) {
			va_list vArgs;
			va_start(vArgs, format);
			parseMessage(format, "WARNING", tag, LogLevel::WARNING, vArgs);
			va_end(vArgs);
		}
		xSemaphoreGive(m_xMutex);
	}
}

void LogHandler::info(const char *tag, const char *format, ...) {
	if(m_currentLogLevel >= LogLevel::INFO) {
		xSemaphoreTake(m_xMutex, portMAX_DELAY);
		if(isLogged(tag)) {
			va_list vArgs;
			va_start(vArgs, format);
			parseMessage(format, "INFO", tag, LogLevel::INFO, vArgs);
			va_end(vArgs);
		}
		xSemaphoreGive(m_xMutex);
	}
}

const char* LogHandler::getLastError() {
	return m_lastError;
}

void LogHandler::setMessageCallback(LOG_FUNCTION_PTR_T f){
	m_message_callback = f == nullptr ? 0 : f;
}
	
void LogHandler::parseMessage(const char* valueFormat, const char* level, const char* tag, LogLevel logLevel, va_list vArgs) {
	try {
		if(strlen(valueFormat) > internal_buffer_length) {
			Serial.println("Log value too big for buffer");
			return;
		}
		char temp[internal_buffer_length] = {'\0'};
		int len = vsnprintf(temp, internal_buffer_length-1, valueFormat, vArgs);
		
		if(len < 0) {
			Serial.println("Error printing vargs");
			return;
		}

		for (size_t i = internal_buffer_length - 1; i >= 0; --i) {
			if ((temp[i] != '\n') && (temp[i] != '\r') && (temp[i] != ' ') && (i < len)) {
				break;
			}
			temp[i] = 0;
		}
		
		if(m_filterDuplicates)
		{
		 	switch(logLevel)
			{
				case LogLevel::ERROR:   if(strcmp(m_lastError, temp) == 0) return; break;
				case LogLevel::VERBOSE: if(strcmp(m_lastVerbose, temp) == 0) return; break;
				case LogLevel::DEBUG:   if(strcmp(m_lastDebug, temp) == 0) return; break;
			}
		}

		Serial.printf("%s %s: %s\n", level, tag, temp);
		switch(logLevel)
		{
			case LogLevel::ERROR:   strncpy(m_lastError,    temp, internal_buffer_length); break;
			case LogLevel::VERBOSE: strncpy(m_lastVerbose,  temp, internal_buffer_length); break;
			case LogLevel::DEBUG:   strncpy(m_lastDebug,    temp, internal_buffer_length); break;
		}
		
		if(m_message_callback)
			m_message_callback(temp, len, logLevel);
		
	} catch (...) {
		Serial.print("Error processing log message.");
		Serial.println(valueFormat);
	}
}
	
bool LogHandler::isTagged(const char* tag) {
	if(m_tags.empty())
		return true;//tag all by default
	std::vector<String>::iterator position = std::find(m_tags.begin(), m_tags.end(), tag);
	return position != m_tags.end();
}
	
bool LogHandler::isFiltered(const char* tag) {
	if(m_filters.empty())
		return false;
	std::vector<String>::iterator position = std::find(m_filters.begin(), m_filters.end(), tag);
	return position != m_filters.end();
}

bool LogHandler::isLogged(const char* tag) {
	bool tagged = isTagged(tag);
	bool filtered = isFiltered(tag);
	return tagged && !filtered;
}