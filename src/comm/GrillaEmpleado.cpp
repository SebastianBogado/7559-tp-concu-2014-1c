/*
 * GrillaEmpleado.cpp
 *
 *  Created on: May 7, 2014
 *      Author: ferno
 */

#include "comm/GrillaEmpleado.h"

GrillaEmpleado::GrillaEmpleado() : Grilla(), _fifo(NULL) {
	me = __FILE__;
}

void GrillaEmpleado::crearGrillaEmpleado(unsigned int cantEmpleados, unsigned int idEmpleado) {
	std::string _me = this->me + std::string(":crearGrillaEmpleado");

	try {
		this->crearGrilla(cantEmpleados);
	} catch(std::string& msg) {
		std::string _msg = std::string("Error creando grilla general para el empleado ") + toString(idEmpleado);
		Logger::error(_msg, me);
		throw _msg;
	}

	std::string filename(fifoJefeEmpleados + toString(idEmpleado));
	_fifo = new FifoLectura(filename);
	try {
		_fifo->abrir();
	} catch(std::string& msg) {
		std::string _msg = std::string("Error abriendo el fifo: ") + filename;
		Logger::error(_msg, me);
		throw _msg;
	}
}

GrillaEmpleado::~GrillaEmpleado() {
	_fifo->cerrar();
	//_fifo->eliminar();
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

