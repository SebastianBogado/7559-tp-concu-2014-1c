#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include <string>
#include <string.h>
#include <sstream>

#include "Mensajes.h"
#include "comm/Cola.hpp"
#include "logger/Logger.h"
#include "common.h"


class Servidor {

	private:
		Cola<mensaje>* cola;
		mensaje peticionRecibida;
		mensaje respuesta;

	public:
		Servidor ( const std::string& archivo,const char letra );
		~Servidor ();

		int recibirPeticion ();
		int procesarPeticion ();
		int responderPeticion () const;

		mensaje getPeticionRecibida ();
		mensaje getRespuesta ();

};

#endif /* SERVIDOR_H_ */
