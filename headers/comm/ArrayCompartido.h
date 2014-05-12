/*
 * ArrayCompartido.h
 *
 *  Created on: May 6, 2014
 *      Author: ferno
 */

#ifndef ARRAYCOMPARTIDO_H_
#define ARRAYCOMPARTIDO_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <string.h>
#include <iostream>
#include <errno.h>
#include "logger/Logger.h"
#include "common.h"

template <class T> class ArrayCompartido {

private:
	int	shmId;
	T*	ptrDatos;
	unsigned int _size;
	std::string me;  // Debugging purposes

	int	cantidadProcesosAdosados() const;

public:
	ArrayCompartido();
	void crear ( const std::string& archivo,const char letra,const unsigned int tam );
	void liberar ();

	ArrayCompartido( const std::string& archivo,const char letra,const unsigned int tam );
	ArrayCompartido( const ArrayCompartido& origen );
	virtual ~ArrayCompartido();
	ArrayCompartido<T>& operator= ( const ArrayCompartido& origen );
	void escribir ( const T& dato, const unsigned int id );
	T leer (const unsigned int id) const;
	unsigned int size() const;
};

template <class T> ArrayCompartido<T>::ArrayCompartido():shmId(0),ptrDatos(NULL),_size(0),me(__FILE__) {
}

template <class T> void ArrayCompartido<T>::crear(const std::string& archivo,const char letra,const unsigned int tam) {
	std::string me = this->me + ":crear";
	key_t clave = ftok ( archivo.c_str(),letra );

	if ( clave > 0 ) {
		this->shmId = shmget ( clave,sizeof(T) * tam,0644|IPC_CREAT );
		this->_size = tam;
		if ( this->shmId > 0 ) {
			void* tmpPtr = (void*) shmat ( this->shmId,NULL,0 );
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

template <class T> void ArrayCompartido<T>::liberar() {
	std::string me = this->me + ":liberar";
	int errorDt = shmdt ( (void *) this->ptrDatos );

	if ( errorDt != -1 ) {
		this->ptrDatos = NULL;
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

template <class T> ArrayCompartido<T>::ArrayCompartido(const std::string& archivo,const char letra,const unsigned int tam):shmId(0),ptrDatos(NULL),_size(tam),me(__FILE__) {
	std::string me = this->me + ":ArrayCompartido(const std::string& , const char )";
	key_t clave = ftok ( archivo.c_str(),letra );

	if ( clave > 0 ) {
		this->shmId = shmget ( clave,sizeof(T) * tam,0644|IPC_CREAT );

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

template <class T> ArrayCompartido<T>::ArrayCompartido ( const ArrayCompartido& origen ):shmId(origen.shmId),_size(origen.size()),me(__FILE__) {
	std::string me = this->me + ":ArrayCompartido(const ArrayCompartido& )";
	void* tmpPtr = shmat ( origen.shmId,NULL,0 );

	if ( tmpPtr != (void*) -1 ) {
		this->ptrDatos = static_cast<T*> (tmpPtr);
	} else {
		std::string mensaje = std::string("Error en shmat(): ") + std::string(strerror(errno));
		Logger::error(mensaje, me);
		throw mensaje;
	}
}

template <class T> ArrayCompartido<T>::~ArrayCompartido () {
	if (this->ptrDatos != NULL) {
		std::string me = this->me + ":~ArrayCompartido";
		int errorDt = shmdt ( (void*) (this->ptrDatos) );

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
			std::cerr << mensaje << std::endl;
			Logger::debug(mensaje, me);
		}
	}
}

template <class T> ArrayCompartido<T>& ArrayCompartido<T>::operator= ( const ArrayCompartido& origen ) {
	std::string me = this->me + ":operator=";
	this->shmId = origen.shmId;
	this->_size = origen._size;
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

template <class T> void ArrayCompartido<T>::escribir ( const T& dato, const unsigned int id ) {
	this->ptrDatos[id] = dato;
}

template <class T> T ArrayCompartido<T>::leer(const unsigned int id) const {
	return (this->ptrDatos[id]);
}

template <class T> int ArrayCompartido<T>::cantidadProcesosAdosados () const {
	shmid_ds estado;
	if (shmctl ( this->shmId,IPC_STAT,&estado ) == -1) {
		std::string mensaje = std::string("Error en shmctl() al verificar la cantidad de procesos adosados: ") + std::string(strerror(errno));
		Logger::error(mensaje, me);
	}
	return estado.shm_nattch;
}

template <class T> unsigned int ArrayCompartido<T>::size() const {
	return _size;
}

#endif /* ARRAYCOMPARTIDO_H_ */
