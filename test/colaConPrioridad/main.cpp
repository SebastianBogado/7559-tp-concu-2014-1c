#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctime>

#include "common.h"
#include "Cliente.h"
#include "Servidor.h"
#include "Mensajes.h"

using namespace std;


/* El cliente se conecta y manda 3 mensajes, uno con prioridad, otro con mayor
 * prioridad y 3 mensajes normales más .
 * El servidor se conecta 3 segundos después y debe leer primero el más
 * importante, después el segundo importante, y finalmente el resto de los mensajes
 * normales.
 */
int main () {

	static const int CANTIDAD_INTERCAMBIOS = 3;
	Logger::initialize(__FILE__".log", Logger::LOG_DEBUG);

	srand(time(NULL));
	int processId = fork ();

	if ( processId == 0 ) {
		// cliente
		Cliente cliente( __FILE__,'a' );
		int i=0, id;
		for ( ;i<CANTIDAD_INTERCAMBIOS;i++ ) {
			id = i + 1;
			// se envia el mensaje al servidor
			cout << "Cliente: enviando mensaje" << endl;
			cliente.enviarPeticion( id, "Mensaje " + toString(id) + " del cliente");
		}
		id = ++i;
		// se envia el mensaje al servidor
		cout << "Cliente: enviando mensaje importante " << endl;
		cliente.enviarPeticionImportante( id, "Mensaje importante " + toString(id) + " del cliente");

		id = ++i;
		// se envia el mensaje al servidor
		cout << "Cliente: enviando mensaje súper importante " << endl;
		cliente.enviarPeticionSuperImportante( id, "Mensaje súper importante " + toString(id) + " del cliente");


		for ( int j=i;j<CANTIDAD_INTERCAMBIOS+i;j++ ) {
			id = j + 1;
			// se envia el mensaje al servidor
			cout << "Cliente: enviando mensaje" << endl;
			cliente.enviarPeticion( id, "Mensaje " + toString(id) + " del cliente");
		}


		return 0;


	} else {
		// servidor

		sleep(3);
		Servidor servidor ( __FILE__,'a' );
		for ( int i=0;i<CANTIDAD_INTERCAMBIOS*2 + 2;i++ ) {
			cout << "Servidor: esperando mensajes" << endl;
			mensaje peticion = servidor.recibirPeticion ();
			cout << "Servidor: mensaje recibido: " << peticion.texto << endl;
		}
		wait ( NULL );

		return 0;
	}
}

