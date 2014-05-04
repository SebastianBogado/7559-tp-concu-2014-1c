#pragma once

/*
*
*
*	Constantes y demases globales al sistema
*/

#include <string>

// Archivo de log
const char* logFile = "log.txt";

// Archivo para el pipe fifo entre input<->jefeEstacion
const std::string fifoInputJefe = "/tmp/fifoInputJefe";
