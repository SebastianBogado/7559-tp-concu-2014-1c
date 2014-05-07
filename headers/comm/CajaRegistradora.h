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

class CajaRegistradora {
private:
	MemoriaCompartida2<double> _caja;
	Semaforo* _sem;
public:
	CajaRegistradora();
	virtual ~CajaRegistradora();

	double cosultarMonto() const;
	void depositar(const double monto);
};

#endif /* CAJAREGISTRADORA_H_ */