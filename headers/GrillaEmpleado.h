/*
 * GrillaEmpleado.h
 *
 *  Created on: May 5, 2014
 *      Author: ferno
 *
 *  Basicamente es un array compartido donde el jefe le asigna un auto (ID) a un empleado (elemento del array)
 *  y este ultimo lo lee y atiende al cliente. A su vez, el empleado avisa al jefe que esta disponible escribiendo un
 *  cero (0) en el elemento del array correspondiente
 */

#ifndef GRILLAEMPLEADO_H_
#define GRILLAEMPLEADO_H_

#include <list>
#include "comm/ArrayCompartido.h"
#include "Semaforo.h"
#include "Pipe.h"

class GrillaEmpleado {
private:
	ArrayCompartido<unsigned int> _mem;
	Semaforo _sems[];

	Pipe _pipes[];

public:
	GrillaEmpleado(unsigned int cantEmpleados);
	virtual ~GrillaEmpleado();

	// Nota: Solo el JEFE o el creador principal debe inicializar la grilla
	void inicializarGrilla(unsigned int cantEmpleados);

	// Devuelve el ID de algun emplado libre. Si no hay ninguno libre, devuelve -1
	unsigned int getEmpleadoLibre() const;

	// Metodo usado por el jefe para marcar la LookUpTable como empleado asignado
	void asignarTrabajo(unsigned int idAuto, unsigned int idEmpleado);

	// Metodo usado por el empleado para esperar su siguiente trabajo
	unsigned int GrillaEmpleado::esperarTrabajo(unsigned int idEmpleado);

	// Metodo usado por un empleado que se libera de su trabajo para avisar al jefe
	void avisarTrabajoTerminado(unsigned int idEmpleado);
};

#endif /* GRILLAEMPLEADO_H_ */
