#include "teclado.h"

void init_teclado() {

	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, gpioSensor[FILA0].port, gpioSensor[FILA0].pin);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, gpioSensor[FILA1].port, gpioSensor[FILA1].pin);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, gpioSensor[FILA2].port, gpioSensor[FILA2].pin);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, gpioSensor[FILA3].port, gpioSensor[FILA3].pin);

	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, gpioSensor[COL0].port, gpioSensor[COL0].pin);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, gpioSensor[COL1].port, gpioSensor[COL1].pin);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, gpioSensor[COL2].port, gpioSensor[COL2].pin);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, gpioSensor[COL3].port, gpioSensor[COL3].pin);
}

void set_filas(bool fila0, bool fila1, bool fila2, bool fila3)
{
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, gpioSensor[FILA0].port, gpioSensor[FILA0].pin, fila0);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, gpioSensor[FILA1].port, gpioSensor[FILA1].pin, fila1);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, gpioSensor[FILA2].port, gpioSensor[FILA2].pin, fila2);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, gpioSensor[FILA3].port, gpioSensor[FILA3].pin, fila3);
}

bool get_columna(unsigned char columna)
{
	switch(columna)
	{
		case 0:
			return Chip_GPIO_GetPinState(LPC_GPIO_PORT, gpioSensor[COL0].port, gpioSensor[COL0].pin);
			break;
		case 1:
			return Chip_GPIO_GetPinState(LPC_GPIO_PORT, gpioSensor[COL1].port, gpioSensor[COL1].pin);
			break;
		case 2:
			return Chip_GPIO_GetPinState(LPC_GPIO_PORT, gpioSensor[COL2].port, gpioSensor[COL2].pin);
			break;
		case 3:
			return Chip_GPIO_GetPinState(LPC_GPIO_PORT, gpioSensor[COL3].port, gpioSensor[COL3].pin);
			break;
		default:
			return true;
	}
}

unsigned char lectura_teclado(void)
{
	set_filas(false, true, true, true);

	vTaskDelay(1/portTICK_RATE_MS);

	if(!get_columna(0)) return 0;
	if(!get_columna(1)) return 1;
	if(!get_columna(2)) return 2;
	if(!get_columna(3)) return 3;

	set_filas(true, false, true, true);

	vTaskDelay(1/portTICK_RATE_MS);

	if(!get_columna(0)) return 4;
	if(!get_columna(1)) return 5;
	if(!get_columna(2)) return 6;
	if(!get_columna(3)) return 7;

	set_filas(true, true, false, true);

	vTaskDelay(1/portTICK_RATE_MS);

	if(!get_columna(0)) return 8;
	if(!get_columna(1)) return 9;
	if(!get_columna(2)) return 10;
	if(!get_columna(3)) return 11;

	set_filas(true, true, true, false);

	vTaskDelay(1/portTICK_RATE_MS);

	if(!get_columna(0)) return 12;
	if(!get_columna(1)) return 13;
	if(!get_columna(2)) return 14;
	if(!get_columna(3)) return 15;

	return BOTON_NO_PRESIONADO;
}
