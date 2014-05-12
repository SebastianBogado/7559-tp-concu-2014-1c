/*
 * Surtidores.cpp
 *
 *  Created on: May 7, 2014
 *      Author: ferno
 */

#include "comm/Surtidores.h"

Surtidores::Surtidores() : me(__FILE__) {}

void Surtidores::crearSurtidores(unsigned int cantSurtidores) {
	std::string me = this->me + ":"  + __func__;

	try {
		_surtidores.crear(shmemSurtidores,'g',cantSurtidores);
	} catch(std::string& msg) {
		std::string _msg = std::string("Error creando memoria compartida para los surtidores");
		Logger::error(_msg, me);

		remove(shmemSurtidores.c_str());

		throw _msg;
	}
	Logger::debug("Creada la shared memory para los surtidores", me);

	try {
		_surtidoresDisponibles.crear(semSurtidoresDisponibles,cantSurtidores);
	} catch(std::string& msg) {
		std::string _msg("Error en la creacion del semaforo general para todos los surtidores");
		Logger::error(_msg, me);
		remove(semSurtidoresDisponibles.c_str());
		throw _msg;
	}
	Logger::debug("Creado el semaforo general para la entrada de los empleados...", me);

	for(unsigned int i = 0; i < cantSurtidores; i++) {
		// El semaforo esta disponible inicialmente para usarlo
		std::string filename("/tmp/Surtidor" + toString(i));
		Semaforo tmpSem;
		try {
			tmpSem.crear(filename,1);
		} catch(std::string& msg) {
			std::string _msg = std::string("Error creando el semaforo para el surtidor ") + toString(i);
			Logger::error(_msg, me);

			destruirSurtidores(i + 1);

			throw me;
		}
		_sems.push_back(tmpSem);
	}
	Logger::debug("Creados los semáforos individuales para cada surtidor...", me);

	inicializarSurtidoresDesocupados();
	Logger::debug("Inicializados los surtidores...", me);
}

void Surtidores::inicializarSurtidores(unsigned int cantSurtidores) {
	std::string me = this->me + ":"  + __func__;

	std::ofstream arch(shmemSurtidores.c_str());
	if (arch.fail() || arch.bad()) {
		std::string _msg = std::string("Error creando el archivo para la memoria de los surtidores. "
				"Nombre: ") + shmemSurtidores + std::string(". Error: ") + strerror(errno);
		Logger::error(_msg, me);
		throw _msg;
	}

	try {
		_surtidores.crear(shmemSurtidores,'g',cantSurtidores);
	} catch(std::string& msg) {
		std::string _msg = std::string("Error inicializando memoria compartida para los surtidores");
		Logger::error(_msg, me);

		arch.close();
		remove(shmemSurtidores.c_str());

		throw _msg;
	}
	arch.close();
	Logger::debug("Creada la shared memory para los surtidores", me);

	std::ofstream archSem(semSurtidoresDisponibles.c_str());
	if (archSem.fail() || archSem.bad()) {
		std::string _msg = std::string("Error creando el archivo para el semaforo de los surtidores. "
						"Nombre: ") + semSurtidoresDisponibles + std::string(". Error: ") + strerror(errno);
		Logger::error(_msg, me);
		throw _msg;
	}

	try {
		_surtidoresDisponibles.crear(semSurtidoresDisponibles,cantSurtidores);
	} catch(std::string& msg) {
		std::string _msg("Error en la init del semaforo para todos los surtidores");
		Logger::error(_msg, me);
		archSem.close();
		remove(semSurtidoresDisponibles.c_str());
		throw _msg;
	}
	archSem.close();
	Logger::debug("Inicializado el semaforo general para la entrada de los empleados a los surtidores...", me);

	for(unsigned int i = 0; i < cantSurtidores; i++) {
		// El semaforo esta disponible inicialmente para usarlo
		std::string filename("/tmp/Surtidor" + toString(i));
		std::ofstream arch(filename.c_str());
		if (arch.bad() || arch.fail()) {
			std::string _msg = std::string("Error creando archivo para el semaforo del surtidor. Nombre: '") + filename + std::string("'. Error: ") + std::string(strerror(errno));
			Logger::error(_msg, me);

			destruirSurtidores(i);

			throw _msg;
		}
		Semaforo tmpSem;
		try {
			tmpSem.crear(filename,1);
		} catch(std::string& msg) {
			std::string _msg(std::string("Error inicializando semaforo para el surtidor") + toString(i));
			Logger::error(_msg, me);
			arch.close();

			destruirSurtidores(i + 1);

			throw _msg;
		}
		_sems.push_back(tmpSem);
		arch.close();
		Logger::debug(std::string("Se ha inicializado el semaforo para el surtidor ") + toString(i), me);
	}
	Logger::debug("Inicializados los semáforos individuales para cada surtidor...", me);

	inicializarSurtidoresDesocupados();
	Logger::debug("Inicializados los surtidores...", me);
}

void Surtidores::inicializarSurtidoresDesocupados() {
	for(unsigned int i = 0; i < _sems.size(); i++) {
		// Siempre se comienza con todos los empleados disponibles
		_sems[i].p();
		_surtidores.escribir(0,i);
		_sems[i].v();
	}
}

void Surtidores::destruirSurtidores(unsigned int cantSurtidores) {

	remove(shmemSurtidores.c_str());
	remove(semSurtidoresDisponibles.c_str());
	for(unsigned int i = 0; i < cantSurtidores; i++) {
		std::string filename("/tmp/Surtidor" + toString(i));
		remove(filename.c_str());
	}
	for (unsigned int i = 0; i < cantSurtidores; i++)
		_sems[i].eliminar();

	_surtidoresDisponibles.eliminar();
	
}

Surtidores::~Surtidores() {
	_surtidores.liberar();
}

unsigned int Surtidores::conseguirSurtidorLibre(unsigned int idEmpleado) {
	_surtidoresDisponibles.p();
	int idSurtidorAsignado = -1;
	for(unsigned int i = 0; (i < _sems.size()) && (idSurtidorAsignado == -1); i++) {
		_sems[i].p();
		unsigned int ocupado = _surtidores.leer(i);
		Logger::debug("Se ha leido estado " + toString(ocupado) + " en surtidor " + toString(i), me);
		if(!ocupado) {
			_surtidores.escribir(1,i);
			Logger::debug("Se ocupa surtidor " + toString(i) + " ahora con estado " + toString(_surtidores.leer(i)), me);
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
