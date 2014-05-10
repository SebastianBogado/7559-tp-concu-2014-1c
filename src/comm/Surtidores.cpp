/*
 * Surtidores.cpp
 *
 *  Created on: May 7, 2014
 *      Author: ferno
 */

#include "comm/Surtidores.h"

Surtidores::Surtidores(unsigned int cantSurtidores) :
		_surtidores(shmemSurtidores,'s',cantSurtidores),
		_surtidoresDisponibles(semSurtidoresDisponibles,cantSurtidores) {

	for(unsigned int i = 0; i < cantSurtidores; i++) {
		// El semaforo esta disponible inicialmente para usarlo
		std::string filename("/tmp/Surtidor" + i);
		std::ofstream arch(filename.c_str());
		Semaforo tmpSem(filename,1);
		_sems[i] = tmpSem;
		filename.clear();
		arch.close();
	}

	inicializarSurtidores();

}

void Surtidores::inicializarSurtidores() {
	for(unsigned int i = 0; i < _sems.size(); i++) {
		// Siempre se comienza con todos los empleados disponibles
		_sems[i].p();
		_surtidores.escribir(0,i);
		_sems[i].v();
	}
}

void Surtidores::destruirSurtidores() {
	remove(shmemSurtidores.c_str());
	remove(semSurtidoresDisponibles.c_str());
	/*for(unsigned int i = 0; i < _sems.size(); i++) {
		// El semaforo esta disponible inicialmente para usarlo
		remove("/tmp/Surtidor" + i);
	}*/
}

Surtidores::~Surtidores() {
	_surtidores.liberar();

	for (unsigned int i = 0; i < _sems.size(); i++)
		_sems[i].eliminar();

	_surtidoresDisponibles.eliminar();
}

unsigned int Surtidores::conseguirSurtidorLibre(unsigned int idEmpleado) {
	_surtidoresDisponibles.p();
	int idSurtidorAsignado = -1;
	for(unsigned int i = 0; (i < _sems.size()) && (idSurtidorAsignado == -1); i++) {
		_sems[i].p();
		unsigned int ocupado = _surtidores.leer(i);
		if(!ocupado) {
			_surtidores.escribir(idEmpleado,i);
			idSurtidorAsignado = i;
		}
		_sems[i].v();
	}
	return idSurtidorAsignado;
}

void Surtidores::liberarSurtidor(unsigned int idSurtidor) {
	_sems[idSurtidor].p();
	_surtidores.escribir(0,idSurtidor);
	_sems[idSurtidor].v();

	_surtidoresDisponibles.v();
}
