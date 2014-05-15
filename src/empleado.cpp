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
#include "comm/ArgHelper.h"

int main(int argc, char* argv[]) {
  
	// Parsear los argumentos que nos manda el padre
	bool debugMode;
	int cantEmpleados, cantSurtidores, idEmpleado;
	
	if(!ArgHelper::decode(argc, argv, &debugMode, &cantEmpleados, &cantSurtidores, &idEmpleado)) {
		std::cerr << "Error - invalid parameters passed to process " << argv[0] << std::endl;
		exit(1);
	}

	// Init Logger
	if(debugMode) {
		Logger::initialize(logFile.c_str(), Logger::LOG_DEBUG);
	}else{
		Logger::initialize(logFile.c_str(), Logger::LOG_WARNING);
	}

	srand(idEmpleado);

	std::string idEmpleadoStr = toString(idEmpleado),
		me = __FILE__"." + idEmpleadoStr + ":main";

	Logger::debug("Parámetros parseados: "
			"debug mode: " + toString(debugMode) + ", "
			"ID empleado: " + idEmpleadoStr + ", "
			"# empleados: " + toString(cantEmpleados) + ", "
			"# surtidores: " + toString(cantSurtidores), me);

	// Get shmem sólo con cantEmpleados según Fer

	CajaRegistradora cajaRegistradora;
	try {
		cajaRegistradora.crearCaja();
	} catch(std::string& msg) {
		std::string _msg("Error en la creacion de la caja registradora");
		Logger::error(_msg, me);
		exit(1);
	}
	Logger::notice("Caja registradora inicializada...", me);

	GrillaEmpleado grillaEmpleado;
	try {
		grillaEmpleado.crearGrillaEmpleado(cantEmpleados,idEmpleado);
	} catch(std::string& msg) {
		std::string _msg("Error en la creacion de la grilla del empleado");
		Logger::error(_msg, me);
		exit(1);
	}
	Logger::notice("Grilla de empleado inicializada...", me);

	Surtidores surtidores;
	try {
		surtidores.crearSurtidores(cantSurtidores);
	} catch(std::string& msg) {
		std::string _msg("Error en la creacion de los surtidores del empleado");
		Logger::error(_msg, me);
		exit(1);
	}

	int idAuto, idSurtidor;
	double monto;
	std::string idAutoStr, idSurtidorStr, montoStr;
	bool trabajando = true;

	while(trabajando) {
		Logger::debug("Entro a la cola de empleados libres", me);
		// Leo 4 bytes del fifo del jefe (bloq)
		idAuto = grillaEmpleado.esperarTrabajo(idEmpleado);
		// Detectamos el fin de la estacion con un -1 como id de auto
		if(idAuto == -1) {
		  trabajando = false;
		}else{
		  // idAuto = rand() % 10000;
		  idAutoStr = toString(idAuto);
		  Logger::debug("El jefe me asignó el auto: " + idAutoStr + ". Voy a buscar surtidor libre", me);
		  // Pido paso al semáforo de surtidores (bloq)
		  // Busco en la tabla un surtidor libre, y lo marco como ocupado
		  idSurtidor = surtidores.conseguirSurtidorLibre(idEmpleado);
		  // idSurtidor = rand() % 10;
		  idSurtidorStr = toString(idSurtidor);
		  Logger::debug("Está libre el surtidor: " + idSurtidorStr + ". Cargando al auto: " + idAutoStr + "...", me);
		  // Cargo la nafta (delay random) (bloq)
		  sleep(rand() % 10 + 2);
		  // Dejo de usar el surtidor y lo informo
		  surtidores.liberarSurtidor(idSurtidor);
		  Logger::debug("Terminé de cargarle al auto: " + idAutoStr + ". Libero el surtidor: " + idSurtidorStr, me);
		  monto = (rand() % 20000 + 5000) / 100.0;
		  montoStr = toString(monto);
		  Logger::debug("Voy a la caja registradora a depositar: " + montoStr, me);
		  // Pido paso al semáforo de la caja registradora (bloq)
		  // Sumo la cantidad total de dinero que me pagó el auto (delay random pequeño) (bloq)
		  cajaRegistradora.depositar(monto);
		  Logger::debug("Deposité en la caja registradora: " + montoStr + " y vuelvo a ponerme libre", me);
		  // Marco en la grilla que ya estoy libre
		  grillaEmpleado.avisarTrabajoTerminado(idEmpleado);
		}
	}


	Logger::debug("Fin de empleado", me);
	Logger::destroy();

	return 0;
}
