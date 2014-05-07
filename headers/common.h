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

// Archivo para el fifo entre jefe<->empleados
const std::string fifoJefeEmpleados = "/tmp/fifoJefeEmpleados";

#endif
