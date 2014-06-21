#ifndef MENSAJES_H_
#define MENSAJES_H_

#define	NIVELES_DE_PRIORIDAD    5
#define	PETICION	            2
#define	PETICION_IMPORTANTE	    1
#define	RESPUESTA	            (PETICION + NIVELES_DE_PRIORIDAD)
#define	RESPUESTA_IMPORTANTE	(PETICION_IMPORTANTE + NIVELES_DE_PRIORIDAD)
#define SET_RESPUESTA(X)        ((X) + NIVELES_DE_PRIORIDAD)
#define TEXTO_SIZE	255


typedef struct mensaje {
	long mtype;
	int id;
	char texto[TEXTO_SIZE];
} mensaje;

#endif /* MENSAJES_H_ */
