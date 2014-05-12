/*
 * GrillaJefe.cpp
 *
 *  Created on: May 7, 2014
 *      Author: ferno
 */

#include "comm/GrillaJefe.h"

GrillaJefe::GrillaJefe(unsigned int cantEmpleados) : Grilla(cantEmpleados),_fifo(fifoJefeEmpleados) {
	_fifo.abrir();
	inicializarGrilla(cantEmpleados);
}

GrillaJefe::~GrillaJefe() {
	_fifo.cerrar();
	_fifo.eliminar();
}

void GrillaJefe::inicializarGrilla(unsigned int cantEmpleados) {
	for(unsigned int i = 0; i < cantEmpleados; i++) {
		// Siempre se comienza con todos los empleados disponibles
		_sems[i].p();
		_mem.escribir(0,i);
		_sems[i].v();
	}
}

unsigned int GrillaJefe::getEmpleadoLibre() const {
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

void GrillaJefe::asignarTrabajo(unsigned int idAuto, unsigned int idEmpleado) {
	// Encontro un empleado libre, entonces marca la shared lookup table y le escribe el ID de auto en el pipe
	_sems[idEmpleado].p();
	_mem.escribir(1, idEmpleado);
	_sems[idEmpleado].v();

	_fifo.escribir(&idAuto, sizeof(unsigned int));
}

void GrillaJefe::avisarTerminarTrabajo(unsigned int cantEmpleados) {
	for(unsigned int i = 0; i < cantEmpleados; i++) {
		// Siempre se comienza con todos los empleados disponibles
		_sems[i].p();
		_mem.escribir(-1,i);
		_sems[i].v();
	}
}


