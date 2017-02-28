
#ifndef BOARD_H_
#define BOARD_H_

/*==================[inclusions]=============================================*/
#include "MKL46Z4.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

#define CLOCK_VLPR 1U
#define CLOCK_RUN  2U
#define CLOCK_NUMBER_OF_CONFIGURATIONS 3U

#ifndef CLOCK_INIT_CONFIG
#define CLOCK_INIT_CONFIG CLOCK_RUN
#endif

#if (CLOCK_INIT_CONFIG == CLOCK_RUN)
#define CORE_CLOCK_FREQ 48000000U
#else
#define CORE_CLOCK_FREQ 4000000U
#endif

/* OSC0 configuration. */
#define OSC0_XTAL_FREQ 8000000U
#define OSC0_SC2P_ENABLE_CONFIG  false
#define OSC0_SC4P_ENABLE_CONFIG  false
#define OSC0_SC8P_ENABLE_CONFIG  false
#define OSC0_SC16P_ENABLE_CONFIG false
#define MCG_HGO0   kOscGainLow
#define MCG_RANGE0 kOscRangeVeryHigh
#define MCG_EREFS0 kOscSrcOsc

/* EXTAL0 PTA18 */
#define EXTAL0_PORT   PORTA
#define EXTAL0_PIN    18
#define EXTAL0_PINMUX kPortPinDisabled

/* XTAL0 PTA19 */
#define XTAL0_PORT   PORTA
#define XTAL0_PIN    19
#define XTAL0_PINMUX kPortPinDisabled

#define UART_INSTANCE   ((UART_Type*)UART0)

/* The UART to use for debug messages. */
#ifndef BOARD_DEBUG_UART_INSTANCE
    #define BOARD_DEBUG_UART_INSTANCE   0
    #define BOARD_DEBUG_UART_BASEADDR   UART0
#endif
#ifndef BOARD_DEBUG_UART_BAUD
    #define BOARD_DEBUG_UART_BAUD       115200
#endif


/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions definition]==========================*/

/** \brief inicializaci�n del hardware
 **
 **/
void board_init(void);

/** \brief Devuelve estado del pulsador sw1
 **
 ** \return 1: si el pulsdor est� apretado
 **         0: si el pulsador no est� apretado
 **/
int8_t pulsadorSw1_get(void);

/** \brief Devuelve estado del pulsador sw3
 **
 ** \return 1: si el pulsdor est� apretado
 **         0: si el pulsador no est� apretado
 **/
int8_t pulsadorSw3_get(void);

/** \brief Enciende Led rojo
 **
 **/
void ledRojo_on(void);

/** \brief ApagaLed rojo
 **
 **/
void ledRojo_off(void);

/** \brief Togglea Led rojo
 **
 **/
void ledRojo_toggle(void);

/** \brief Enciende Led verde
 **
 **/
void ledVerde_on(void);

/** \brief Apgaga Led verde
 **
 **/
void ledVerde_off(void);

/** \brief Togglea Led verde
 **
 **/
void ledVerde_toggle(void);


/* Function to initialize clock base on board configuration. */
void BOARD_ClockInit(void);

/* Function to initialize OSC0 base on board configuration. */
void BOARD_InitOsc0(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* BOARD_H_ */
