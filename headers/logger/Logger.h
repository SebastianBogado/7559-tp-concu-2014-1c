/*
*	Logger.h
*	Se encapsula la funcionalidad de log en una clase usando un singleton
*/

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include "sync/LockFile.h"

class Logger {

private:
	LockFile _output;
	unsigned short int _logLevel;
	struct tm* _currentTime;
	static Logger* _logger;

	Logger(const char* filename) : _output(filename), _logLevel(Logger::LOG_NOTICE), _currentTime(NULL) {};
	virtual ~Logger() {
		//delete _output;
	};

public:
	// TODO: ¿Algun level mas? Creo que son suficientes
	static const unsigned short int LOG_DEBUG = 1;
	static const unsigned short int LOG_NOTICE = 2;
	static const unsigned short int LOG_WARNING = 3;
	static const unsigned short int LOG_CRITICAL = 4;

	/*
	 * filename: nombre del archivo a crear
	 * logLevel: representa uno de los 4 niveles soportados por la clase
	 * */
	static void initialize(const char* filename, unsigned short int logLevel) {
		// TODO: Ptr Error check
		if (!_logger) {
			_logger = new Logger(filename);
			_logger->_logLevel = logLevel;
		}
	};

	static void destroy() {
		if (_logger)
			delete _logger;
	}

	static void setLogLevel( unsigned short int logLevel) {
		_logger->_logLevel = logLevel;
	}

	/*
	 * Loggea mensaje si y solo si el logLevel es mayor al setteado en la inicializacion
	 * */
	static void log(const std::string& msg, unsigned short int logLevel);

};

#endif
