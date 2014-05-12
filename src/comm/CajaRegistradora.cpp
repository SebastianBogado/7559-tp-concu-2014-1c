/*
 * CajaRegistradora.cpp
 *
 *  Created on: May 4, 2014
 *      Author: ferno
 */

#include "comm/CajaRegistradora.h"

CajaRegistradora::CajaRegistradora() : me(__FILE__) {}

CajaRegistradora::~CajaRegistradora() {}

void CajaRegistradora::crearCaja() {
	std::string me = this->me + ":crearCaja";
	// La idea es que todos los que instancien de esa clase tengan la misma shmem y el mismo sem
	Logger::debug("Comienza obtención de memoria compartida para la caja registradora", me);

	try {
		_caja.crear(shmemCaja,'c');
	} catch(std::string& msg) {
		std::string _msg = std::string("Error obteniendo memoria para la caja registradora");
		Logger::error(_msg, me);
	}

	try {
		_sem.crear(semCaja,1);
	} catch(std::string& msg) {
		Logger::error(msg, me);
		exit(3);
	}
	Logger::debug("Semaforo creado", me);
	Logger::notice("Se ha inicializado la caja registradora correctamente", me);

}

void CajaRegistradora::inicializarCaja() {
	std::string me = this->me + ":inicializarCaja";
	// La idea es que todos los que instancien de esa clase tengan la misma shmem y el mismo sem
	Logger::debug("Comienza creación de memoria compartida para la caja registradora", me);

	std::ofstream arch(shmemCaja.c_str());
	if (arch.fail() || arch.bad()) {
		std::string _msg = std::string("Error creando archivo para la memoria de la caja registradora: ") + std::string(strerror(errno));
		Logger::error(_msg, me);
		throw _msg;
	}

	Logger::debug("Creado archivo que representa la memoria compartida...", me);
	try {
		_caja.crear(shmemCaja,'c');
	} catch(std::string& msg) {
		std::string _msg = std::string("Error creando la memoria para la caja registradora");
		Logger::error(_msg, me);
		arch.close();
		remove(shmemCaja.c_str());
		throw _msg;
	}

	Logger::debug("Memoria compartida creada. Se creará el semáforo para sync de usuarios", me);
	std::ofstream archSem(semCaja.c_str());
	if (archSem.fail() || archSem.bad()) {
		std::string _msg = std::string("Error creando archivo para semaforo de la caja registradora. Error: ") + std::string(strerror(errno));
		Logger::error(_msg, me);
		throw _msg;
	}

	try {
		_sem.crear(semCaja, 1);
	} catch(std::string& msg) {
		std::string _msg = std::string("Error creando semaforo para la caja registradora");
		Logger::error(_msg, me);
		throw _msg;
	}
	Logger::debug("Semaforo creado", me);
	arch.close();
	archSem.close();
	Logger::notice("Se ha inicializado la caja registradora correctamente", me);

}

void CajaRegistradora::destruirCaja() {
	remove(shmemCaja.c_str());
	remove(semCaja.c_str());
}

double CajaRegistradora::consultarMonto() const {
	double monto;
	_sem.p();
	// TODO: Ver cuanto sleepea en la caja
	sleep(3);
	monto = _caja.leer();
	_sem.v();
	return monto;
}

void CajaRegistradora::depositar(const double monto) {
	_sem.p();
	double montoActual = _caja.leer();
	_caja.escribir(montoActual + monto);
	montoActual = _caja.leer();
	Logger::notice(std::string("La caja ahora contiene ") + toString(montoActual));
	_sem.v();
}
