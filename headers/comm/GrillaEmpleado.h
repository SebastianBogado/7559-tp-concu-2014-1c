/*
 * GrillaEmpleado.h
 *
 *  Created on: May 7, 2014
 *      Author: ferno
 */

#ifndef GRILLAEMPLEADO_H_
#define GRILLAEMPLEADO_H_

#include "Grilla.h"
#include "FifoLectura.h"
#include "common.h"
#include "logger/Logger.h"
#include <vector>

class GrillaEmpleado : public Grilla {
private:
	FifoLectura* _fifo;

public:
	GrillaEmpleado(unsigned int cantEmpleados, unsigned int idEmpleado);
	virtual ~GrillaEmpleado();

	// Metodo usado por el empleado para esperar su siguiente trabajo
	int esperarTrabajo(unsigned int idEmpleado);

	// Metodo usado por un empleado que se libera de su trabajo para avisar al jefe
	void avisarTrabajoTerminado(unsigned int idEmpleado);
};




#endif /* GRILLAEMPLEADO_H_ */
