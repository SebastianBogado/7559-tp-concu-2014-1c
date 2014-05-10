#pragma once

#include <vector>
#include <string>

// Clase para simplificar el pasaje de argumentos entre procesos durante execv

class ArgHelper {
	std::vector<char*> _argv;
	void clear();
	
	public:
	~ArgHelper();
	void encode(const std::string& program, bool debug, int empleados, int surtidores, int id);
	char** getArgv();
	
	static bool decode(int argc, char* argv[], bool* debug, int* empleados, int* surtidores, int* id);
};