/*	$Id: iovalid.c,v 1.11 2010/03/11 12:25:31 kristaps Exp $ */
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


static	const char * const ioinvalidnames[IOERR_IMAX] = {
	"attribute not supported in html version",
	"attribute value malformed",
	"invalid element in context",
	"malformed character reference",
	"unknown character reference",
	"bad attribute for element",
};


int
iovalid_attr(struct hattr *p, struct iovalid *v)
{

	if (v->valid_attr)
		return((*v->valid_attr)(p, v->arg));
	return(1);
}


int
iovalid_battr(struct hattr *p, enum ioinvalid err, struct iovalid *v)
{

	if (NULL == v->valid_battr)
		return(0);
	(*v->valid_battr)(p, ioinvalidnames[err], v->arg);
	return(0);
}


int
iovalid_ident(struct hident *p, struct iovalid *v)
{

	if (v->valid_ident)
		return((*v->valid_ident)(p, v->arg));
	return(1);
}


int
iovalid_node(struct hnode *p, struct iovalid *v)
{

	if (v->valid_node)
		return((*v->valid_node)(p, v->arg));
	return(1);
}


int
iovalid_bnode(struct hnode *p, enum ioinvalid err, struct iovalid *v)
{

	if (NULL == v->valid_bnode)
		return(0);
	(*v->valid_bnode)(p, ioinvalidnames[err], v->arg);
	return(0);
}


