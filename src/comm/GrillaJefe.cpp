/*
 * GrillaJefe.cpp
 *
 *  Created on: May 7, 2014
 *      Author: ferno
 */

#include "comm/GrillaJefe.h"

GrillaJefe::GrillaJefe(unsigned int cantEmpleados) : Grilla(), _cantEmpleados(cantEmpleados), _lastEmpleado(0) {
	me = __FILE__;
}

void GrillaJefe::crearGrillaJefe() {
	std::string _me = this->me + ":crearGrillaJefe";

	try {
		this->crearGrilla(_cantEmpleados);
	} catch(std::string& msg) {
		std::string _msg = std::string("Error creando grilla general para el jefe");
		Logger::error(_msg, me);
		throw _msg;
	}

	for (unsigned int i = 0; i < _cantEmpleados; i++) {
		std::string filename(fifoJefeEmpleados + toString(i));
		FifoEscritura tmpFifo(filename);
		try {
			_fifo.push_back(tmpFifo);
			_fifo[i].abrir();
		} catch(std::string& msg) {
			std::string _msg = std::string("Error en la creacion del fifo: ") + filename;
			Logger::error(_msg, _me);

			destruirGrillaJefe(i);

			throw _msg;
		}
	}
	inicializarGrillaJefeEmpleados(_cantEmpleados);
}

void GrillaJefe::inicializarGrillaJefe() {
	std::string _me = this->me + ":inicializarGrillaJefe";

	// Se crean los FIFOs que comunican con los empleados
	for (unsigned int i = 0; i < _cantEmpleados; i++) {
		std::string filename(fifoJefeEmpleados + toString(i));
		FifoEscritura tmpFifo(filename);
		try {
			tmpFifo.crear();
		} catch(std::string& msg) {
			std::string _msg = std::string("Error en la inicializacion del fifo: ") + filename;
			Logger::error(_msg, _me);

			destruirGrillaJefe(i);

			throw _msg;
		}
	}
}

void GrillaJefe::destruirGrillaJefe(unsigned int cantEmpleados) {
	for (unsigned int i = 0; i < cantEmpleados; i++) {
		std::string filename(fifoJefeEmpleados + toString(i));
		FifoEscritura tmpFifo(filename);
		tmpFifo.eliminar();
	}
}

GrillaJefe::~GrillaJefe() {
	//for (unsigned int i = 0; i < _cantEmpleados; i++) {
		//_fifo[i].cerrar();
		//_fifo[i].eliminar();
	//}
}

void GrillaJefe::inicializarGrillaJefeEmpleados(unsigned int cantEmpleados) {
	for(unsigned int i = 0; i < cantEmpleados; i++) {
		// Siempre se comienza con todos los empleados disponibles
		_sems[i].p();
		_mem.escribir(0,i);
		_sems[i].v();
	}
}

unsigned int GrillaJefe::getEmpleadoLibre() {
	std::string me = this->me + ":getEmpleadoLibre";
	unsigned int current = _lastEmpleado;
	for(unsigned int i = 0; i < _cantEmpleados; i++) {
		_sems[current].p();
		unsigned int ocupado = _mem.leer(current);
		Logger::debug("Se ha leido el estado " + toString(ocupado) + " del empleado " + toString(current), me);
		_sems[current].v();
		// 0 == Libre, 1 == Ocupado
		if (!ocupado)	{
			_lastEmpleado = current;
			return current;
		}else{
			// modulo _cantEmpleados increase
			current++;
			if(current == _cantEmpleados) current = 0;
		}
	}
	// No encontro ningun empleado libre
	return -1;
}

void GrillaJefe::asignarTrabajo(int idAuto, unsigned int idEmpleado) {
	// Encontro un empleado libre, entonces marca la shared lookup table y le escribe el ID de auto en el pipe
	_sems[idEmpleado].p();
	_mem.escribir(1, idEmpleado);
	_sems[idEmpleado].v();

	_fifo[idEmpleado].escribir(&idAuto, sizeof(int));
}

void GrillaJefe::avisarTerminarTrabajo(unsigned int cantEmpleados) {
	int idTrabajoTerminado = -1;
	for(unsigned int i = 0; i < cantEmpleados; i++) {
		_fifo[i].escribir(&idTrabajoTerminado, sizeof(int));
	}
}


