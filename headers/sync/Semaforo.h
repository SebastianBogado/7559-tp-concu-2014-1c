#ifndef SEMAFORO_H_
#define SEMAFORO_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <string>

// Check de error y log
#include <cstring>
#include <cerrno>

class Semaforo {

private:
	int id;
	int valorInicial;

	int inicializar () const;

public:
	Semaforo() : id(0),valorInicial(0) {};
	Semaforo ( const std::string& nombre,const int valorInicial );
	~Semaforo();

	int crear(const std::string& nombre,const int valorInicial);
	int p () const; // decrementa
	int v () const; // incrementa
	void eliminar () const;
};

#endif /* SEMAFORO_H_ */
