/************************************************************************/
/* Blink an LED at 2Hz using the ARM systick function                   */
/* Designed by: Justin Owen												*/
/************************************************************************/

#include <asf.h>

/************************************************************************/
/* Global Variables                                                     */
/************************************************************************/

volatile unsigned int sys_timer1 = 0, sys_timer2 = 0;

/************************************************************************/
/* INIT PORTS                                                           */
/************************************************************************/

void configure_port_pins(void)
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);

	config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(LED_0_PIN, &config_port_pin);
}

/************************************************************************/
/* INIT Clocks                                                          */
/************************************************************************/

void enable_tc_clocks(void)
{
	struct system_gclk_chan_config gclk_chan_conf;
	
	/* Turn on TC module in PM */
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, PM_APBCMASK_TC3);

	/* Set up the GCLK for the module */
	system_gclk_chan_get_config_defaults(&gclk_chan_conf);
	
	//Setup generic clock 0 (also the clock for MCU (running at 8 Mhz) as source for the timer clock)
	gclk_chan_conf.source_generator = GCLK_GENERATOR_0;
	system_gclk_chan_set_config(TC3_GCLK_ID, &gclk_chan_conf);
	
	//Enable the generic clock for the Timer/ Counter block
	system_gclk_chan_enable(TC3_GCLK_ID);
}

int main (void)
{
	system_init();
	system_clock_init();
	configure_port_pins();
	
	// SysTick every milliSec
	SysTick_Config(8000);
	
	while (1) 
	{
		if (sys_timer1 > 250) // Toggle every 250 milliSec
		{
			port_pin_toggle_output_level(LED_0_PIN);
			sys_timer1 = 0;
		}
		
	}
}

/************************************************************************/
/* Subroutines                                                          */
/************************************************************************/

void SysTick_Handler(void)
{
	sys_timer1++;
}