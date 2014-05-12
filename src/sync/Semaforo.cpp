#include "sync/Semaforo.h"

Semaforo :: Semaforo ( const std::string& nombre,const int valorInicial ):id(0),valorInicial(valorInicial) {}

Semaforo::~Semaforo() {}

int Semaforo::crear(const std::string& nombre,const int valorInicial) {
	this->valorInicial = valorInicial;
	key_t clave = ftok ( nombre.c_str(),'a' );

	if (clave == -1) {
		std::string mensaje = std::string("Error obteniendo la clave para semaforo. Error: ") + std::string(strerror(errno));
		throw mensaje;
	}

	this->id = semget ( clave,1,0666 | IPC_CREAT );

	if (this->id == -1) {
		std::string mensaje = std::string("Error obteniendo el semaforo. Error: ") + std::string(strerror(errno));
		throw mensaje;
	}

	return (this->inicializar());
}

int Semaforo :: inicializar () const {

	union semnum {
		int val;
		struct semid_ds* buf;
		ushort* array;
	};

	semnum flag;
	struct semid_ds buf;
	semnum init;

	// TODO: TESTTTTTT!
	// Check si el sem ya fue inicializado
	flag.buf = &buf;
	if (semctl(this->id,0,IPC_STAT,flag) == -1) {
		std::string mensaje = std::string("Error intentando de obtener sem_otime el semaforo. Error: ") + std::string(strerror(errno));
		throw mensaje;
	}
	if (flag.buf->sem_otime == 0) {
		// No ha sido inicializado!
		init.val = this->valorInicial;
		int resultado = semctl ( this->id,0,SETVAL,init );
		if (resultado == -1) {
			std::string mensaje = std::string("Error inicializando el semaforo. Error: ") + std::string(strerror(errno));
			throw mensaje;
		}
		return resultado;
	}
	return 0;
}

int Semaforo :: p () const {

	struct sembuf operacion;

	operacion.sem_num = 0;	// numero de semaforo
	operacion.sem_op  = -1;	// restar 1 al semaforo
	operacion.sem_flg = SEM_UNDO;

	int resultado = semop ( this->id,&operacion,1 );
	if (resultado == -1) {
		std::string mensaje = std::string("Error intentando entrar al semaforo. Error: ") + std::string(strerror(errno));
		throw mensaje;
	}
	return resultado;
}

int Semaforo :: v () const {

	struct sembuf operacion;

	operacion.sem_num = 0;	// numero de semaforo
	operacion.sem_op  = 1;	// sumar 1 al semaforo
	operacion.sem_flg = SEM_UNDO;

	int resultado = semop ( this->id,&operacion,1 );
	if (resultado == -1) {
		std::string mensaje = std::string("Error intentando salir del semaforo. Error: ") + std::string(strerror(errno));
		throw mensaje;
	}
	return resultado;
}

void Semaforo :: eliminar () const {
	if (semctl ( this->id,0,IPC_RMID ) == -1) {
		std::string mensaje = std::string("Error intentando cerrar el semaforo. Error: ") + std::string(strerror(errno));
		throw mensaje;
	}
}
