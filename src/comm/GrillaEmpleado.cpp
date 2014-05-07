/*
 * GrillaEmpleado.cpp
 *
 *  Created on: May 5, 2014
 *      Author: ferno
 */

#include "comm/GrillaEmpleado.h"

GrillaEmpleado::GrillaEmpleado(unsigned int cantEmpleados) {
	std::string filename("/tmp/grillaEmpleados");
	_mem.crear(filename,'g',cantEmpleados);
	_pipe = new Pipe();

	for(unsigned int i = 0; i < cantEmpleados; i++) {
		// El semaforo esta disponible inicialmente para usarlo
		std::string filename("/tmp/Empleado" + i);
		Semaforo tmpSem(filename,1);
		_sems[i] = tmpSem;
		filename.clear();
	}
}

GrillaEmpleado::~GrillaEmpleado() {}

void GrillaEmpleado::inicializarGrilla(unsigned int cantEmpleados) {
	for(unsigned int i = 0; i < cantEmpleados; i++) {
		// Siempre se comienza con todos los empleados disponibles
		_mem.escribir(0,cantEmpleados);
		_pipe->setearModo(Pipe::ESCRITURA);
	}
}

unsigned int GrillaEmpleado::getEmpleadoLibre() const {
	for(unsigned int i = 0; i < _mem.size(); i++) {
		_sems[i].p();
		unsigned int ocupado = _mem.leer(i);
		_sems[i].v();
		// 0 == Libre, 1 == Ocupado
		if (!ocupado)	return i;
	}
	// No encontro ningun empleado libre
	return -1;
}

void GrillaEmpleado::asignarTrabajo(unsigned int idAuto, unsigned int idEmpleado) {
	// Encontro un empleado libre, entonces marca la shared lookup table y le escribe el ID de auto en el pipe
	_sems[idEmpleado].p();
	_mem.escribir(1, idEmpleado);
	_sems[idEmpleado].v();

	_pipe->escribir(&idAuto, sizeof(unsigned int));
}

unsigned int GrillaEmpleado::esperarTrabajo(unsigned int idEmpleado) {
	unsigned int idAuto;
	_pipe->setearModo(Pipe::LECTURA);
	_pipe->leer(&idAuto, sizeof(unsigned int));
	return idAuto;
}

void GrillaEmpleado::avisarTrabajoTerminado(unsigned int idEmpleado) {
	// El empleado terminó el trabajo, por lo que debe escribir un 0 en el array
	_sems[idEmpleado].p();
	_mem.escribir(0,idEmpleado);
	_sems[idEmpleado].v();
}