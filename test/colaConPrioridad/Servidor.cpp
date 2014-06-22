#include "Servidor.h"

Servidor :: Servidor ( const std::string& archivo,const char letra ) {
	cola = new ColaConPrioridad<mensaje> ( archivo,letra );
}

Servidor :: ~Servidor () {
	cola->destruir ();
	delete cola;
}

mensaje Servidor :: recibirPeticion () {
	return cola->leer();
}

