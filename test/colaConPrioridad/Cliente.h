#ifndef CLIENTE_H_
#define CLIENTE_H_

#include <string>
#include <string.h>

#include "Mensajes.h"
#include "comm/ColaConPrioridad.hpp"

class Cliente {

	private:
		ColaConPrioridad<mensaje>* cola;

	public:
		Cliente ( const std::string& archivo,const char letra );
		~Cliente();
		void enviarPeticion ( const int id,const std::string& texto ) const;
		void enviarPeticionImportante ( const int id,const std::string& texto ) const;
		void enviarPeticionSuperImportante ( const int id,const std::string& texto ) const;
};

#endif /* CLIENTE_H_ */
