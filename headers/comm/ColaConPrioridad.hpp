#ifndef COLA_CON_PRIORIDAD_HPP_
#define COLA_CON_PRIORIDAD_HPP_

#include <string>
#include "comm/Cola.hpp"
#include "logger/Logger.h"

template <class T> class ColaConPrioridad {
	private:
		Cola<T> cola;
		std::string me;  // Debugging purposes

	public:
		ColaConPrioridad ( const std::string& archivo,const char letra );
		~ColaConPrioridad();
		void escribir ( const T& dato, const char prioridad = 0 ) const;
		void leer ( T* buffer ) const;
		void destruir () const;
};

template <class T> ColaConPrioridad<T> :: ColaConPrioridad ( const std::string& archivo,const char letra )
	: me(__FILE__), cola(archivo, letra) {}

template <class T> ColaConPrioridad<T> :: ~ColaConPrioridad () {
	std::string me = this->me + ":~ColaConPrioridad";
}

template <class T> void ColaConPrioridad<T> :: destruir () const {
	std::string me = this->me + ":destruir";
	try {
		cola.destruir();
	} catch (const std::string& mensaje) {
		std::string mensaje = std::string("Error en cola.destruir(): " + mensaje);
		Logger::error(mensaje, me);
		throw mensaje;
	}
}

template <class T> void ColaConPrioridad<T> :: escribir ( const T& dato, const int prioridad ) const {
	std::string me = this->me + ":escribir";
	Logger::error("Not implemented", me);
}

template <class T> void ColaConPrioridad<T> :: leer ( T* buffer ) const {
	std::string me = this->me + ":leer";
	Logger::error("Not implemented", me);
}


#endif /* COLA_CON_PRIORIDAD_HPP_ */