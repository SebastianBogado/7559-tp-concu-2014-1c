/*
 * CajaRegistradora.cpp
 *
 *  Created on: May 4, 2014
 *      Author: ferno
 */

#include "CajaRegistradora.h"

CajaRegistradora::CajaRegistradora() {
	// La idea es que todos los que instancien de esa clase tengan la misma shmem y el mismo sem
	std::string file("/tmp/shmemCaja");
	_caja.crear(file, 'c');
	_sem = new Semaforo(file, 1);
}

CajaRegistradora::~CajaRegistradora() {
	_caja.liberar();
	delete _sem;
}

double CajaRegistradora::cosultarMonto() const {
	double monto;
	_sem->p();
	monto = _caja.leer();
	_sem->v();
	return monto;
}

void CajaRegistradora::depositar(const double monto) {
	_sem->p();
	_caja.escribir(monto);
	_sem->v();
}
