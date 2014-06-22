#pragma once

/*
*
*
*	Constantes y otras cosas globales al sistema
*/

#ifndef COMMON_H_
#define COMMON_H_

#include <string>
#include <sstream>

// Archivo de log
const std::string logFile = "log.txt";

// Archivo para la memoria compartida de la caja
const std::string shmemCaja = "/tmp/shmemCaja";

// Archivo para el semaforo de la shmem de la caja
const std::string semCaja = "/tmp/semCaja";

// Archivo para la cola entre input<->jefeEstacion
const std::string colaInputJefe = "/tmp/colaInputJefe";
const char colaInputJefeKey = 'z';

// Archivo para la memoria compartida de la grilla
const std::string shmemGrilla = "/tmp/grillaEmpleados";

// Archivo para el fifo entre jefe<->empleados
const std::string fifoJefeEmpleados = "/tmp/fifoJefeEmpleados";

// Archivo para la memoria compartida de los surtidores
const std::string shmemSurtidores = "/tmp/Surtidores";

// Archivo para el semaforo de los surtidores disponibles
const std::string semSurtidoresDisponibles = "/tmp/SurtidoresDisponibles";

// Retorna convertido en string el elemento
template<typename T> std::string toString(const T& elem) {
    std::stringstream ss;
    std::string res;

	ss << elem;
	ss >> res;

	return res;
}

typedef struct automovil {
	int id;
	bool esVip;
} automovil;

const automovil automovilFinal = { -1, true };

#endif
