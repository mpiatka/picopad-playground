#include <pico/stdlib.h>
#include <hardware/pio.h>
#include "pwm.pio.h"
#include "config_def.h"
#include "pioaudio.h"

static void set_pwm_period(PIO pio, uint sm){
	pio_sm_set_enabled(pio, sm, false);
	pio_sm_put_blocking(pio, sm, 265);
	pio_sm_exec(pio, sm, pio_encode_pull(false, false));
	pio_sm_exec(pio, sm, pio_encode_out(pio_isr, 32));
}

static void pio_init(PIO pio, uint sm, uint offset){

	uint pin = 15;
	pio_gpio_init(pio, pin);
	pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
	pio_sm_config c = pwm_program_get_default_config(offset);
	sm_config_set_sideset_pins(&c, pin);

	pio_sm_init(pio, sm, offset, &c);
}

void pioaudio_init(){
	PIO	pio = pio1;

	uint offset = pio_add_program(pio, &pwm_program);
	uint sm = pio_claim_unused_sm(pio, true);

	pio_init(pio, sm, offset);
	set_pwm_period(pio, sm);
	pio_sm_set_clkdiv_int_frac(pio, sm, 8, 0); //(266MHz / (266*4)) / 8 = 31250
	pio_sm_set_enabled(pio, sm, true);
}
