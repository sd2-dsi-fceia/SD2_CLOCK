/* Copyright 2017, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 * Copyright 2017, Diego Alegrechi
 * Copyright 2017, Gustavo Muro
 * All rights reserved.
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

/*==================[inclusions]=============================================*/

#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_clock_manager.h"

#include "fsl_sim_hal.h"
#include "fsl_mcg_hal.h"
#include "fsl_tpm_hal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/* Configuration for enter VLPR mode. Core clock = 4MHz. */
const clock_manager_user_config_t g_defaultClockConfigVlpr =
{
    .mcgConfig =
    {
        .mcg_mode           = kMcgModeBLPI,   // Work in BLPI mode.
        .irclkEnable        = true,  	// MCGIRCLK enable.
        .irclkEnableInStop  = false, 	// MCGIRCLK disable in STOP mode.
        .ircs               = kMcgIrcFast, // Select IRC4M.
        .fcrdiv             = 0U,    	// FCRDIV is 0 (Divide Factor is 1).

        .frdiv   = 0U,					// Divide Factor is 1
        .drs     = kMcgDcoRangeSelLow,  // Low frequency range
        .dmx32   = kMcgDmx32Default,    // DCO has a default range of 25%

        .pll0EnableInFllMode = false,	// PLL0 disable
        .pll0EnableInStop  = false,		// PLL0 disable in STOP mode
        .prdiv0            = 0U,
        .vdiv0             = 0U,
    },
    .simConfig =
    {
        .pllFllSel = kClockPllFllSelFll,// PLLFLLSEL select FLL.
        .er32kSrc  = kClockEr32kSrcLpo,	// ERCLK32K selection, use LPO.
        .outdiv1   = 0U,
        .outdiv4   = 4U,
    },
    .oscerConfig =
    {
        .enable       = true,  			// OSCERCLK enable.
        .enableInStop = false, 			// OSCERCLK disable in STOP mode.
    }
};

/* Configuration for enter RUN mode. Core clock = 48MHz. */
const clock_manager_user_config_t g_defaultClockConfigRun =
{
    .mcgConfig =
    {
        .mcg_mode           = kMcgModePEE,   // Work in PEE mode.
        .irclkEnable        = true,  	// MCGIRCLK enable.
        .irclkEnableInStop  = false, 	// MCGIRCLK disable in STOP mode.
        .ircs               = kMcgIrcFast, // Select IRC32k.
        .fcrdiv             = 0U,    	// FCRDIV is 0.

        .frdiv   = 3U,
        .drs     = kMcgDcoRangeSelLow,  // Low frequency range
        .dmx32   = kMcgDmx32Default,    // DCO has a default range of 25%

        .pll0EnableInFllMode        = false,  // PLL0 disable
        .pll0EnableInStop  = false,  	// PLL0 disable in STOP mode
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


/*==================[internal functions declaration]=========================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

void TPM_Init(void)
{
	uint32_t channel = 0;

	// Habilito clock del periferico
	SIM_HAL_EnableClock(SIM, kSimClockGateTpm0);

	// Detengo cuenta del Timer
	TPM_HAL_SetClockMode(TPM0, kTpmClockSourceNoneClk);

	// Reseteo el timer
	TPM_HAL_Reset(TPM0, 0);

	// Clock prescaler = 128
	TPM_HAL_SetClockDiv(TPM0, kTpmDividedBy128);

	// Borro bandera de overflow
	TPM_HAL_ClearTimerOverflowFlag(TPM0);

	// Configuro valor MOD a TPM_MOD_MOD_MASK (0xFFFF)
	TPM_HAL_SetMod(TPM0, TPM_MOD_MOD_MASK);

	// Uso TPM solo como timer -- Deshabilito todos los canales
	for (channel = 0; channel < FSL_FEATURE_TPM_CHANNEL_COUNT ; channel++)
	{
		TPM_HAL_DisableChn(TPM0, channel);
	}

	// Cuenta ascendente
	TPM_HAL_SetCpwms(TPM0, 0);

	// Activo interupcion ante bandera de OverFlow
	TPM_HAL_EnableTimerOverflowInt(TPM0);

	// Elijo la fuente de clock para el TPM (OSCERCLK)
	CLOCK_HAL_SetTpmSrc(SIM, 0, kClockTpmSrcOsc0erClk);

	// Habilito el Timer -- Fuente de clock interna
	TPM_HAL_SetClockMode(TPM0, kTpmClockSourceModuleClk);
}

/*==================[external functions definition]==========================*/

extern const clock_manager_user_config_t g_defaultClockConfigRun;
extern const clock_manager_user_config_t g_defaultClockConfigVlpr;

int main(void)
{
	uint32_t i;

	board_init();

	// Se inicializa el timer
	TPM_Init();

	// Se habilitan interrupciones de TPM
	NVIC_ClearPendingIRQ(TPM0_IRQn);
	NVIC_EnableIRQ(TPM0_IRQn);

	while (1)
	{
		// Delay por software
		i = 300000;
		while (i--)
		{
			__asm("nop");
		}

		ledRojo_toggle();

		if(pulsadorSw3_get())
		{
			CLOCK_SYS_SetConfiguration(&g_defaultClockConfigRun);
		}

		if(pulsadorSw1_get())
		{
			CLOCK_SYS_SetConfiguration(&g_defaultClockConfigVlpr);
		}
    }
}

///////////////////////////////////////////////////////////////////////////////
// TPM0 IRQ
///////////////////////////////////////////////////////////////////////////////

void TPM0_IRQHandler(void)
{
	// Borro bandera de overflow
	TPM_HAL_ClearTimerOverflowFlag(TPM0);

	ledVerde_toggle();
}

/*==================[end of file]============================================*/
