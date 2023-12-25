#ifndef PIOAUDIO_H_a95e015e5488
#define PIOAUDIO_H_a95e015e5488


#ifdef __cplusplus
extern "C" {
#endif

#include <hardware/pio.h>
#include <hardware/dma.h>
#define PIO_AUDIO_PIN 15;


struct PioaudioCtx{
	PIO pio;
	uint sm;

	uint dma_ch;
};

void pioaudio_init(struct PioaudioCtx *c);

void pioaudio_play(struct PioaudioCtx *c, void *buf, uint len);

static inline void pioaudio_wait(struct PioaudioCtx *c){
	dma_channel_wait_for_finish_blocking(c->dma_ch);
}

#ifdef __cplusplus
}
#endif

#endif
