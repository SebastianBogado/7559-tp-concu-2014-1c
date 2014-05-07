/*
 * Grilla.h
 *
 *  Created on: May 5, 2014
 *      Author: ferno
 *
 *  Basicamente es un array compartido donde el jefe le asigna un auto (ID) a un empleado (elemento del array)
 *  y este ultimo lo lee y atiende al cliente. A su vez, el empleado avisa al jefe que esta disponible escribiendo un
 *  cero (0) en el elemento del array correspondiente
 */

#ifndef GRILLA_H_
#define GRILLA_H_

#include <vector>
#include "comm/ArrayCompartido.h"
#include "sync/Semaforo.h"
#include "logger/Logger.h"

class Grilla {
protected:
	ArrayCompartido<unsigned int> _mem;
	std::vector<Semaforo> _sems;

public:
	Grilla(unsigned int cantEmpleados);
	virtual ~Grilla();
};

#endif /* GRILLA_H_ */
