/*	$Id: ioout.c,v 1.6 2011/07/05 09:29:11 kristaps Exp $ */
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
#include <stdio.h>
#include <stdlib.h>

#include "html.h"
#include "extern.h"

int
ioout_puts(struct ioout *out, const char *msg)
{

	assert(out->puts);
	return((*out->puts)(msg, out->arg));
}

int
ioout_putchar(struct ioout *out, char c)
{

	assert(out->putchar);
	return((*out->putchar)(c, out->arg));
}

int
ioout_close(struct ioout *out)
{

	return(out->close ? (*out->close)(out->arg) : 1);
}

int
ioout_open(struct ioout *out)
{

	return(out->open ? (*out->open)(out->arg) : 1);
}

/* ARGSUSED */
int
iostdout_puts(const char *msg, void *arg)
{

	printf("%s", msg);
	return(1);
}

/* ARGSUSED */
int
iostdout_putchar(char c, void *arg)
{

	putchar(c);
	return(1);
}
