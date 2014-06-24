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
		_semAdmin.crear(semAdminCaja,1);
	} catch(std::string& msg) {
		Logger::error(msg, me);
		exit(3);
	}

	try {
		_semEmp.crear(semEmpCaja,1);
	} catch(std::string& msg) {
		Logger::error(msg, me);
		exit(4);
	}

	Logger::debug("Semaforos creados", me);
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

	Logger::debug("Memoria compartida creada. Se crearán los semaforos para sync de admin y employees", me);
	std::ofstream archSemAdmin(semAdminCaja.c_str());
	if (archSemAdmin.fail() || archSemAdmin.bad()) {
		std::string _msg = std::string("Error creando archivo para semaforo del admin de la caja registradora. Error: ") + std::string(strerror(errno));
		Logger::error(_msg, me);
		throw _msg;
	}
	std::ofstream archSemEmp(semEmpCaja.c_str());
	if (archSemEmp.fail() || archSemEmp.bad()) {
		std::string _msg = std::string("Error creando archivo para semaforo de la caja registradora. Error: ") + std::string(strerror(errno));
		Logger::error(_msg, me);
		throw _msg;
	}

	try {
		_semAdmin.crear(semAdminCaja, 1);
	} catch(std::string& msg) {
		std::string _msg = std::string("Error creando semaforo del admin para la caja registradora");
		Logger::error(_msg, me);
		throw _msg;
	}

	try {
		_semEmp.crear(semEmpCaja, 1);
	} catch(std::string& msg) {
		std::string _msg = std::string("Error creando semaforo de empleados para la caja registradora");
		Logger::error(_msg, me);
		throw _msg;
	}

	Logger::debug("Semaforos creado", me);
	arch.close();
	archSemAdmin.close();
	archSemEmp.close();
	Logger::notice("Se ha inicializado la caja registradora correctamente", me);

}

void CajaRegistradora::destruirCaja() {
	_caja.liberar();
	remove(shmemCaja.c_str());
	remove(semAdminCaja.c_str());
	remove(semEmpCaja.c_str());
	_semAdmin.eliminar();
	_semEmp.eliminar();
}

double CajaRegistradora::consultarMonto() const {
	double monto;
	_semAdmin.p();
	monto = _caja.leer();
	_semAdmin.v();
	return monto;
}

void CajaRegistradora::depositar(const double monto) {
	std::string me = this->me + ":depositar";
	_semEmp.p();
	_semAdmin.p();
	double montoActual = _caja.leer();
	_caja.escribir(montoActual + monto);
	montoActual = _caja.leer();
	Logger::notice(std::string("La caja ahora contiene ") + toString(montoActual), me);
	_semAdmin.v();
	_semEmp.v();
}
