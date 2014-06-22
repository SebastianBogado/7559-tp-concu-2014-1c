#include "Cliente.h"

Cliente :: Cliente ( const std::string& archivo,const char letra ) {
	cola = new ColaConPrioridad<mensaje> ( archivo,letra );
}

Cliente :: ~Cliente() {
	delete cola;
}

void Cliente :: enviarPeticion ( const int id,const std::string& texto ) const {
	mensaje peticion;

	peticion.id = id;
	strcpy ( peticion.texto,texto.c_str() );

	cola->escribir ( peticion );
}


void Cliente :: enviarPeticionImportante ( const int id,const std::string& texto ) const {
	mensaje peticion;

	peticion.id = id;
	strcpy ( peticion.texto,texto.c_str() );

	cola->escribir ( peticion, 1 );
}


void Cliente :: enviarPeticionSuperImportante ( const int id,const std::string& texto ) const {
	mensaje peticion;

	peticion.id = id;
	strcpy ( peticion.texto,texto.c_str() );

	cola->escribir ( peticion, 2 );
}
