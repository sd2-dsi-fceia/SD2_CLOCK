
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

/* EXTAL0 PTA18 */
#define EXTAL0_PORT   PORTA
#define EXTAL0_PIN    18
#define EXTAL0_PINMUX kPortPinDisabled

/* XTAL0 PTA19 */
#define XTAL0_PORT   PORTA
#define XTAL0_PIN    19
#define XTAL0_PINMUX kPortPinDisabled

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

/* Configuration for enter VLPR mode. Core clock = 4MHz. */
const clock_manager_user_config_t g_defaultClockConfigVlpr =
{
    .mcgConfig =
    {
        .mcg_mode           = kMcgModeBLPI,   // Work in BLPI mode.
        .irclkEnable        = true,  // MCGIRCLK enable.
        .irclkEnableInStop  = false, // MCGIRCLK disable in STOP mode.
        .ircs               = kMcgIrcFast, // Select IRC4M.
        .fcrdiv             = 0U,    // FCRDIV is 0.

        .frdiv   = 0U,
        .drs     = kMcgDcoRangeSelLow,  // Low frequency range
        .dmx32   = kMcgDmx32Default,    // DCO has a default range of 25%

        .pll0EnableInFllMode        = false,  // PLL0 disable
        .pll0EnableInStop  = false,  // PLL0 disalbe in STOP mode
        .prdiv0            = 0U,
        .vdiv0             = 0U,
    },
    .simConfig =
    {
        .pllFllSel = kClockPllFllSelFll, // PLLFLLSEL select FLL.
        .er32kSrc  = kClockEr32kSrcLpo,     // ERCLK32K selection, use LPO.
        .outdiv1   = 0U,
        .outdiv4   = 4U,
    },
    .oscerConfig =
    {
        .enable       = true,  // OSCERCLK enable.
        .enableInStop = false, // OSCERCLK disable in STOP mode.
    }
};

/* Configuration for enter RUN mode. Core clock = 48MHz. */
const clock_manager_user_config_t g_defaultClockConfigRun =
{
    .mcgConfig =
    {
        .mcg_mode           = kMcgModePEE,   // Work in PEE mode.
        .irclkEnable        = true,  // MCGIRCLK enable.
        .irclkEnableInStop  = false, // MCGIRCLK disable in STOP mode.
        .ircs               = kMcgIrcSlow, // Select IRC32k.
        .fcrdiv             = 0U,    // FCRDIV is 0.

        .frdiv   = 3U,
        .drs     = kMcgDcoRangeSelLow,  // Low frequency range
        .dmx32   = kMcgDmx32Default,    // DCO has a default range of 25%

        .pll0EnableInFllMode        = false,  // PLL0 disable
        .pll0EnableInStop  = false,  // PLL0 disalbe in STOP mode
        .prdiv0            = 0x1U,
        .vdiv0             = 0x0U,
    },
    .simConfig =
    {
        .pllFllSel = kClockPllFllSelPll,    // PLLFLLSEL select PLL.
        .er32kSrc  = kClockEr32kSrcLpo,     // ERCLK32K selection, use LPO.
        .outdiv1   = 1U,
        .outdiv4   = 3U,
    },
    .oscerConfig =
    {
        .enable       = true,  // OSCERCLK enable.
        .enableInStop = false, // OSCERCLK disable in STOP mode.
    }
};

static void CLOCK_SetBootConfig(clock_manager_user_config_t const* config)
{
    CLOCK_SYS_SetSimConfigration(&config->simConfig);

    CLOCK_SYS_SetOscerConfigration(0, &config->oscerConfig);

#if (CLOCK_INIT_CONFIG == CLOCK_VLPR)
    CLOCK_SYS_BootToBlpi(&config->mcgConfig);
 #else
    CLOCK_SYS_BootToPee(&config->mcgConfig);
 #endif

    SystemCoreClock = CORE_CLOCK_FREQ;
}

/* Function to initialize OSC0 base on board configuration. */
void BOARD_InitOsc0(void)
{
    // OSC0 configuration.
    osc_user_config_t osc0Config =
    {
        .freq                = OSC0_XTAL_FREQ,
        .hgo                 = MCG_HGO0,
        .range               = MCG_RANGE0,
        .erefs               = MCG_EREFS0,
        .enableCapacitor2p   = OSC0_SC2P_ENABLE_CONFIG,
        .enableCapacitor4p   = OSC0_SC4P_ENABLE_CONFIG,
        .enableCapacitor8p   = OSC0_SC8P_ENABLE_CONFIG,
        .enableCapacitor16p  = OSC0_SC16P_ENABLE_CONFIG,
    };

    CLOCK_SYS_OscInit(0U, &osc0Config);
}



void board_init(void)
{
    /* Activación de clock para los puertos utilizados */
    SIM_HAL_EnableClock(SIM, kSimClockGatePortA);
    SIM_HAL_EnableClock(SIM, kSimClockGatePortC);
    SIM_HAL_EnableClock(SIM, kSimClockGatePortD);
    SIM_HAL_EnableClock(SIM, kSimClockGatePortE);

    // Inicializo el clock del core
    BOARD_ClockInit();

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


/* Initialize clock. */
void BOARD_ClockInit(void)
{
    /* Set allowed power mode, allow all. */
    SMC_HAL_SetProtection(SMC, kAllowPowerModeAll);

    /* Setup board clock source. */
    // Setup OSC0 if used.
    // Configure OSC0 pin mux.
    PORT_HAL_SetMuxMode(EXTAL0_PORT, EXTAL0_PIN, EXTAL0_PINMUX);
    PORT_HAL_SetMuxMode(XTAL0_PORT, XTAL0_PIN, XTAL0_PINMUX);
    BOARD_InitOsc0();


    CLOCK_SetBootConfig(&g_defaultClockConfigRun);

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
