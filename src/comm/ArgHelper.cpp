#include "comm/ArgHelper.h"
#include <cassert>
#include <cstdlib>
#include <sstream>
#include <cstring>

// Encoder stuff
void ArgHelper::encode(const std::string& program, bool debug, int empleados, int surtidores, int id) {
	this->clear(); // just in case
	_argv.resize(6);
	std::stringstream ss;
	// argv[0] = program name convention
	_argv[0] = strdup(program.c_str());
	// Debug
	if(debug) {
		ss << "1";
	}else{
		ss << "0";
	}
	_argv[1] = strdup(ss.str().c_str());
	// cant empleados
	ss.str("");
	ss << empleados;
	_argv[2] = strdup(ss.str().c_str());
	// cant surtidores
	ss.str("");
	ss << surtidores;
	_argv[3] = strdup(ss.str().c_str());
	// id del proceso
	ss.str("");
	ss << id;
	_argv[4] = strdup(ss.str().c_str());
	// array end
	_argv[5] = NULL;
}

char** ArgHelper::getArgv() {
	return &_argv[0];
}

ArgHelper::~ArgHelper() {
	this->clear();
}

void ArgHelper::clear() {
	// Only really used if you encoded previously
	if(_argv.size() > 0) {
		for(size_t i = 0;i < _argv.size();i++) {
			// avoid the last NULL element
			if(_argv[i] != NULL) free(_argv[i]);
		}
	}
}

// Decoder stuff
bool ArgHelper::decode(int argc, char* argv[], bool* debug, int* empleados, int* surtidores, int* id) {
	if(argc != 5) return false;
	// Debug
	if(strcmp(argv[1], "1") == 0) {
		*debug = true;
	}else if(strcmp(argv[1], "0") == 0) {
		*debug = false;
	}else{
		return false;
	}
	// Empleados
	*empleados = atoi(argv[2]);
	// Surtidores
	*surtidores = atoi(argv[3]);
	// Id
	*id = atoi(argv[4]);
	return true;
}