#include "common.h"
#include "logger/Logger.h"

int main(int argc, char* argv[]) {

	// Init Logger
	Logger::initialize(logFile,Logger::LOG_DEBUG);

	Logger::debug("Hola, soy jefeEstacion.cpp", "jefeEstacion.cpp:main");

}
