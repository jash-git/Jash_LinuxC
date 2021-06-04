/*	$Id: iobuf.c,v 1.5 2010/02/02 11:06:07 kristaps Exp $ */
/*
 * Copyright (c) 2009, 2010 Kristaps Dzonsons <kristaps@bsd.lv>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "html.h"
#include "extern.h"

#define	IOBUF_STEP	  512

/*
 * While data is being read, it's buffered for processing into the text
 * segments of nodes (element name or content).  This is a growable
 * buffer of that content.
 */
struct	iobuf {
	char		 *buf;
	size_t		  bufsz;
	size_t		  cursz;
};


struct iobuf *
iobuf_alloc(void)
{
	struct iobuf	*p;

	if (NULL == (p = calloc(1, sizeof(struct iobuf))))
		return(NULL);

	if (iobuf_grow(p))
		return(p);

	free(p);
	return(NULL);
}


const char *
iobuf_buf(const struct iobuf *p)
{
	
	return(p->buf);
}


size_t
iobuf_size(const struct iobuf *p)
{

	return(p->cursz);
}


void
iobuf_reset(struct iobuf *buf)
{

	assert(buf);
	buf->cursz = 0;
}


void
iobuf_final(struct iobuf *p)
{

	p->buf[(int)p->cursz] = '\0';
	p->cursz = 0;
}


void
iobuf_free(struct iobuf *p)
{

	if (p->buf)
		free(p->buf);
	free(p);
}


int
iobuf_puts(struct iobuf *buf, const char *p, size_t len)
{

	if (0 == len)
		return(1);

	assert(buf->bufsz);
	while (buf->cursz + len >= buf->bufsz - 1)
		if ( ! iobuf_grow(buf))
			return(0);

	memcpy(&buf->buf[(int)buf->cursz], p, len);
	buf->cursz += len;
	return(1);
}


int
iobuf_putchar(struct iobuf *buf, char c)
{

	assert(buf->bufsz);
	if (buf->cursz + 1 >= buf->bufsz - 1)
		if ( ! iobuf_grow(buf))
			return(0);

	buf->buf[(int)buf->cursz++] = c;
	return(1);
}


int
iobuf_grow(struct iobuf *buf)
{
	size_t		 sz;
	char		*p;

	sz = buf->bufsz + IOBUF_STEP;
	p = realloc(buf->buf, sz);

	if (NULL == p)
		return(0);

	buf->bufsz = sz;
	buf->buf = p;

	return(1);
}

