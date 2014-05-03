// Pequeño programa para generar el input del programa principal
// Establece el standard de input para el programa principal
// Via stdin, se leen 'autos'. Cada auto es un string que representa un id, seguido de un semi-colon (;)
// Este programa se encarga de generar dicho stream, poniendo los delays entre cada auto como sea necesario
// Como parametro toma la cantidad de autos a generar
// Un ejemplo de salida seria
// ./input 4
// 1;2;3;4;(EOF)

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <unistd.h>

// Constantes de configuracion
// Minimo y maximo delay entre la llegada de autos consecutivos
const int min_delay_default = 1;
const int max_delay_default = 5;
const int cant_autos_default = 10;

int main(int argc, const char* argv[]) {

	// Cargamos la cantidad de autos a generar de argv o -1 (inf) si no hay argumentos
	const int cant_autos = (argc >= 2 ? std::atoi(argv[1]) : cant_autos_default);
	const int min_delay = (argc >= 3 ? std::atoi(argv[2]) : min_delay_default);
	const int max_delay = (argc >= 4 ? std::atoi(argv[3]) : max_delay_default);

	// rand seed
	std::srand(std::time(NULL));

	// Loop para generar
	for(int current=0;current < cant_autos;current++) {
		// sleep hasta que llega
		const int sleep_interval = (rand() % (max_delay - min_delay)) + min_delay;
		sleep(sleep_interval);
		// stdout del "auto"
		std::cout << current << ";" << std::flush;
	}

	return 0;
}
