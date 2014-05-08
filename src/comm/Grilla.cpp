/*
 * GrillaEmpleado.cpp
 *
 *  Created on: May 5, 2014
 *      Author: ferno
 */

#include "comm/Grilla.h"

Grilla::Grilla(unsigned int cantEmpleados) : _mem(shmemGrilla,'g',cantEmpleados) {
	for(unsigned int i = 0; i < cantEmpleados; i++) {
		// El semaforo esta disponible inicialmente para usarlo
		std::string filename("/tmp/Empleado" + i);
		Semaforo tmpSem(filename,1);
		_sems[i] = tmpSem;
		filename.clear();
	}
}

Grilla::~Grilla() {
	_mem.liberar();
	for(unsigned int i = 0; i < _sems.size(); i++) {
		_sems[i].eliminar();
	}
}