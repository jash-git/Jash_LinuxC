/*	$Id: write.c,v 1.22 2011/04/28 21:14:32 kristaps Exp $ */
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
#include <sys/types.h>

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "html.h"
#include "extern.h"

/*
 * A note on documentation in this file.  Parse functions are generally
 * documented with notes on the relevant parts of the SGML, XML, XHTML,
 * or HTML reference.  These were originally culled from:
 *
 *   SGML: http://xml.coverpages.org/sgmlsyn/sgmlsyn.htm
 *   XML: http://www.w3.org/TR/2008/REC-xml-20081126/
 *   XHTML: http://www.w3.org/TR/2002/REC-xhtml1-20020801
 *   HTML: http://www.w3.org/TR/1999/REC-html401-19991224
 *
 * In general, this file is a generalised writer for SGML/XML as
 * constrained by the DOM tree interface walk routines.
 *
 * It manages minutiae such as follow:
 *
 *  - collapsing empty XML elements into <elem />;
 *  - manging minimised attributes;
 *  - upper-case (SGML) and lower-case (XML) attributes and names;
 *  - and so on.
 */

struct	hwrite {
	enum hmode	  hmode;
};


static	int	   hwrite_attr(struct hwrite *, 
			struct hattr *, struct ioout *);
static	int	   hwrite_comm(struct hwrite *, 
			struct hnode *, struct ioout *);
static	int	   hwrite_ctag(struct hwrite *, 
			struct hnode *, struct ioout *);
static	int	   hwrite_name(struct hwrite *, 
			const char *, struct ioout *);
static	int	   hwrite_node(struct hwrite *, 
			struct hnode *, struct ioout *);
static	int	   hwrite_otag(struct hwrite *, 
			struct hnode *, struct ioout *);
static	int	   hwrite_proc(struct hwrite *, 
			struct hnode *, struct ioout *);
static	int	   hwrite_text(struct hwrite *, 
			struct hnode *, struct ioout *);
static	int	   hwrite_type(struct hwrite *, 
			struct hnode *, struct ioout *);


struct hwrite *
hwrite_alloc(enum hmode mode)
{
	struct hwrite	*p;

	p = calloc(1, sizeof(struct hwrite));
	if (NULL == p) 
		return(NULL);

	p->hmode = mode;
	return(p);
}


void
hwrite_delete(struct hwrite *p)
{

	free(p);
}


void
hwrite_mode(struct hwrite *p, enum hmode mode)
{

	p->hmode = mode;
}


int
hwrite_tree(struct hwrite *p, struct ioout *out, struct hnode *n)
{
	int		 c;

	/* 
	 * FIXME: first, we can't transform from XHTML into HTML because
	 * of things like namespaces (XHTML is a superset of HTML).
	 * Second, when going into XHTML from HTML, we don't have
	 * necessary things like xmlns.
	 */
	if ( ! ioout_open(out))
		return(0);
	c = hwrite_node(p, n, out);
	if ( ! ioout_close(out))
		return(0);
	return(c);
}


static int
hwrite_node(struct hwrite *p, struct hnode *n, struct ioout *out)
{
	struct hnode	*np;
	int		 c;
	
	switch (hnode_type(n)) {
	case (HNODE_ELEM):
		c = hwrite_otag(p, n, out);
		break;
	case (HNODE_TEXT):
		c = hwrite_text(p, n, out);
		break;
	case (HNODE_COMMENT):
		c = hwrite_comm(p, n, out);
		break;
	case (HNODE_DECL):
		c = hwrite_type(p, n, out);
		break;
	case (HNODE_PROC):
		c = hwrite_proc(p, n, out);
		break;
	default:
		c = 1;
		break;
	}

	if (0 == c)
		return(0);

	for (np = hnode_child(n); np; np = hnode_sibling(np))
		hwrite_node(p, np, out);

	if (HNODE_ELEM == hnode_type(n))
		if ( ! hwrite_ctag(p, n, out))
			return(0);

	return(1);
}


static int
hwrite_name(struct hwrite *p, const char *n, struct ioout *out)
{

	if (HMODE_SGML & /* LINTED */ p->hmode)
		return(ioout_puts(out, n));

	/* XHTML ref: 4.2 */

	for ( ; '\0' != *n; n++)
		if ( ! ioout_putchar(out, (char)tolower((u_char)*n)))
			return(0);

	return(1);
}


