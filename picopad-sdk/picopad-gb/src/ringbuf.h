#ifndef RINGBUF_H
#define RINGBUF_H

#define RINGBUF_SIZE 1024 //must be power of 2

typedef struct Ringbuf{
	volatile unsigned read_idx;
	volatile unsigned write_idx;

	unsigned char buf[RINGBUF_SIZE] __attribute__((aligned(4)));
} Ringbuf;

void ringbufInit(Ringbuf *buf);

void ringbufGetReadPtr(Ringbuf *buf,
		void **ptr1, unsigned *size1,
		void **ptr2, unsigned *size2);

void ringbufMoveReadPtr(Ringbuf *buf, unsigned n);

void ringbufGetWritePtr(Ringbuf *buf,
		void **ptr1, unsigned *size1,
		void **ptr2, unsigned *size2);

void ringbufMoveWritePtr(Ringbuf *buf, unsigned n);
unsigned ringbufRead(Ringbuf *buf, unsigned n, void *dst);
void ringbufWrite(Ringbuf *buf, unsigned n, const void *src);

unsigned ringbufWrite_ublock(Ringbuf *buf, unsigned n, const void *src);

unsigned ringbufGetReadAvail(Ringbuf *buf);
unsigned ringbufGetWriteAvail(Ringbuf *buf);


#endif
