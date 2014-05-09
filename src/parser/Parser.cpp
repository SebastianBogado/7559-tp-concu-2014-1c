#include "parser/Parser.h"

#include <iostream>
#include <cassert>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

Parser::Parser() : _debug(default_debug), _ready(true), _surtidores(default_surtidores), _empleados(default_empleados) {
}

void Parser::printUsage() const {
	std::cout << "Uso: ./estacion [-d] [-e] <empleados> [-s] <surtidores>\n";
	std::cout << "\t-d: debug mode\n";
	std::cout << "\t-e: cantidad de empleados\n";
	std::cout << "\t-s: cantidad de surtidores\n";
}

bool Parser::parse(int argc, char* argv[]) {
	int option;
	while ((option = getopt(argc, argv, "de:s:")) != -1) {
		switch (option) {
			case 'd':
				_debug = true;
				break;
			case 'e':
				_empleados = atoi(optarg); 
				break;
			case 's':
				_surtidores = atoi(optarg);
				break;
			case '?':

				_ready = false;
				return false;
			default:
				_ready = false;
				return false;
		}
	}
	_ready = true;
	return true;
}

int Parser::cantSurtidores() const {
	assert(_ready);
	return _surtidores;
}

int Parser::cantEmpleados() const {
	assert(_ready);
	return _empleados;
}

bool Parser::debugMode() const {
	assert(_ready);
	return _debug;
}