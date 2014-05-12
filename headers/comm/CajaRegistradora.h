/*
 * CajaRegistradora.h
 *
 *  Created on: May 3, 2014
 *      Author: ferno
 */

#ifndef CAJAREGISTRADORA_H_
#define CAJAREGISTRADORA_H_

#include "comm/MemoriaCompartida.h"
#include "sync/Semaforo.h"
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cerrno>
#include "logger/Logger.h"
#include "common.h"

class CajaRegistradora {
private:
	MemoriaCompartida<double> _caja;
	Semaforo _sem;
	std::string me;
public:
	CajaRegistradora();
	virtual ~CajaRegistradora();

	// Este metodo lo usan todos los que quieran obtener la caja. Se obtendra la misma shmem para cada instancia
	void crearCaja();

	// Este metodo lo usará solo el que cree por primera vez la caja
	void inicializarCaja();

	// Este metodolo usará solo el proceso que haya creado la caja
	void destruirCaja();

	double consultarMonto() const;
	void depositar(const double monto);
};

#endif /* CAJAREGISTRADORA_H_ */
