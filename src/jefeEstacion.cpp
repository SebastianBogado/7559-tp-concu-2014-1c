#include "common.h"
#include "logger/Logger.h"

int main(int argc, char* argv[]) {

	// Init Logger
	Logger::initialize(logFile,Logger::LOG_DEBUG);

	Logger::log("Hola, soy jefeEstacion.cpp", Logger::LOG_DEBUG);

}
