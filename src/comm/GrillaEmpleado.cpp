/*
 * GrillaEmpleado.cpp
 *
 *  Created on: May 7, 2014
 *      Author: ferno
 */

#include "comm/GrillaEmpleado.h"

GrillaEmpleado::GrillaEmpleado(unsigned int cantEmpleados, unsigned int idEmpleado) : Grilla(cantEmpleados) {
	me = __FILE__;
	std::string filename(fifoJefeEmpleados + toString(idEmpleado));
	// TODO: Error correction! Mover a un metodo "crear"
	_fifo = new FifoLectura(filename);
	_fifo->abrir();
}

GrillaEmpleado::~GrillaEmpleado() {
	_fifo->cerrar();
	_fifo->eliminar();
	delete _fifo;
}

int GrillaEmpleado::esperarTrabajo(unsigned int idEmpleado) {
	int idAuto;
	_fifo->leer(&idAuto, sizeof(int));
	return idAuto;
}

void GrillaEmpleado::avisarTrabajoTerminado(unsigned int idEmpleado) {
	// El empleado terminó el trabajo, por lo que debe escribir un 0 en el array
	_sems[idEmpleado].p();
	_mem.escribir(0,idEmpleado);
	_sems[idEmpleado].v();
}

