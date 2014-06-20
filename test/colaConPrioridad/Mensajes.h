#ifndef MENSAJES_H_
#define MENSAJES_H_

#define	PETICION	1
#define	PETICION_IMPORTANTE	2
#define	RESPUESTA	5
#define	RESPUESTA_IMPORTANTE	6
#define TEXTO_SIZE	255


typedef struct mensaje {
	long mtype;
	int id;
	char texto[TEXTO_SIZE];
} mensaje;

#endif /* MENSAJES_H_ */
