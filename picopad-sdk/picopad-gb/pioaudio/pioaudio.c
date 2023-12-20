#include <pico/stdlib.h>
#include <hardware/pio.h>
#include <hardware/dma.h>
#include "pwm.pio.h"
#include "config_def.h"
#include "pioaudio.h"

static void set_pwm_period(struct PioaudioCtx *c){
	pio_sm_set_enabled(c->pio, c->sm, false);
	pio_sm_put_blocking(c->pio, c->sm, 265);
	pio_sm_exec(c->pio, c->sm, pio_encode_pull(false, false));
	pio_sm_exec(c->pio, c->sm, pio_encode_out(pio_isr, 32));
}

static void pio_init(PIO pio, uint sm, uint offset){

	uint pin = 15;
	pio_gpio_init(pio, pin);
	pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
	pio_sm_config c = pwm_program_get_default_config(offset);
	sm_config_set_sideset_pins(&c, pin);
	sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);

	pio_sm_init(pio, sm, offset, &c);
}

void pioaudio_play(struct PioaudioCtx *c, void *buf, uint len){
	dma_channel_wait_for_finish_blocking(c->dma_ch);

	dma_channel_set_read_addr(c->dma_ch, buf, false);
	dma_channel_set_trans_count(c->dma_ch, len / 4, true);
}

void pioaudio_init(struct PioaudioCtx *c){
	c->pio = pio1;

	uint offset = pio_add_program(c->pio, &pwm_program);
	c->sm = pio_claim_unused_sm(c->pio, true);

	pio_init(c->pio, c->sm, offset);
	set_pwm_period(c);
	pio_sm_set_clkdiv_int_frac(c->pio, c->sm, 8, 0); //(266MHz / (266*4)) / 8 = 31250
	pio_sm_set_enabled(c->pio, c->sm, true);

	c->dma_ch = dma_claim_unused_channel(true);
	dma_channel_config dma_c = dma_channel_get_default_config(c->dma_ch);
	channel_config_set_transfer_data_size(&dma_c, DMA_SIZE_32);
	channel_config_set_read_increment(&dma_c, true);
	channel_config_set_dreq(&dma_c, pio_get_dreq(c->pio, c->sm, true));

	dma_channel_configure(
			c->dma_ch,
			&dma_c,
			&c->pio->txf[c->sm], // Write address (only need to set this once)
			NULL,
			0,
			false // Don't start yet
			);
}
