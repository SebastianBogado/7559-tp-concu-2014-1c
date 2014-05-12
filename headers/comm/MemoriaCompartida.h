#ifndef MEMORIACOMPARTIDA_H_
#define MEMORIACOMPARTIDA_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <string.h>
#include <iostream>
#include <errno.h>
#include "logger/Logger.h"
#include "common.h"

template <class T> class MemoriaCompartida {

private:
	int	shmId;
	T*	ptrDatos;
	std::string me;  // Debugging purposes

	int	cantidadProcesosAdosados() const;

public:
	MemoriaCompartida ();
	void crear ( const std::string& archivo,const char letra );
	void liberar ();

	MemoriaCompartida ( const std::string& archivo,const char letra );
	MemoriaCompartida ( const MemoriaCompartida& origen );
	~MemoriaCompartida ();
	MemoriaCompartida<T>& operator= ( const MemoriaCompartida& origen );
	void escribir ( const T& dato );
	T leer () const;
};

template <class T> MemoriaCompartida<T>::MemoriaCompartida ():shmId(0),ptrDatos(NULL), me(__FILE__) {
}

template <class T> void MemoriaCompartida<T>::crear ( const std::string& archivo,const char letra ) {
	std::string me = this->me + ":crear";
	Logger::debug("Iniciando creación de la memoria compartida", me);
	key_t clave = ftok ( archivo.c_str(),letra );
	Logger::debug("Se obtiene la clave para la memoria compartida", me);


	if ( clave > 0 ) {
		this->shmId = shmget ( clave,sizeof(T),0644|IPC_CREAT );

		if ( this->shmId > 0 ) {
			void* tmpPtr = shmat ( this->shmId,NULL,0 );
			if ( tmpPtr != (void*) -1 ) {
				this->ptrDatos = static_cast<T*> (tmpPtr);
				std::string mensaje = std::string("La cantidad de procesos attacheados al array compartido es de: ") + toString(cantidadProcesosAdosados());
				Logger::debug(mensaje, me);
			} else {
				std::string mensaje = std::string("Error en shmat(): ") + std::string(strerror(errno));
				Logger::error(mensaje, me);
				throw mensaje;
			}
		} else {
			std::string mensaje = std::string("Error en shmget(): ") + std::string(strerror(errno));
			Logger::error(mensaje, me);
			throw mensaje;
		}
	} else {
		std::string mensaje = std::string("Error en ftok(): ") + std::string(strerror(errno));
		Logger::error(mensaje, me);
		throw mensaje;
	}
}

template <class T> void MemoriaCompartida<T>::liberar() {
	std::string me = this->me + ":liberar";
	int errorDt = shmdt ( (void *) this->ptrDatos );

	if ( errorDt != -1 ) {
		int procAdosados = this->cantidadProcesosAdosados ();
		if ( procAdosados == 0 ) {
			if (shmctl ( this->shmId,IPC_RMID,NULL ) == -1) {
				std::string mensaje = std::string("Error destruyendo el Array Compartido al no haber procesos attacheados. Error: ") + std::string(strerror(errno));
				Logger::error(mensaje, me);
			}
		}
	} else {
		std::string mensaje = std::string("Error en shmdt(): ") + std::string(strerror(errno));
		Logger::error(mensaje, me);
		throw mensaje;
	}
}

template <class T> MemoriaCompartida<T>::MemoriaCompartida ( const std::string& archivo,const char letra ):shmId(0),ptrDatos(NULL), me(__FILE__) {
	std::string me = this->me + ":MemoriaCompartida(const std::string& , const char )";
	key_t clave = ftok ( archivo.c_str(),letra );

	if ( clave > 0 ) {
		this->shmId = shmget ( clave,sizeof(T),0644|IPC_CREAT );

		if ( this->shmId > 0 ) {
			void* tmpPtr = shmat ( this->shmId,NULL,0 );
			if ( tmpPtr != (void*) -1 ) {
				this->ptrDatos = static_cast<T*> (tmpPtr);
			} else {
				std::string mensaje = std::string("Error en shmat(): ") + std::string(strerror(errno));
				Logger::error(mensaje, me);
				throw mensaje;
			}
		} else {
			std::string mensaje = std::string("Error en shmget(): ") + std::string(strerror(errno));
			Logger::error(mensaje, me);
			throw mensaje;
		}
	} else {
		std::string mensaje = std::string("Error en ftok(): ") + std::string(strerror(errno));
		Logger::error(mensaje, me);
		throw mensaje;
	}
}

template <class T> MemoriaCompartida<T>::MemoriaCompartida ( const MemoriaCompartida& origen ):shmId(origen.shmId) {
	std::string me = this->me + ":MemoriaCompartida(const MemoriaCompartida& )";
	void* tmpPtr = shmat ( origen.shmId,NULL,0 );

	if ( tmpPtr != (void*) -1 ) {
		this->ptrDatos = static_cast<T*> (tmpPtr);
	} else {
		std::string mensaje = std::string("Error en shmat(): ") + std::string(strerror(errno));
		Logger::error(mensaje, me);
		throw mensaje;
	}
}

template <class T> MemoriaCompartida<T>::~MemoriaCompartida () {
}

template <class T> MemoriaCompartida<T>& MemoriaCompartida<T>::operator= ( const MemoriaCompartida& origen ) {
	std::string me = this->me + ":operator=";
	this->shmId = origen.shmId;
	void* tmpPtr = shmat ( this->shmId,NULL,0 );

	if ( tmpPtr != (void*) -1 ) {
		this->ptrDatos = static_cast<T*> (tmpPtr);
	} else {
		std::string mensaje = std::string("Error en shmat(): ") + std::string(strerror(errno));
		Logger::error(mensaje, me);
		throw mensaje;
	}

	return *this;
}

template <class T> void MemoriaCompartida<T>::escribir ( const T& dato ) {
	*(this->ptrDatos) = dato;
}

template <class T> T MemoriaCompartida<T>::leer() const {
	return *(this->ptrDatos);
}

template <class T> int MemoriaCompartida<T> :: cantidadProcesosAdosados () const {
	shmid_ds estado;
	if (shmctl ( this->shmId,IPC_STAT,&estado ) == -1) {
		std::string mensaje = std::string("Error en shmctl() al verificar la cantidad de procesos adosados: ") + std::string(strerror(errno));
		Logger::error(mensaje, me);
	}
	return estado.shm_nattch;
}

#endif
