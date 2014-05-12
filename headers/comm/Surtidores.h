/*
 * Surtidores.h
 *
 *  Created on: May 7, 2014
 *      Author: ferno
 *
 *  Esta clase se encarga de manejar la abstracción de los Surtidores para con los empleados, manteniendo un array de
 *  X cantidad de surtidores pasado por parámetro con sus respectivos semáforos y un semáforo de X cantidad de
 *  surtidores como manejador de surtidores disponibles
 */

#ifndef SURTIDORES_H_
#define SURTIDORES_H_

#include "sync/Semaforo.h"
#include "comm/ArrayCompartido.h"
#include <vector>
#include <cstdlib>
#include "common.h"

class Surtidores {
private:
	ArrayCompartido<unsigned int> _surtidores;
	std::vector<Semaforo> _sems;

	Semaforo _surtidoresDisponibles;

	void inicializarSurtidores();

public:
	Surtidores(unsigned int cantSurtidores);
	virtual ~Surtidores();

	void destruirSurtidores();

	// Este metodo consigue un surtidor de la lista. Devuelve el numero de surtidor que le toco al empleado
	unsigned int conseguirSurtidorLibre(unsigned int idEmpleado);

	// El empleado deja de usar el surtidor luego de atender al cliente
	void liberarSurtidor(unsigned int idSurtidor);
};



#endif /* SURTIDORES_H_ */
