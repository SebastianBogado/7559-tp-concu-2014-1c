/*
 * GrillaEmpleado.cpp
 *
 *  Created on: May 5, 2014
 *      Author: ferno
 */

#include "comm/Grilla.h"

Grilla::Grilla() : me(__FILE__) {}

void Grilla::crearGrilla(unsigned int cantEmpleados) {
	std::string me = this->me + ":crearGrilla";

	try {
		_mem.crear(shmemGrilla,'g',cantEmpleados);
	} catch(std::string& msg) {
		std::string _msg = std::string("Error creando memoria compartida para la grilla");
		Logger::error(_msg, me);

		remove(shmemGrilla.c_str());

		throw _msg;
	}
	Logger::debug("Memoria compartida para la grilla Jefe<->Empleados creada", me);

	for(unsigned int i = 0; i < cantEmpleados; i++) {
		// El semaforo esta disponible inicialmente para usarlo
		std::string filename("/tmp/Empleado" + toString(i));

		Semaforo tmpSem;
		try {
			tmpSem.crear(filename,1);
		} catch(std::string& msg) {
			std::string _msg(std::string("Error creando semaforo para la Grilla del empleado") + toString(i));
			Logger::error(_msg, me);

			destruir(i + 1);

			throw _msg;
		}

		_sems.push_back(tmpSem);
		filename.clear();
		Logger::debug(std::string("Se ha creado el semaforo para el empleado ") + toString(i), me);
	}

	Logger::notice("Creada la grilla Jefe<->Empleados correctamente", me);

}

void Grilla::inicializarGrilla(unsigned int cantEmpleados) {
	std::string me = this->me + ":inicializarGrilla";

	std::ofstream arch(shmemGrilla.c_str());
	if (arch.fail() || arch.bad()) {
		std::string _msg = std::string("Error creando archivo para la grilla de empleados. Error: ") + std::string(strerror(errno));
		Logger::error(_msg, me);
		throw _msg;
	}

	try {
		_mem.crear(shmemGrilla,'g',cantEmpleados);
	} catch(std::string& msg) {
		std::string _msg = std::string("Error creando memoria compartida para la grilla");
		Logger::error(_msg, me);

		arch.close();
		remove(shmemGrilla.c_str());

		throw _msg;
	}
	arch.close();
	Logger::debug("Memoria compartida para la grilla Jefe<->Empleados creada", me);

	for(unsigned int i = 0; i < cantEmpleados; i++) {
		// El semaforo esta disponible inicialmente para usarlo
		std::string filename("/tmp/Empleado" + toString(i));
		std::ofstream archSem(filename.c_str());
		if (archSem.fail() || archSem.bad()) {
			std::string _msg = std::string("Error creando archivo para el semaforo de la grilla. Nombre: '") + filename + std::string("'. Error: ") + std::string(strerror(errno));
			Logger::error(_msg, me);

			destruir(i);

			throw _msg;
		}

		Semaforo tmpSem;
		try {
			tmpSem.crear(filename,1);
		} catch(std::string& msg) {
			std::string _msg(std::string("Error creando semaforo para la Grilla del empleado") + toString(i));
			Logger::error(_msg, me);
			archSem.close();

			destruir(i + 1);

			throw _msg;
		}

		_sems.push_back(tmpSem);
		filename.clear();
		archSem.close();
		Logger::debug(std::string("Se ha creado el semaforo para el empleado ") + toString(i), me);
	}

	Logger::notice("Creada e inicializada la grilla Jefe<->Empleados correctamente", me);
}

Grilla::~Grilla() {
	//_mem.liberar();
	//for(unsigned int i = 0; i < _sems.size(); i++) {
		//_sems[i].eliminar();
	//}
}

void Grilla::destruir(unsigned int cantEmpleados) {
	remove(shmemGrilla.c_str());
	for(unsigned int i = 0; i < cantEmpleados; i++) {
		std::string filename("/tmp/Empleado" + toString(i));
		remove(filename.c_str());
	}
	_mem.liberar();
	for(unsigned int i = 0; i < _sems.size(); i++) {
		_sems[i].eliminar();
	}
}
