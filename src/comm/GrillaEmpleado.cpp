/*
 * GrillaEmpleado.cpp
 *
 *  Created on: May 7, 2014
 *      Author: ferno
 */

#include "comm/GrillaEmpleado.h"

GrillaEmpleado::GrillaEmpleado(unsigned int cantEmpleados) : Grilla(cantEmpleados),_fifo(fifoJefeEmpleados) {
	_fifo.abrir();
}

GrillaEmpleado::~GrillaEmpleado() {
	_fifo.cerrar();
	_fifo.eliminar();
}

unsigned int GrillaEmpleado::esperarTrabajo(unsigned int idEmpleado) {
	unsigned int idAuto;
	_fifo.leer(&idAuto, sizeof(unsigned int));
	return idAuto;
}

void GrillaEmpleado::avisarTrabajoTerminado(unsigned int idEmpleado) {
	// El empleado terminó el trabajo, por lo que debe escribir un 0 en el array
	_sems[idEmpleado].p();
	_mem.escribir(0,idEmpleado);
	_sems[idEmpleado].v();
}

