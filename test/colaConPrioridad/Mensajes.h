#ifndef MENSAJES_H_
#define MENSAJES_H_

#define TEXTO_SIZE	255


typedef struct mensaje {
	int id;
	char texto[TEXTO_SIZE];
} mensaje;

#endif /* MENSAJES_H_ */
