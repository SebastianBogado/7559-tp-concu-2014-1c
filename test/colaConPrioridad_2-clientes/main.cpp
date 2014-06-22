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


/* El cliente 2, que manda mensajes con mayor prioridad, se conecta 1 segundo
 * para asegurar que manda sus mensajes después.
 * El servidor se conecta 3 segundos después, para asegurar que al menos haya
 * un mensaje con prioridad y otro sin.
 */
int main () {

	static const int CANTIDAD_INTERCAMBIOS = 3;
	Logger::initialize(__FILE__".log", Logger::LOG_DEBUG);

	srand(time(NULL));
	int processId = fork ();

	if ( processId == 0 ) {
		// cliente con prioridad

		sleep(1);
		Cliente clienteConPrioridad ( __FILE__,'a' );

		for ( int i=0;i<CANTIDAD_INTERCAMBIOS;i++ ) {

			// se arma el texto del mensaje
			std::stringstream peticion;
			peticion << "Peticion " << (i+1) << " del cliente con prioridad";

			// se envia el mensaje al servidor
			cout << "Cliente con prioridad: enviando peticion" << endl;
			mensaje rta = clienteConPrioridad.enviarPeticionImportante ( i+1,peticion.str() );
			cout << "Cliente con prioridad: respuesta recibida = (ID = " << rta.id << ") - " << rta.texto << endl;
			sleep(rand() % 4);
		}


		return 0;

	} else {

		int processId2 = fork ();

		if ( processId2 == 0 ) {
			// cliente
			Cliente cliente ( __FILE__,'a' );

			for ( int i=0;i<CANTIDAD_INTERCAMBIOS;i++ ) {

				// se arma el texto del mensaje
				std::stringstream peticion;
				peticion << "Peticion " << (i+1) << " del cliente";

				// se envia el mensaje al servidor
				cout << "Cliente: enviando peticion" << endl;
				mensaje rta = cliente.enviarPeticion ( i+1,peticion.str() );
				cout << "Cliente: respuesta recibida = (ID = " << rta.id << ") - " << rta.texto << endl;
			}
			sleep(rand() % 4);

			return 0;

		} else {
			// servidor

			sleep(3);
			Servidor servidor ( __FILE__,'a' );
			for ( int i=0;i<CANTIDAD_INTERCAMBIOS*2;i++ ) {
				cout << "Servidor: esperando peticiones" << endl;
				servidor.recibirPeticion ();
				cout << "Servidor: peticion recibida: " << servidor.getPeticionRecibida().texto << endl;
				servidor.procesarPeticion ();
				cout << "Servidor: peticion procesada - enviando respuesta: " << servidor.getRespuesta().texto << endl;
				servidor.responderPeticion ();
				cout << "Servidor: respuesta enviada" << endl << endl;
				sleep(1);
			}
			wait ( NULL );
			wait ( NULL );

			return 0;
		}
	}
}

