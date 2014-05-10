/*
 * CajaRegistradora.cpp
 *
 *  Created on: May 4, 2014
 *      Author: ferno
 */

#include "comm/CajaRegistradora.h"

CajaRegistradora::CajaRegistradora() {
	// La idea es que todos los que instancien de esa clase tengan la misma shmem y el mismo sem
	Logger::debug("A punto de crear la memoria compartida para la caja registradora");

	std::ofstream arch(shmemCaja.c_str());
	if (arch.fail() || arch.bad()) {
		Logger::error("Error creando archivo para la caja registradora");
		exit(1);
	}

	Logger::debug("Creado archivo que representara la memoria compartida");
	try {
		_caja.crear(shmemCaja,'c');
	} catch(std::string& msg) {
		Logger::error(msg);
		arch.close();
		exit(2);
	}

	Logger::debug("Memoria compartida creada. Se creará el semáforo para sync de usuarios");
	std::ofstream archSem(semCaja.c_str());
	if (archSem.fail() || archSem.bad()) {
		Logger::error("Error creando archivo para la caja registradora");
		exit(3);
	}
	_sem = new Semaforo(semCaja, 1);
	Logger::debug("Semaforo creado");
	arch.close();
	archSem.close();
	Logger::notice("Se ha inicializado la caja registradora correctamente");
}

CajaRegistradora::~CajaRegistradora() {
	_caja.liberar();
	delete _sem;
}

void CajaRegistradora::destruirCaja() {
	remove(shmemCaja.c_str());
	remove(semCaja.c_str());
}

double CajaRegistradora::consultarMonto() const {
	double monto;
	_sem->p();
	sleep(20);
	monto = _caja.leer();
	_sem->v();
	return monto;
}

void CajaRegistradora::depositar(const double monto) {
	_sem->p();
	_caja.escribir(monto);
	_sem->v();
}
