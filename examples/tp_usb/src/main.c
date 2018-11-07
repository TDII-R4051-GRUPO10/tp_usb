/* Copyright 2015, Pablo Ridolfi
 * All rights reserved.
 *
 * This file is part of lpc1769_template.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** @brief Blinky using FreeRTOS.
 *
 *
 * NOTE: It's interesting to check behavior differences between standard and
 * tickless mode. Set @ref configUSE_TICKLESS_IDLE to 1, increment a counter
 * in @ref vApplicationTickHook and print the counter value every second
 * inside a task. In standard mode the counter will have a value around 1000.
 * In tickless mode, it will be around 25.
 *
 */

/** \addtogroup rtos_blink FreeRTOS blink example
 ** @{ */

/*==================[inclusions]=============================================*/

#include "board.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "queue.h"
#include "main.h"
#include "cdc_vcom.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

char keys[16]= { '1','2','3','A','4','5','6','B','7','8','9','C','#','0','*','D'};

xQueueHandle queue_tecla;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void initHardware(void)
{
    SystemCoreClockUpdate();

    Board_Init();

    init_teclado();
}

void TeclaEvent (void *pvParameters)
{
	unsigned int EstadoDebounce = NO_OPRIMIDO;
	unsigned char tecla;
	portTickType xMeDesperte;

	//Inicio de variables y recuperación de parámetros

	xMeDesperte = xTaskGetTickCount();
	tecla = BOTON_NO_PRESIONADO;

	while(1){
	//debo verificar rebote
	switch(EstadoDebounce){
		case NO_OPRIMIDO:
			vTaskDelayUntil(&xMeDesperte,SCAN_RATE_ms/portTICK_RATE_MS);
			tecla = lectura_teclado();
			if(tecla != BOTON_NO_PRESIONADO)	//Si retorna una opresión
				EstadoDebounce = DEBOUNCE;		//Indico que esta corriendo el tiempo Debounce
			break;

		case DEBOUNCE:
			vTaskDelay(TIEMPO_DE_DEBOUNCE_ms/portTICK_RATE_MS);
			EstadoDebounce = VALIDAR;

		case VALIDAR:
			if(tecla == lectura_teclado())			//Si retorna algo sigue presionado
				EstadoDebounce=OPRIMIDO;
			else							// fue error
				EstadoDebounce=NO_OPRIMIDO;
			break;

		case OPRIMIDO:
			if(tecla != lectura_teclado()) //No envio mensaje hasta no soltar
			{
				xQueueSend( queue_tecla, keys[tecla], 0 );

				EstadoDebounce = NO_OPRIMIDO;
			}
			break;
		default:  break;
		}
	}
}


/*==================[external functions definition]==========================*/

int main(void)
{
	initHardware();

	queue_tecla = xQueueCreate(5,sizeof(char));

	xTaskCreate(taskCDC, (const char *)"taskCDC", configMINIMAL_STACK_SIZE*2, 0, tskIDLE_PRIORITY+2, 0);
	xTaskCreate(TeclaEvent, (const char *)"TeclaEvent", configMINIMAL_STACK_SIZE*2, 0, tskIDLE_PRIORITY+1, 0);

	vTaskStartScheduler();

	while (1) {
	}
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
