#include "ringbuf.h"
#include "string.h"

#define SMALL_MASK ((RINGBUF_SIZE) - 1)
#define BIG_MASK (((RINGBUF_SIZE) * 2) - 1)

void ringbufInit(Ringbuf *buf){
	buf->read_idx = 0;
	buf->write_idx = 0;
}

void ringbufGetReadPtr(Ringbuf *buf,
		void **ptr1, unsigned *size1,
		void **ptr2, unsigned *size2)
{
	unsigned r_idx = buf->read_idx & SMALL_MASK;
	unsigned used = ringbufGetReadAvail(buf);

	*ptr1 = &buf->buf[r_idx];
	if(r_idx + used > RINGBUF_SIZE){
		*size1 = RINGBUF_SIZE - r_idx;
		*ptr2 = &buf->buf[0];
		*size2 = used - *size1;
	} else {
		*size1 = used;
		*ptr2 = 0;
		*size2 = 0;
	}
}

void ringbufMoveReadPtr(Ringbuf *buf, unsigned n){
	buf->read_idx += n;
	buf->read_idx &= BIG_MASK;
}

void ringbufGetWritePtr(Ringbuf *buf,
		void **ptr1, unsigned *size1,
		void **ptr2, unsigned *size2)
{
	unsigned w_idx = buf->write_idx & SMALL_MASK;
	unsigned free = ringbufGetWriteAvail(buf);

	*ptr1 = &buf->buf[w_idx];
	if(w_idx + free > RINGBUF_SIZE){
		*size1 = RINGBUF_SIZE - w_idx;
		*ptr2 = &buf->buf[0];
		*size2 = free - *size1;
	} else {
		*size1 = free;
		*ptr2 = 0;
		*size2 = 0;
	}
}

void ringbufMoveWritePtr(Ringbuf *buf, unsigned n){
	buf->write_idx += n;
	buf->write_idx &= BIG_MASK;
}

unsigned ringbufRead(Ringbuf *buf, unsigned n, void *dst){
	void *ptr1, *ptr2;
	unsigned size1, size2;

	unsigned bytes_read = 0;

	ringbufGetReadPtr(buf, &ptr1, &size1, &ptr2, &size2);

	unsigned rs = (n > size1) ? size1 : n;
	memcpy(dst, ptr1, rs);
	bytes_read += rs;
	n -= rs;
	dst = ((char *) dst) + rs;

	if(n > 0 && size2 != 0){
		rs = (n > size2) ? size2 : n;
		memcpy(dst, ptr2, rs);
		bytes_read += rs;
	}
	
	ringbufMoveReadPtr(buf, bytes_read);	

	return bytes_read;
}

unsigned ringbufWrite_ublock(Ringbuf *buf, unsigned n, const void *src){
	void *ptr1, *ptr2;
	unsigned size1, size2;

	unsigned bytes_written = 0;

	ringbufGetWritePtr(buf, &ptr1, &size1, &ptr2, &size2);

	unsigned ws = (n > size1) ? size1 : n;
	memcpy(ptr1, src, ws);
	bytes_written += ws;
	n -= ws;
	src = ((char *) src) + ws;

	if(n > 0 && size2 != 0){
		ws = (n > size2) ? size2 : n;
		memcpy(ptr2, src, ws);
		bytes_written += ws;
	}
	
	ringbufMoveWritePtr(buf, bytes_written);	

	return bytes_written;
}

void ringbufWrite(Ringbuf *buf, unsigned n, const void *src){
	const char *data = src;

	while (n > 0){
		unsigned written = ringbufWrite_ublock(buf, n, data);
		data += written;
		n -= written;
	}
}

unsigned ringbufGetReadAvail(Ringbuf *buf){
    return (buf->write_idx - buf->read_idx) & BIG_MASK;
}

unsigned ringbufGetWriteAvail(Ringbuf *buf){
    return RINGBUF_SIZE - ringbufGetReadAvail(buf);
}
