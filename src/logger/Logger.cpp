/*
*
*
*	Logger.cpp
*/

#include "logger/Logger.h"

Logger* Logger::_logger = NULL;

void Logger::log(const std::string& msg, unsigned short int logLevel) {
	if (logLevel >= _logger->_logLevel) {
		time_t timer;
		time(&timer);
		_logger->_currentTime = localtime(&timer);
		_logger->_output 	<< _logger->_currentTime->tm_mday << "/"
							<< _logger->_currentTime->tm_mon + 1 << "/"
							<< _logger->_currentTime->tm_year + 1900 << " "
							<< _logger->_currentTime->tm_hour << ":"
							<< _logger->_currentTime->tm_min << ":"
							<< _logger->_currentTime->tm_sec << " "
							<< msg << std::endl;
	}
}
