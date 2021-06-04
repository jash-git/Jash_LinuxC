/*	$Id: validate.c,v 1.18 2011/04/28 21:14:32 kristaps Exp $ */
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

#define	CHARREFSIZ	  16

struct	hvalid {
	enum hmode	  hmode;
};

static	int	helem_excl(struct hvalid *,
			struct iovalid *, struct hnode *,
			const enum helemt *);
static	int	hvalid_attr(struct hvalid *,
			struct iovalid *, struct hattr *);
static	int	hvalid_charref(struct hvalid *, 
			struct iovalid *, struct hnode *, 
			const char **);
static	int	hvalid_charrefnum(struct hvalid *, 
			struct iovalid *, struct hnode *, 
			const char **);
static	int	hvalid_elem(struct hvalid *,
			struct iovalid *, struct hnode *);
static	int	hvalid_ident(struct hvalid *,
			struct iovalid *, struct hident *);
static	int	hvalid_postnode(struct hvalid *,
			struct iovalid *, struct hnode *);
static	int	hvalid_prenode(struct hvalid *,
			struct iovalid *, struct hnode *);
static	int	hvalid_text(struct hvalid *,
			struct iovalid *, struct hnode *);


struct hvalid *
hvalid_alloc(enum hmode mode)
{
	struct hvalid	*p;

	p = calloc(1, sizeof(struct hvalid));
	if (NULL == p) 
		return(NULL);

	p->hmode = mode;
	return(p);
}


void
hvalid_delete(struct hvalid *p)
{

	free(p);
}


/* ARGSUSED */
static int
hvalid_attr(struct hvalid *p, struct iovalid *v, struct hattr *a)
{

	return(iovalid_attr(a, v));
}


/* ARGSUSED */
static int
hvalid_ident(struct hvalid *p, struct iovalid *v, struct hident *i)
{

	return(iovalid_ident(i, v));
}


/*
 * Depth-first search for exclusions.  DO NOT start this on the root
 * node of the search, as it may exclude its own type.  See
 * hvalid_prenode() for invocation.
 */
static int
helem_excl(struct hvalid *p, struct iovalid *v,
		struct hnode *n, const enum helemt *e)
{

	if (NULL == n || HNODE_ELEM != hnode_type(n))
		return(1);

	for ( ; HELEM__MAX != *e; e++)
		if (hnode_elem(n) == *e)
			return(iovalid_bnode(n, IOERR_ELEMEX, v));

	if ( ! helem_excl(p, v, hnode_child(n), e))
		return(0);
	return(helem_excl(p, v, hnode_sibling(n), e));
}


/*
 * Pre-node analysis, at this time limited to doing exclusion searches
 * on nodes and attribute validity.
 */
static int
hvalid_prenode(struct hvalid *p, struct iovalid *v, struct hnode *n)
{
	const enum helemt *excl;

	if (HNODE_ELEM != hnode_type(n))
		return(1);

	excl = helem_excludes(hnode_elem(n), p->hmode);
	if (NULL == excl)
		return(1);

	return(helem_excl(p, v, hnode_child(n), excl));
}


/* ARGSUSED */
static int
hvalid_postnode(struct hvalid *p, struct iovalid *v, struct hnode *n)
{

	switch (hnode_type(n)) {
	case (HNODE_ELEM):
		if ( ! hvalid_elem(p, v, n))
			return(0);
		break;
	case (HNODE_TEXT):
		if ( ! hvalid_text(p, v, n))
			return(0);
		break;
	default:
		break;
	}

	return(iovalid_node(n, v));
}


int
hvalid_tree(struct hvalid *p, struct iovalid *v, struct hnode *n)
{
	struct hnode	*np;
	struct hattr	*ap;
	struct hident	*ip;

	if ( ! hvalid_prenode(p, v, n))
		return(0);

	for (ap = hnode_attr(n); ap; ap = hattr_sibling(ap))
		if ( ! hvalid_attr(p, v, ap))
			return(0);

	for (ip = hnode_ident(n); ip; ip = hident_sibling(ip))
		if ( ! hvalid_ident(p, v, ip))
			return(0);

	for (np = hnode_child(n); np; np = hnode_sibling(np))
		if ( ! hvalid_tree(p, v, np))
			return(0);

	return(hvalid_postnode(p, v, n));
}


