/*
 * Surtidores.cpp
 *
 *  Created on: May 7, 2014
 *      Author: ferno
 */

#include "comm/Surtidores.h"

Surtidores::Surtidores(unsigned int cantSurtidores) {

	std::ofstream arch(shmemSurtidores.c_str());
	if (arch.fail() || arch.bad()) {
		Logger::error("Error creando archivo para la caja registradora");
		exit(1);
	}
	_surtidores.crear(shmemSurtidores,'s',cantSurtidores);
	arch.close();
	Logger::debug("Creada la shared memory para los surtidores");

	std::ofstream archSem(semSurtidoresDisponibles.c_str());
	if (archSem.fail() || archSem.bad()) {
		Logger::error("Error creando archivo para la caja registradora");
		exit(1);
	}
	_surtidoresDisponibles.crear(semSurtidoresDisponibles,cantSurtidores);
	archSem.close();
	Logger::debug("Creado el semaforo general para la entrada de los empleados...");

	for(unsigned int i = 0; i < cantSurtidores; i++) {
		// El semaforo esta disponible inicialmente para usarlo
		std::string filename("/tmp/Surtidor" + toString(i));
		std::ofstream arch(filename.c_str());
		Semaforo tmpSem(filename,1);
		_sems.push_back(tmpSem);
		filename.clear();
		arch.close();
	}
	Logger::debug("Creados los semáforos individuales para cada surtidor...");

	inicializarSurtidores();
	Logger::debug("Inicializados los surtidores...");

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
	for(unsigned int i = 0; i < _sems.size(); i++) {
		std::string filename("/tmp/Surtidor" + toString(i));
		remove(filename.c_str());
	}
}

Surtidores::~Surtidores() {
	//_surtidores.liberar();

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
	_surtidores.liberar();

	_sems[idSurtidor].p();
	_surtidores.escribir(0,idSurtidor);
	_sems[idSurtidor].v();

	_surtidoresDisponibles.v();
}
