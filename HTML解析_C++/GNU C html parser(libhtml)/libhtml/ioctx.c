/*	$Id: ioctx.c,v 1.9 2010/02/02 11:06:07 kristaps Exp $ */
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

#include "html.h"
#include "extern.h"


static	const char * const ioerrornames[IOERR_EMAX] = {
	"memory exhausted"		/* IOERR_MEM */
};

static	const char * const iosyntaxnames[IOERR_SMAX] = {
	"unexpected end of file",	/* IOERR_NEOF */
	"unknown tag name",		/* IOERR_TAG */
	"bad tag syntax",		/* IOERR_BTAG */
	"invalid scope closure",	/* IOERR_NCXT */
	"unknown attribute name",	/* IOERR_ATTR */
	"bad attribute syntax",		/* IOERR_BATTR */
	"unsupported sgml construct",	/* IOERR_SGML */
	"bad declaration syntax",	/* IOERR_BDECL */
	"unknown declaration name",	/* IOERR_DECL */
	"bad document type name",	/* IOERR_DTYPE */
	"bad processing syntax",	/* IOERR_BPROC */
	"bad processing name",		/* IOERR_PROC */
	"invalid child of parent",	/* IOERR_CHLD */
};

static	const char * const iowarnnames[IOERR_WMAX] = {
	"uppercase suggested",		/* IOERR_UPPER */
	"lowercase suggested",		/* IOERR_LOWER */
};


int
ioctx_open(struct ioctx *p)
{

	return(p->open ? (*p->open)(p->arg) : 1);
}


int
ioctx_close(struct ioctx *p)
{

	return(p->close ? (*p->close)(p->arg) : 1);
}


int
ioctx_rew(struct ioctx *p)
{

	assert(p->rew);
	return((*p->rew)(p->arg));
}


int
ioctx_getchar(struct ioctx *p, char *c)
{

	assert(p->getchar);
	return((*p->getchar)(c, p->arg));
}


int
ioctx_warn(struct ioctx *p, int line, int col, enum iowarn err)
{

	if (NULL == p->warn)
		return(1);
	return((*p->warn)(iowarnnames[err], line + 1, col, p->arg));
}


int
ioctx_syntax(struct ioctx *p, int line, int col, enum iosyntax err)
{

	if (NULL == p->syntax)
		return(0);
	(*p->syntax)(iosyntaxnames[err], line + 1, col, p->arg);
	return(0);
}


int
ioctx_error(struct ioctx *p, enum ioerror err)
{

	if (p->error)
		(*p->error)(ioerrornames[err], p->arg);
	return(0);
}


int
ioctx_xcode(struct ioctx *p, char c, char **cp, size_t *szp)
{

	*cp = NULL;
	if (NULL == p->xcode)
		return(1);
	return((*p->xcode)(c, cp, szp, p->arg));
}
