/*
 * GrillaEmpleado.cpp
 *
 *  Created on: May 5, 2014
 *      Author: ferno
 */

#include "GrillaEmpleado.h"

GrillaEmpleado::GrillaEmpleado(unsigned int cantEmpleados) {
	std::string filename("/tmp/grillaEmpleados");
	_mem.crear(filename,'g',cantEmpleados);
	for(unsigned int i = 0; i < cantEmpleados; i++) {
		// El semaforo esta disponible inicialmente para usarlo
		std::string filename("/tmp/Empleado" + i);
		_sems[i] = Semaforo(filename, 1);
		filename.clear();
	}
}

void GrillaEmpleado::inicializarGrilla(unsigned int cantEmpleados) {
	for(unsigned int i = 0; i < cantEmpleados; i++) {
		// Siempre se comienza con todos los empleados disponibles
		_mem.escribir(0,cantEmpleados);
	}
}

unsigned int GrillaEmpleado::getEmpleadoLibre() const {
	for(unsigned int i = 0; i < _mem.size(); i++) {
		_sems[i].p();
		unsigned int ocupado = _mem.leer(i);
		_sems[i].v();
		// 0 == Libre, != 0 --> Ocupado
		if (!ocupado)	return i;
	}
	// No encontro ningun empleado libre
	return -1;
}

void GrillaEmpleado::asignarTrabajo(unsigned int idAuto, unsigned int idEmpleado) {
	// Encontro un empleado libre, entonces le escribe el ID del auto en el array
	_sems[idEmpleado].p();
	_mem.escribir(idAuto, idEmpleado);
	_sems[idEmpleado].v();
}

void GrillaEmpleado::avisarTrabajoTerminado(unsigned int idEmpleado) {
	// El empleado terminó el trabajo, por lo que debe escribir un 0 en el array
	_sems[idEmpleado].p();
	_mem.escribir(0,idEmpleado);
	_sems[idEmpleado].v();
}
