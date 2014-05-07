/*
 * GrillaJefe.h
 *
 *  Created on: May 7, 2014
 *      Author: ferno
 *
 *  Esta clase implementa un FIFO Escritura, para que el jefe le pase los autos a los empleados
 */

#ifndef GRILLAJEFE_H_
#define GRILLAJEFE_H_

#include "Grilla.h"
#include "FifoEscritura.h"
#include "common.h"

class GrillaJefe : public Grilla {
private:
	FifoEscritura _fifo;

	void inicializarGrilla(unsigned int cantEmpleados);

public:
	GrillaJefe(unsigned int cantEmpleados);
	virtual ~GrillaJefe();

	// Devuelve el ID de algun emplado libre. Si no hay ninguno libre, devuelve -1
	unsigned int getEmpleadoLibre() const;

	// Metodo usado por el jefe para marcar la LookUpTable como empleado asignado
	void asignarTrabajo(unsigned int idAuto, unsigned int idEmpleado);
};

#endif /* GRILLAJEFE_H_ */
