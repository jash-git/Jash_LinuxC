/*	$Id: fd.c,v 1.1 2011/07/05 09:29:11 kristaps Exp $ */
/*
 * Copyright (c) 2011 Kristaps Dzonsons <kristaps@bsd.lv>
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
#include <sys/types.h>

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "html.h"

int
iofd_getchar(char *c, void *arg)
{
	struct iofd	*fd;
	ssize_t		 ssz;

	assert(arg && c);
	fd = (struct iofd *)arg;

	assert(-1 != fd->fd);

	if (FD_REW & fd->flags) {
		*c = fd->last;
		fd->flags &= ~FD_REW;
		return(1);
	}

	ssz = read(fd->fd, c, 1);

	if (1 == ssz) {
		fd->last = *c;
		return(1);
	} else if (0 == ssz)
		return(0);

	if (*fd->perr)
		(*fd->perr)(fd->file, fd->arg);

	return(-1);
}

int
iofd_open(void *arg)
{
	struct iofd	*fd;

	assert(arg);
	fd = (struct iofd *)arg;

	assert(-1 == fd->fd);
	if (-1 != (fd->fd = open(fd->file, O_RDONLY, 0)))
		return(1);

	if (*fd->perr)
		(*fd->perr)(fd->file, fd->arg);

	return(0);
}

int
iofd_rew(void *arg)
{
	struct iofd	*fd;

	assert(arg);
	fd = (struct iofd *)arg;

	assert( ! (FD_REW & fd->flags));
	fd->flags |= FD_REW;

	return(1);
}

int
iofd_close(void *arg)
{
	struct iofd	*fd;
	int		 c;

	assert(arg);
	fd = (struct iofd *)arg;

	assert(-1 != fd->fd);

	if (-1 == (c = close(fd->fd)))
		if (*fd->perr)
			(*fd->perr)(fd->file, fd->arg);

	fd->fd = -1;
	return(-1 != c);
}
