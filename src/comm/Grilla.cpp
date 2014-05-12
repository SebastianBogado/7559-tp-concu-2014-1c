/*
 * GrillaEmpleado.cpp
 *
 *  Created on: May 5, 2014
 *      Author: ferno
 */

#include "comm/Grilla.h"

Grilla::Grilla(unsigned int cantEmpleados) {

	std::ofstream arch(shmemGrilla.c_str());
	if (arch.fail() || arch.bad()) {
		Logger::error("Error creando archivo para la grilla de empleados");
		exit(1);
	}
	_mem.crear(shmemGrilla,'g',cantEmpleados);
	arch.close();
	Logger::debug("Memoria compartida para la grilla de empleados creada");

	for(unsigned int i = 0; i < cantEmpleados; i++) {
		// El semaforo esta disponible inicialmente para usarlo
		std::string filename("/tmp/Empleado" + toString(i));
		std::ofstream archSem(filename.c_str());
		Semaforo tmpSem(filename,1);
		_sems.push_back(tmpSem);
		filename.clear();
		archSem.close();
	}
}

Grilla::~Grilla() {
	_mem.liberar();
	for(unsigned int i = 0; i < _sems.size(); i++) {
		_sems[i].eliminar();
	}
}

void Grilla::destruir() {
	remove(shmemGrilla.c_str());
	for(unsigned int i = 0; i < _sems.size(); i++) {
		std::string filename("/tmp/Empleado" + toString(i));
		remove(filename.c_str());
	}
}
