/*
*
*	TP 1 Técnicas de programación concurrentes I
*	Integrantes:
*		Bogado, Sebastián
*		Martty, Juan
*		Pereira, Fernando
*
*/

#include <cstdlib>
#include <ctime>

#include "logger/Logger.h"
#include "common.h"
#include "comm/Surtidores.h"
#include "comm/CajaRegistradora.h"
#include "comm/GrillaEmpleado.h"

int main(int argc, char* argv[]) {
	// Init Logger
	Logger::initialize(logFile.c_str(), Logger::LOG_DEBUG);

	srand(time(NULL));

	unsigned int idEmpleado, cantEmpleados;
	std::string idEmpleadoStr,
		me = __FILE__;


	// Parseo cabeza de args
	if ( argc > 1 ) {
		idEmpleado = atoi(argv[1]);
		idEmpleadoStr = toString(idEmpleado);
		me += "." + idEmpleadoStr + ":main";
		Logger::debug("Parámetro parseado 'idEmpleado': " + idEmpleadoStr, me);
	}  else {
		me += ":main";
		Logger::error("Argumentos insuficientes. Se espera \n"
					  "$ ./empleado <idEmpleado> <cantEmpleados>", me);
		return 1;
	}

	if ( argc > 2 ) {
		cantEmpleados = atoi(argv[2]);
		Logger::debug("Parámetro parseado 'cantEmpleados': " + toString(cantEmpleados), me);
	} else {
		Logger::error("Argumentos insuficientes. Se espera \n"
					  "$ ./empleado <idEmpleado> <cantEmpleados>", me);
		return 1;
	}

	// Get shmem sólo con cantEmpleados según Fer

	unsigned int idAuto, idSurtidor;
	double monto;
	std::string idAutoStr, idSurtidorStr, montoStr;

	while (true) {
		 Logger::debug("Entro a la cola de empleados libres", me);
		// Me encolo en una cola que no sé dónde está (bloq)
		 Logger::debug("Soy el próximo en la cola, el próximo auto me toca a mí", me);
		// Leo 4 bytes del fifo del jefe (bloq)
		// idAuto = grillaEmpleado.esperarTrabajo(idEmpleado);
		 idAuto = rand() % 10000;
		 idAutoStr = toString(idAuto);
		 Logger::debug("El jefe me asignó el auto: " + idAutoStr + ". Voy a buscar surtidor libre", me);
		// Pido paso al semáforo de surtidores (bloq)
		// Busco en la tabla un surtidor libre, y lo marco como ocupado
		// idSurtidor = surtidores.conseguirSurtidorLibre(idEmpleado);
		 idSurtidor = rand() % 10;
		 idSurtidorStr = toString(idSurtidor);
		 Logger::debug("Está libre el surtidor: " + idSurtidorStr + ". Cargando al auto: " + idAutoStr + "...", me);
		// Cargo la nafta (delay random) (bloq)
		 sleep(rand() % 10 + 2);
		// Dejo de usar el surtidor y lo informo
		// liberarSurtidor(idSurtidor);
		 Logger::debug("Terminé de cargarle al auto: " + idAutoStr + ". Libero el surtidor: " + idSurtidorStr, me);
		 monto = (rand() % 20000 + 5000) / 100.0;
		 montoStr = toString(monto);
		 Logger::debug("Voy a la caja registradora a depositar: " + montoStr, me);
		// Pido paso al semáforo de la caja registradora (bloq)
		// Sumo la cantidad total de dinero que me pagó el auto (delay random pequeño) (bloq)
		// cajaRegistradora.depositar(monto);
		 Logger::debug("Deposité en la caja registradora: " + montoStr + " y vuelvo a ponerme libre", me);
		// Marco en la grilla que ya estoy libre
		// grillaEmpleado.avisarTrabajoTerminado(idEmpleado);
		break;
	}


	Logger::destroy();

	return 0;
}
