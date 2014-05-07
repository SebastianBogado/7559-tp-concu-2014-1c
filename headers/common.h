#pragma once

/*
*
*
*	Constantes y demases globales al sistema
*/

#ifndef COMMON_H_
#define COMMON_H_

#include <string>

// Archivo de log
const std::string logFile = "log.txt";

// Archivo para el pipe fifo entre input<->jefeEstacion
const std::string fifoInputJefe = "/tmp/fifoInputJefe";

// Archivo para la memoria compartida de la grilla
const std::string shmemGrilla = "/tmp/grillaEmpleados";

// Archivo para el fifo entre jefe<->empleados
const std::string fifoJefeEmpleados = "/tmp/fifoJefeEmpleados";

// Archivo para la memoria compartida de los surtidores
const std::string shmemSurtidores = "/tmp/Surtidores";

// Archivo para el semaforo de los surtidores disponibles
const std::string semSurtidoresDisponibles = "/tmp/SurtidoresDisponibles";

#endif
