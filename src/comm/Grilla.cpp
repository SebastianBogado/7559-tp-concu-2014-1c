/*
 * GrillaEmpleado.cpp
 *
 *  Created on: May 5, 2014
 *      Author: ferno
 */

#include "comm/Grilla.h"

Grilla::Grilla(unsigned int cantEmpleados) {
	std::string filename("/tmp/grillaEmpleados");
	_mem.crear(filename,'g',cantEmpleados);

	for(unsigned int i = 0; i < cantEmpleados; i++) {
		// El semaforo esta disponible inicialmente para usarlo
		std::string filename("/tmp/Empleado" + i);
		Semaforo tmpSem(filename,1);
		_sems[i] = tmpSem;
		filename.clear();
	}
}

Grilla::~Grilla() {}
