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
#include "logger/Logger.h"
#include <vector>

class GrillaJefe : public Grilla {
private:
	std::vector<FifoEscritura> _fifo;
	unsigned int _cantEmpleados;
	unsigned int _lastEmpleado; // Guarda el # de empleado del ultimo getEmpleadoLibre

	void inicializarGrillaJefeEmpleados(unsigned int cantEmpleados);

public:
	GrillaJefe(unsigned int cantEmpleados);
	virtual ~GrillaJefe();

	void crearGrillaJefe();
	// Se crean los FIFOs correspondientes a los empleados
	void inicializarGrillaJefe();
	static void destruirGrillaJefe(unsigned int cantEmpleados);

	// Devuelve el ID de algun emplado libre. Si no hay ninguno libre, devuelve -1
	// Se recorre linealmente en busqueda de un empleado libre
	// Para hacer la asignacion mas fair, se mantiene un puntero interno al ultimo empleado, desde donde se comienza a buscar cada vez hasta recorrer todos los empleados
	unsigned int getEmpleadoLibre();

	// Metodo usado por el jefe para marcar la LookUpTable como empleado asignado
	void asignarTrabajo(int idAuto, unsigned int idEmpleado);

	// Le avisa a todos los empleados que el trabajo termino y se pueden ir a casa :D
	void avisarTerminarTrabajo(unsigned int cantEmpleados);
};

#endif /* GRILLAJEFE_H_ */