static int
hwrite_attr(struct hwrite *p, struct hattr *a, struct ioout *out)
{

	if ( ! hwrite_name(p, hattr_name(hattr_type(a)), out))
		return(0);

	/* HTML ref: Attribute declarations: 3.3.4 */
	/* XHTML ref: Attribute Minimization: 4.5 */

	if ( ! hattr_value(a) && HMODE_XML & /* LINTED */ p->hmode) {
		if ( ! ioout_puts(out, "=\""))
			return(0);
		if ( ! ioout_puts(out, hattr_name(hattr_type(a))))
			return(0);
		return(ioout_putchar(out, '\"'));
	} else if ( ! hattr_value(a))
		return(1);

	if ( ! ioout_putchar(out, '='))
		return(0);

	if (hattr_literal(a) && ! ioout_putchar(out, '\"'))
		return(0);
	if ( ! ioout_puts(out, hattr_value(a)))
		return(0);
	if (hattr_literal(a) && ! ioout_putchar(out, '\"'))
		return(0);

	return(1);
}


static int
hwrite_proc(struct hwrite *p, struct hnode *n, struct ioout *out)
{
	struct hattr	*hp;

	assert(HNODE_PROC == hnode_type(n));

	if ( ! ioout_puts(out, "<?"))
		return(0);

	switch (hnode_proc(n)) {
	case (HPROC_XML):
		if ( ! ioout_puts(out, "xml"))
			return(0);
		break;
	default:
		break;
	}

	for (hp = hnode_attr(n); hp; hp = hattr_sibling(hp)) {
		if ( ! ioout_putchar(out, ' '))
			return(0);
		if ( ! hwrite_attr(p, hp, out))
			return(0);
	}

	if (HMODE_XML & /* LINTED */ p->hmode)
		if ( ! ioout_putchar(out, '?'))
			return(0);

	return(ioout_putchar(out, '>'));
}


/* ARGSUSED */
static int
hwrite_type(struct hwrite *p, struct hnode *n, struct ioout *out)
{
	struct hident	*ip;

	assert(HNODE_DECL == hnode_type(n));

	if ( ! ioout_puts(out, "<!"))
		return(0);

	switch (hnode_decl(n)) {
	case (HDECL_DOCTYPE):
		if ( ! ioout_puts(out, "DOCTYPE"))
			return(0);
		break;
	default:
		break;
	}

	for (ip = hnode_ident(n); ip; ip = hident_sibling(ip)) {
		if ( ! ioout_putchar(out, ' '))
			return(0);
		if (hident_literal(ip)) {
			if ( ! ioout_putchar(out, '\"'))
				return(0);
			if ( ! ioout_puts(out, hident_value(ip)))
				return(0);
			if ( ! ioout_putchar(out, '\"'))
				return(0);
		} else if ( ! ioout_puts(out, hident_value(ip)))
			return(0);
	}

	return(ioout_putchar(out, '>'));
}


/* ARGSUSED */
static int
hwrite_comm(struct hwrite *p, struct hnode *n, struct ioout *out)
{

	assert(HNODE_COMMENT == hnode_type(n));
	if ( ! ioout_puts(out, "<!--"))
		return(0);
	if ( ! ioout_puts(out, hnode_comment(n)))
		return(0);
	return(ioout_puts(out, "-->"));
}


static int
hwrite_otag(struct hwrite *p, struct hnode *n, struct ioout *out)
{
	struct hattr	*hp;

	assert(HNODE_ELEM == hnode_type(n));

	if ( ! ioout_putchar(out, '<'))
		return(0);
	if ( ! hwrite_name(p, helem_name(hnode_elem(n)), out))
		return(0);

	for (hp = hnode_attr(n); hp; hp = hattr_sibling(hp)) {
		if ( ! ioout_putchar(out, ' '))
			return(0);
		if ( ! hwrite_attr(p, hp, out))
			return(0);
	}

	if (HMODE_XML & /* LINTED */ p->hmode) 
		if (NULL == hnode_child(n))
			if ( ! ioout_puts(out, " /"))
				return(0);

	return(ioout_putchar(out, '>'));
}


static int
hwrite_ctag(struct hwrite *p, struct hnode *n, struct ioout *out)
{

	assert(HNODE_ELEM == hnode_type(n));

	if (NULL == hnode_child(n)) {
		if (HMODE_XML & /* LINTED */ p->hmode)
			return(1);
		if (helem_empty(hnode_elem(n), p->hmode))
			return(1);
	}

	if ( ! ioout_puts(out, "</"))
		return(0);
	if ( ! hwrite_name(p, helem_name(hnode_elem(n)), out))
		return(0);
	return(ioout_putchar(out, '>'));
}


/* ARGSUSED */
static int
hwrite_text(struct hwrite *p, struct hnode *n, struct ioout *out)
{

	assert(HNODE_TEXT == hnode_type(n));
	assert(hnode_text(n));
	return(ioout_puts(out, hnode_text(n)));
}
