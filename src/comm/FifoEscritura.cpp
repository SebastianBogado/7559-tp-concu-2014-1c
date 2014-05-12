#include "comm/FifoEscritura.h"

FifoEscritura::FifoEscritura(const std::string nombre) : Fifo(nombre) {}

FifoEscritura::~FifoEscritura() {}

void FifoEscritura::abrir() {
	fd = open ( nombre.c_str(),O_WRONLY );
	if (fd == -1) {
		std::string msg("Error abriendo el Fifo para escritura: Nombre = " + nombre + ". Errno = '" + std::string(strerror(errno)) + "'");
		throw msg;
	}
}

ssize_t FifoEscritura::escribir(const void* buffer,const ssize_t buffsize) const {
	ssize_t bytesEscritos = 0;
	bytesEscritos = write ( fd,buffer,buffsize );

	if (bytesEscritos == -1) {
		std::string msg("Error escribiendo en el Fifo: Nombre = " + nombre + ". Errno = '" + std::string(strerror(errno)) + "'");
		throw msg;
	}

	return bytesEscritos;
}
