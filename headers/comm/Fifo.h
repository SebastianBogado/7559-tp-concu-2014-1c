#ifndef FIFO_H_
#define FIFO_H_

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Para envio de mensaje de error
#include <cstring>
#include <cerrno>

class Fifo {
public:
	Fifo(const std::string nombre);
	virtual ~Fifo();
	virtual void abrir() = 0;
	void cerrar();
	void eliminar() const;
	void crear();

protected:
	std::string nombre;
	int fd;
};

#endif /* FIFO_H_ */