static int
hvalid_charrefnum(struct hvalid *p, struct iovalid *v, 
		struct hnode *n, const char **cpp)
{
	int		 hex, i;

	/* SGML ref: character reference: 9.5, 356:1 */
	/* XML ref: character reference: 66 */

	/* XML doesn't allow upper-case hex indicators. */
	if ('X' == **cpp && HMODE_XML & /* LINTED */ p->hmode)
		return(iovalid_bnode(n, IOERR_BCHARREF, v));

	if ('x' == **cpp || 'X' == **cpp) {
		hex = 1;
		(*cpp)++;
	} else
		hex = 0;

	for (i = 0; '\0' != **cpp; i++) {
		/* SGML allows us to exit with a newline. */
		if ('\n' == **cpp && HMODE_SGML & /* LINTED */ p->hmode)
			break;
		else if ('\n' == **cpp)
			return(iovalid_bnode(n, IOERR_BCHARREF, v));
		else if (';' == **cpp)
			break;

		if ( ! isdigit((u_char)**cpp)) {
			if (hex && ! isalpha((u_char)**cpp))
				return(iovalid_bnode(n, IOERR_BCHARREF, v));
			else if ( ! hex) 
				return(iovalid_bnode(n, IOERR_BCHARREF, v));
		}

		i++;
		(*cpp)++;
	}

	if ('\0' == **cpp)
		return(iovalid_bnode(n, IOERR_BCHARREF, v));
	else if (0 == i)
		return(iovalid_bnode(n, IOERR_BCHARREF, v));

	return(1);
}


static int
hvalid_charref(struct hvalid *p, struct iovalid *v, 
		struct hnode *n, const char **cpp)
{
	char		buf[CHARREFSIZ];
	int		i;

	if ('#' == **cpp) {
		(*cpp)++;
		return(hvalid_charrefnum(p, v, n, cpp));
	}

	/*
	 * FIXME: see FUBAR in parse_name() of parse.c regarding XML and
	 * unicode declarations.
	 */

	/* SGML ref: general entity reference: 9.4.4, 350:17 */
	/* XML ref: entity ref: 68 */

	if ( ! isalpha((u_char)**cpp))
		return(iovalid_bnode(n, IOERR_BCHARREF, v));

	i = 0;
	assert(i + 1 <= CHARREFSIZ - 1);
	buf[i++] = **cpp;
	(*cpp)++;

	while ('\0' != **cpp) {
		/* SGML allows us to exit with a newline. */
		if ('\n' == **cpp && /* LINTED */ HMODE_SGML & p->hmode)
			break;
		else if ('\n' == **cpp)
			return(iovalid_bnode(n, IOERR_BCHARREF, v));
		else if (';' == **cpp)
			break;

		if ( ! isalnum((u_char)**cpp) && 
				'.' != **cpp &&
				'-' != **cpp && 
				'_' != **cpp &&
				':' != **cpp)
			return(iovalid_bnode(n, IOERR_BCHARREF, v));

		if (i + 1 > CHARREFSIZ - 1)
			return(0);
		buf[i++] = **cpp;
		(*cpp)++;
	}

	if ('\0' == **cpp)
		return(iovalid_bnode(n, IOERR_BCHARREF, v));

	buf[i] = '\0';

	/*
	 * Make sure that the HTML version actually has a definition for
	 * the character reference we've parsed out.
	 */
	if (HCREF__MAX == hcref_find(buf))
		return(iovalid_bnode(n, IOERR_CHARREF, v));
	return(1);
}


static int
hvalid_text(struct hvalid *p, struct iovalid *v, struct hnode *n)
{
	const char	*cp;

	for (cp = hnode_text(n); '\0' != *cp; cp++) {
		/* SGML ref: replacable char data: 9.1, 343:1 */
		if ('&' != *cp)
			continue;
		cp++;
		if ( ! hvalid_charref(p, v, n, &cp))
			return(0);
	}

	return(1);
}


static int
hvalid_elem(struct hvalid *p, struct iovalid *v, struct hnode *n)
{
	struct hattr	*h;
	enum helemt	 elem;

	elem = hnode_elem(n);

	for (h = hnode_attr(n) ; h; h = hattr_sibling(h)) {
		if (hattr_elem_perm(hattr_type(h), elem, p->hmode))
			continue;
		return(iovalid_battr(h, IOERR_BELEMATTR, v));
	}

	return(1);
}
