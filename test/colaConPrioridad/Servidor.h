#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include <string>
#include <string.h>
#include <sstream>

#include "Mensajes.h"
#include "comm/ColaConPrioridad.hpp"
#include "logger/Logger.h"
#include "common.h"


class Servidor {

	private:
		ColaConPrioridad<mensaje>* cola;

	public:
		Servidor ( const std::string& archivo,const char letra );
		~Servidor ();

		mensaje recibirPeticion ();

};

#endif /* SERVIDOR_H_ */
