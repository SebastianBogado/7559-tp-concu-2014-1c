#include "comm/FifoLectura.h"

FifoLectura::FifoLectura(const std::string nombre) : Fifo(nombre) {
}

FifoLectura::~FifoLectura() {
}

void FifoLectura::abrir() {
	fd = open ( nombre.c_str(),O_RDONLY );
	if (fd == -1) {
		std::string msg("Error abriendo el Fifo para escritura: Nombre = " + nombre + ". Errno = '" + std::string(strerror(errno)) + "'");
		throw msg;
	}
}

ssize_t FifoLectura::leer(void* buffer,const ssize_t buffsize) const {
	ssize_t bytesLeidos = 0;
	bytesLeidos = read ( fd,buffer,buffsize );

	if (bytesLeidos == -1) {
		std::string msg("Error leyendo del Fifo: Nombre = " + nombre + ". Errno = '" + std::string(strerror(errno)) + "'");
		throw msg;
	}

	return bytesLeidos;
}
