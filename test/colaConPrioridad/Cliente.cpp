#include "Cliente.h"

Cliente :: Cliente ( const std::string& archivo,const char letra ) {
	this->cola = new Cola<mensaje> ( archivo,letra );
}

Cliente :: ~Cliente() {
//	this->cola->destruir ();
	delete this->cola;
}

mensaje Cliente :: enviarPeticion ( const int id,const std::string& texto ) const {
	mensaje peticion;
	mensaje respuesta;

	peticion.mtype = PETICION;
	peticion.id = id;
	strcpy ( peticion.texto,texto.c_str() );

	this->cola->escribir ( peticion );
	this->cola->leer ( RESPUESTA,&respuesta );

	return respuesta;
}


mensaje Cliente :: enviarPeticionImportante ( const int id,const std::string& texto ) const {
	mensaje peticion;
	mensaje respuesta;

	peticion.mtype = PETICION;
	peticion.id = id;
	strcpy ( peticion.texto,texto.c_str() );

	this->cola->escribir ( peticion );
	this->cola->leer ( RESPUESTA,&respuesta );

	return respuesta;
}
