/*
 * CajaRegistradora.h
 *
 *  Created on: May 3, 2014
 *      Author: ferno
 */

#ifndef CAJAREGISTRADORA_H_
#define CAJAREGISTRADORA_H_

#include "comm/MemoriaCompartida2.h"
#include "sync/Semaforo.h"
#include <vector>
#include <cstdlib>
#include <cstdio>
#include "logger/Logger.h"
#include "common.h"

class CajaRegistradora {
private:
	MemoriaCompartida2<double> _caja;
	Semaforo* _sem;
public:
	CajaRegistradora();
	virtual ~CajaRegistradora();

	static void destruirCaja();
	double consultarMonto() const;
	void depositar(const double monto);
};

#endif /* CAJAREGISTRADORA_H_ */
