#ifndef COLA_HPP_
#define COLA_HPP_

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string>
#include "logger/Logger.h"

template <class T> class Cola {
	private:
		key_t	clave;
		int		id;
		std::string me;  // Debugging purposes


	public:
		Cola ( const std::string& archivo,const char letra );
		~Cola();
		int escribir ( const T& dato ) const;
		int leer ( const int tipo,T* buffer ) const;
		int destruir () const;
};

template <class T> Cola<T> :: Cola ( const std::string& archivo,const char letra ): me(__FILE__) {
	std::string me = this->me + ":Cola";
	this->clave = ftok ( archivo.c_str(),letra );
	if ( this->clave == -1 ) {
		std::string mensaje = std::string("Error en ftok(): ") + std::string(strerror(errno));
		Logger::error(mensaje, me);
		throw mensaje;
	}

	this->id = msgget ( this->clave,0777|IPC_CREAT );
	if ( this->id == -1 ){
		std::string mensaje = std::string("Error en msgget(): ") + std::string(strerror(errno));
		Logger::error(mensaje, me);
		throw mensaje;
	}
}

template <class T> Cola<T> :: ~Cola () {
	std::string me = this->me + ":~Cola";
}

template <class T> int Cola<T> :: destruir () const {
	std::string me = this->me + ":destruir";
	int resultado = msgctl ( this->id,IPC_RMID,NULL );
	if ( resultado == -1 ){
		std::string mensaje = std::string("Error en msgctl(): ") + std::string(strerror(errno));
		Logger::error(mensaje, me);
		throw mensaje;
	}
	return resultado;
}

template <class T> int Cola<T> :: escribir ( const T& dato ) const {
	std::string me = this->me + ":escribir";
	int resultado = msgsnd ( this->id,static_cast<const void*>(&dato),sizeof(T)-sizeof(long),0 );
	if ( resultado == -1 ){
		std::string mensaje = std::string("Error en msgsnd(): ") + std::string(strerror(errno));
		Logger::error(mensaje, me);
		throw mensaje;
	}
	return resultado;
}

template <class T> int Cola<T> :: leer ( const int tipo,T* buffer ) const {
	std::string me = this->me + ":leer";
	int resultado = msgrcv ( this->id,static_cast<void *>(buffer),sizeof(T)-sizeof(long),tipo,0 );
	if ( resultado == -1 ){
		std::string mensaje = std::string("Error en msgrcv(): ") + std::string(strerror(errno));
		Logger::error(mensaje, me);
		throw mensaje;
	}
	return resultado;
}

#endif /* COLA_HPP_ */