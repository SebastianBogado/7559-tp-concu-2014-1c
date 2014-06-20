#include "Servidor.h"

Servidor :: Servidor ( const std::string& archivo,const char letra ) {
	this->cola = new Cola<mensaje> ( archivo,letra );
}

Servidor :: ~Servidor () {
	delete this->cola;
}

int Servidor :: recibirPeticion () {
	this->peticionRecibida.id = 0;
	this->cola->leer ( PETICION,&(this->peticionRecibida) );
	return 0;
}

int Servidor :: procesarPeticion () {
	std::stringstream textoRta;
	textoRta << "[Respuesta a " << this->peticionRecibida.texto << "]";

	this->respuesta.mtype = RESPUESTA;
	this->respuesta.id = this->peticionRecibida.id;
	strcpy ( this->respuesta.texto,textoRta.str().c_str() );

	return 0;
}

int Servidor :: responderPeticion () const {
	this->cola->escribir ( this->respuesta );
	return 0;
}

mensaje Servidor :: getPeticionRecibida () {
	return this->peticionRecibida;
}

mensaje Servidor :: getRespuesta () {
	return this->respuesta;
}

