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
	std::ofstream _output;
	unsigned short int _logLevel;
	struct tm* _currentTime;
	static Logger* _logger;

	Logger() : _logLevel(Logger::LOG_NOTICE), _currentTime(NULL) {};
	virtual ~Logger() {
		_output.close();
	};

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
	static void initialize(const char* filename, unsigned short int logLevel) {
		// TODO: Ptr Error check
		if (!_logger)
			_logger = new Logger();

		// TODO: File Error check
		_logger->_output.open(filename,std::ofstream::out);
		_logger->_logLevel = logLevel;
	};

	static void setLogLevel( unsigned short int logLevel) {
		_logger->_logLevel = logLevel;
	}

	/*
	 * Loggea mensaje si y solo si el logLevel es mayor al setteado en la inicializacion
	 * */
	static void log(const std::string& msg, unsigned short int logLevel);

};

#endif
