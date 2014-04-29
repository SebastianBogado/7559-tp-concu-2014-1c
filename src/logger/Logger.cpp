/*
*
*
*	Logger.cpp
*/

#include "logger/Logger.h"

void Logger::log(std::string msg, unsigned short int logLevel) {
	if (logLevel >= _logLevel) {
		_currentTime = localtime(&time(0));
		_output << _currentTime->tm_mday << "/"	<< _currentTime->tm_mon + 1	<< "/" << _currentTime->tm_year + 1900
				<< " " << _currentTime->tm_hour << ":" << _currentTime->tm_min << ":" << _currentTime->tm_sec << " "
				<< msg << std::endl;
	}
};
