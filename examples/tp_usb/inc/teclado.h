#ifndef _TECLADO_
#define _TECLADO_

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"

#define FILA0 	0
#define FILA1 	1
#define FILA2 	2
#define FILA3 	3
#define COL0 	4
#define COL1 	5
#define COL2 	6
#define COL3 	7

typedef struct {
	uint8_t port;
	uint8_t pin;
} io_port_t;

static const io_port_t gpioSensor[] = {{3,0},{3,3},{3,4},{5,15},{5,16},{3,5},{3,6},{3,7}};

#define BOTON_NO_PRESIONADO 99

void init_teclado();

void set_filas(bool fila0, bool fila1, bool fila2, bool fila3);

bool get_columna(unsigned char columna);

unsigned char lectura_teclado(void);

#endif /* #ifndef _TECLADO_ */
