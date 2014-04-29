/*
*	Logger.h
*	Se encapsula la funcionalidad de log en una clase usando un singleton
*/

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <fstream>
#include <ctime>

class Logger {

private:
	std::ostream _output;
	unsigned short int _logLevel;
	struct tm* _currentTime;
	Logger _logger;

	Logger(std::ostream oFile, unsigned short int logLevel) : _output(oFile), _logLevel(logLevel) {
		_currentTime = NULL;
	};
	virtual ~Logger() {};

public:
	// TODO: ¿Algun level mas? Creo que son suficientes
	static const unsigned short int LOG_DEBUG = 1;
	static const unsigned short int LOG_NOTICE = 2;
	static const unsigned short int LOG_WARNING = 3;
	static const unsigned short int LOG_CRITICAL = 4;

	/*
	 * oFile: archivo creado y con permiso de escritura
	 * logLevel: representa uno de los 4 niveles soportados por la clase
	 * */
	static void initialize(std::ostream oFile, unsigned short int logLevel) {
		if (!_logger)	_logger = new Logger(oFile, logLevel);
	};

	static void setLogLevel( unsigned short int logLevel) {
		_logLevel = logLevel;
	}

	/*
	 * Loggea mensaje si y solo si el logLevel es mayor al setteado en la inicializacion
	 * */
	static void log(std::string msg, unsigned short int logLevel);

};

#endif
