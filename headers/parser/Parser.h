#pragma once

class Parser {

	static const int default_surtidores = 2;
	static const int default_empleados = 3;
	static const bool default_debug = false;

	bool _debug;
	bool _ready;
	int _surtidores;
	int _empleados;
	
	public:
	
	Parser();
	
	// Devuelve true en caso de exito
	// False en caso de error
	bool parse(int argc, char* argv[]);
	// Devuelve la cant de surtidores
	int cantSurtidores() const;
	// Devuelve la cant de empleados
	int cantEmpleados() const;
	// Devuelve true si se ejecuta en modo verbose/debug
	bool debugMode() const;
	// imprime el uso correcto de los args
	void printUsage() const;	
};
