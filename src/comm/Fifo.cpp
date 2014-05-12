#include "comm/Fifo.h"

Fifo::Fifo(const std::string nombre) : nombre(nombre), fd(-1) {}

void Fifo::crear() {
	if (mknod ( static_cast<const char*>(nombre.c_str()),S_IFIFO|0666,0 ) != 0) {
		std::string msg("Error en la creación del Fifo: Nombre = " + nombre + ". Errno = '" + std::string(strerror(errno)) + "'");
		throw msg;
	}
}

Fifo::~Fifo() {}

void Fifo::cerrar() {
	if (close ( fd ) != 0) {
		std::string msg("Error en el cierre del Fifo: Nombre = " + nombre + ". Errno = '" + std::string(strerror(errno)) + "'");
		throw msg;
	}
	fd = -1;
}

void Fifo::eliminar() const {
	if (unlink ( nombre.c_str() ) != 0) {
		std::string msg("Error en la eliminación del Fifo: Nombre = " + nombre + ". Errno = '" + std::string(strerror(errno)) + "'");
		throw msg;
	}
}
