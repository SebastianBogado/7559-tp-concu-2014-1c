#ifndef COLA_CON_PRIORIDAD_HPP_
#define COLA_CON_PRIORIDAD_HPP_

#include <string>
#include "comm/Cola.hpp"
#include "logger/Logger.h"

#define NIVELES_DE_PRIORIDAD 10

template <class T> class ColaConPrioridad {
	private:
		typedef struct wrapper {
			long mtype;
			T dato;
		} wrapper;
		Cola<wrapper> cola;
		std::string me;  // Debugging purposes

	public:
		ColaConPrioridad ( );
		ColaConPrioridad ( const ColaConPrioridad<T>& otraCola );
		ColaConPrioridad ( const std::string& archivo,const char letra );
		~ColaConPrioridad();
		void escribir ( const T& dato, const long prioridad = 0 ) const;
		T leer () const;
		void destruir () const;
};



template <class T> ColaConPrioridad<T> :: ColaConPrioridad ( ): me(__FILE__) {}

template <class T> ColaConPrioridad<T> :: ColaConPrioridad ( const ColaConPrioridad<T>& otraCola )
	: cola(otraCola.cola), me(__FILE__) {}

template <class T> ColaConPrioridad<T> :: ColaConPrioridad ( const std::string& archivo,const char letra )
	: cola(archivo, letra), me(__FILE__) {}

template <class T> ColaConPrioridad<T> :: ~ColaConPrioridad () {
	std::string me = this->me + ":~ColaConPrioridad";
}

template <class T> void ColaConPrioridad<T> :: destruir () const {
	std::string me = this->me + ":destruir";
	cola.destruir();
}

template <class T> void ColaConPrioridad<T> :: escribir ( const T& dato, const long prioridad ) const {
	std::string me = this->me + ":escribir";
	wrapper wrapperObj = { NIVELES_DE_PRIORIDAD - prioridad, dato};
	cola.escribir(wrapperObj);
}

template <class T> T ColaConPrioridad<T> :: leer () const {
	std::string me = this->me + ":leer";
	wrapper wrapperObj;
	cola.leer( -NIVELES_DE_PRIORIDAD, &wrapperObj );
	return wrapperObj.dato;
}


#endif /* COLA_CON_PRIORIDAD_HPP_ */