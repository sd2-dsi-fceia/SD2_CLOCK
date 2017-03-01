
/*==================[inclusions]=============================================*/
#include "board.h"
#include "MKL46Z4.h"
#include "fsl_port_hal.h"
#include "fsl_gpio_hal.h"
#include "fsl_sim_hal.h"
#include "fsl_clock_manager.h"
#include "fsl_i2c_hal.h"
#include "fsl_lpsci_hal.h"
#include "fsl_smc_hal.h"

/*==================[macros and definitions]=================================*/

/** \brief definiciones para el Led rojo */
#define LED_ROJO_PORT       PORTE
#define LED_ROJO_GPIO       GPIOE
#define LED_ROJO_PIN        29

/** \brief definiciones para el Led verde */
#define LED_VERDE_PORT      PORTD
#define LED_VERDE_GPIO      GPIOD
#define LED_VERDE_PIN       5

/** \brief definiciones para el SW1 */
#define SW1_PORT            PORTC
#define SW1_GPIO            GPIOC
#define SW1_PIN             3

/** \brief definiciones para el SW3 */
#define SW3_PORT            PORTC
#define SW3_GPIO            GPIOC
#define SW3_PIN             12

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/


void board_init(void)
{
    /* Activación de clock para los puertos utilizados */
    SIM_HAL_EnableClock(SIM, kSimClockGatePortA);
    SIM_HAL_EnableClock(SIM, kSimClockGatePortC);
    SIM_HAL_EnableClock(SIM, kSimClockGatePortD);
    SIM_HAL_EnableClock(SIM, kSimClockGatePortE);

    // Inicializo el clock del core
    ClockInit();

	/* =========== LED ROJO =============== */
	
    PORT_HAL_SetMuxMode(LED_ROJO_PORT, LED_ROJO_PIN, kPortMuxAsGpio);
    ledRojo_off();
	GPIO_HAL_SetPinDir(LED_ROJO_GPIO, LED_ROJO_PIN, kGpioDigitalOutput);
	
	/* =========== LED VERDE ============== */
	
	PORT_HAL_SetMuxMode(LED_VERDE_PORT, LED_VERDE_PIN, kPortMuxAsGpio);
    ledVerde_off();
    GPIO_HAL_SetPinDir(LED_VERDE_GPIO, LED_VERDE_PIN, kGpioDigitalOutput);
	
	/* =========== SW1 =================== */

    PORT_HAL_SetMuxMode(SW1_PORT, SW1_PIN, kPortMuxAsGpio);
    GPIO_HAL_SetPinDir(SW1_GPIO, SW1_PIN, kGpioDigitalInput);
    PORT_HAL_SetPullCmd(SW1_PORT, SW1_PIN, true);
    PORT_HAL_SetPullMode(SW1_PORT, SW1_PIN, kPortPullUp);
	
	/* =========== SW3 =================== */

    PORT_HAL_SetMuxMode(SW3_PORT, SW3_PIN, kPortMuxAsGpio);
    GPIO_HAL_SetPinDir(SW3_GPIO, SW3_PIN, kGpioDigitalInput);
    PORT_HAL_SetPullCmd(SW3_PORT, SW3_PIN, true);
    PORT_HAL_SetPullMode(SW3_PORT, SW3_PIN, kPortPullUp);
}

int8_t pulsadorSw1_get(void)
{
    if (GPIO_HAL_ReadPinInput(SW1_GPIO, SW1_PIN))
        return 0;
    else
        return 1;
}

int8_t pulsadorSw3_get(void)
{
    if (GPIO_HAL_ReadPinInput(SW3_GPIO, SW3_PIN))
        return 0;
    else
        return 1;
}

void ledRojo_on(void)
{
    GPIO_HAL_ClearPinOutput(LED_ROJO_GPIO, LED_ROJO_PIN);
}

void ledRojo_off(void)
{
    GPIO_HAL_SetPinOutput(LED_ROJO_GPIO, LED_ROJO_PIN);
}

void ledRojo_toggle(void)
{
    GPIO_HAL_TogglePinOutput(LED_ROJO_GPIO, LED_ROJO_PIN);
}

void ledVerde_on(void)
{
    GPIO_HAL_ClearPinOutput(LED_VERDE_GPIO, LED_VERDE_PIN);
}

void ledVerde_off(void)
{
    GPIO_HAL_SetPinOutput(LED_VERDE_GPIO, LED_VERDE_PIN);
}

void ledVerde_toggle(void)
{
    GPIO_HAL_TogglePinOutput(LED_VERDE_GPIO, LED_VERDE_PIN);
}

/*==================[end of file]============================================*/
