/*	$Id: parse.c,v 1.34 2010/06/16 08:44:36 kristaps Exp $ */
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
 * In general, this file is a constrained SGML parser (i.e., focusses
 * only on the HTML parts of SGML, e.g., hparse_doctype()), with
 * particular "well formedeness" extensions here and there for XML.  It
 * draws from the HTML spec only in defining which elements have an
 * optional or no closing tag (e.g., <p>, <meta>, etc.).  Nodes added to
 * a DOM tree with this interface are NOT semantically validated; you
 * can build any old [syntactically-consistent] tree you like.  The
 * validator is in a different file.
 */

struct	hparse {
	struct hnode	 *root;
	struct hnode	 *current;
	struct iobuf	 *buf;
	int		  line;
	int		  col;
	enum hmode	  hmode;
};

#define	LOWERCASE 2 /* Cf. hparse_name() */
#define	UPPERCASE 1
#define	INHERIT   0

static	int	  hparse_attr(struct hparse *, struct ioctx *);
static	int	  hparse_attrlist(struct hparse *, struct ioctx *);
static	int	  hparse_comment(struct hparse *, struct ioctx *);
static	int	  hparse_ctag(struct hparse *, struct ioctx *);
static	int	  hparse_decl(struct hparse *, struct ioctx *);
static	int	  hparse_doctype(struct hparse *, struct ioctx *);
static	int	  hparse_elem(struct hparse *, struct ioctx *);
static	int	  hparse_end(struct hparse *, struct ioctx *);
static	int	  hparse_ident(struct hparse *, struct ioctx *);
static	int	  hparse_identlist(struct hparse *, struct ioctx *);
static	int	  hparse_lit(struct hparse *, struct ioctx *, char);
static	int	  hparse_name(struct hparse *, struct ioctx *, int);
static	int	  hparse_otag(struct hparse *, struct ioctx *);
static	int	  hparse_otag_xml(struct hparse *, struct ioctx *);
static	int	  hparse_otag_sgml(struct hparse *, struct ioctx *);
static	int	  hparse_proc(struct hparse *, struct ioctx *);
static	int	  hparse_proc_xml(struct hparse *, struct ioctx *);
static	int	  hparse_proc_sgml(struct hparse *, struct ioctx *);
static	int	  hparse_text(struct hparse *, struct ioctx *, int, int);
static	int	  hparse_type(struct hparse *, struct ioctx *);
static	int	  hparse_whitespace(struct hparse *, struct ioctx *);
	  
static	int	  hparse_getchar(struct hparse *, 
			struct ioctx *, char *);
static	int	  hparse_rewchar(struct hparse *, struct ioctx *);
static	int	  hparse_syntax(const struct hparse *, 
			struct ioctx *, enum iosyntax);
static	int	  hparse_token(struct hparse *, 
			struct ioctx *, char *);
static	int	  hparse_warn(const struct hparse *, 
			struct ioctx *, enum iowarn);
static	int	  hparse_xcode(struct hparse *,
			struct ioctx *, char);


/* See html.3. */
struct hparse *
hparse_alloc(enum hmode mode)
{
	struct hparse	*p;

	if (NULL == (p = calloc(1, sizeof(struct hparse))))
		return(NULL);

	if (NULL == (p->buf = iobuf_alloc())) {
		free(p);
		return(NULL);
	}

	p->hmode = mode;
	return(p);
}


/* See html.3. */
void
hparse_delete(struct hparse *p)
{

	assert(p);
	iobuf_free(p->buf);
	free(p);
}


static int
hparse_end(struct hparse *p, struct ioctx *ctx)
{

	/* 
	 * If we haven't rewound to the root, a scope is still open at
	 * the end of file.
	 */
	if (p->root != p->current)
		return(hparse_syntax(p, ctx, IOERR_NEOF));

	return(1);
}


static int
hparse_text(struct hparse *p, struct ioctx *ctx, int line, int col)
{
	struct hnode	*n;

	if (0 == iobuf_size(p->buf))
		return(1);

	iobuf_final(p->buf);
	n = hnode_alloc_chars(iobuf_buf(p->buf), line, col);

	if (NULL == n)
		return(0);

	if ( ! hnode_addchild(p->current, n))
		return(hparse_syntax(p, ctx, IOERR_CHLD));

	return(1);
}


static int
hparse_rewchar(struct hparse *p, struct ioctx *ctx)
{

	/* FIXME: crossing line border! */
	p->col -= 1 == p->col ? 0 : 1;
	return(ioctx_rew(ctx));
}


static int
hparse_getchar(struct hparse *p, struct ioctx *ctx, char *c)
{
	int		rc;

	if (1 != (rc = ioctx_getchar(ctx, c)))
		return(rc);

	p->col++;

	if ('\n' != *c)
		return(1);

	p->col = 1;
	p->line++;
	return(1);
}


static int
hparse_xcode(struct hparse *p, struct ioctx *ctx, char c)
{
	char		*cp;
	size_t		 sz;

	if ( ! ioctx_xcode(ctx, c, &cp, &sz))
		return(0);

	if (NULL == cp) {
		if ( ! iobuf_putchar(p->buf, c))
			return(ioctx_error(ctx, IOERR_MEM));
		return(1);
	}

	if ( ! iobuf_puts(p->buf, cp, sz))
		return(ioctx_error(ctx, IOERR_MEM));

	return(1);
}


static int
hparse_token(struct hparse *p, struct ioctx *ctx, char *c)
{
	int		rc;

	if (-1 == (rc = hparse_getchar(p, ctx, c)))
		return(0);
	else if (0 == rc)
		return(hparse_syntax(p, ctx, IOERR_NEOF));

	return(1);
}


static int
hparse_lit(struct hparse *p, struct ioctx *ctx, char end)
{
	int		 rc;
	char		 c;

	iobuf_reset(p->buf);

	/* SGML ref: attribute value literal: 7.9.3, 331:4 */

	/* LINTED */
	while ((rc = hparse_token(p, ctx, &c)))
		if (end == c)
			break;
		else if ( ! hparse_xcode(p, ctx, c))
			return(0);

	if (0 == rc)
		return(0);

	iobuf_final(p->buf);
	return(1);
}


static int
hparse_attr(struct hparse *p, struct ioctx *ctx)
{
	enum hattrt	 attr;
	struct hattr	*attrp;
	char		 c;
	int		 line, col, lit, cs;

	/* SGML ref: attribute specification: 7.9, 327:19 */

	line = p->line;
	col = p->col;
	cs = /* LINTED */ HMODE_XML & p->hmode ? 
		LOWERCASE : UPPERCASE;

	if ( ! hparse_name(p, ctx, cs))
		return(0);

	/* FIXME: this doesn't handle namespaces. */

	attr = hattr_find(iobuf_buf(p->buf));
	if (HATTR__MAX == attr)
		return(hparse_syntax(p, ctx, IOERR_ATTR));

	if ( ! hparse_whitespace(p, ctx))
		return(0);
	if ( ! hparse_token(p, ctx, &c))
		return(0);

	if ('=' != c) {
		if (HMODE_XML & /* LINTED */ p->hmode)
			return(hparse_syntax(p, ctx, IOERR_BATTR));

		/* HTML ref: Attribute declarations: 3.3.4 */

		attrp = hattr_alloc_chars(attr, NULL, 0, line, col);
		if (NULL == attrp)
			return(ioctx_error(ctx, IOERR_MEM));

		if ( ! hnode_addattr(p->current, attrp))
			return(hparse_syntax(p, ctx, IOERR_CHLD));

		return(hparse_rewchar(p, ctx));
	}

	if ( ! hparse_whitespace(p, ctx))
		return(0);
	if ( ! hparse_token(p, ctx, &c))
		return(0);

	/* SGML ref: attribute value: 7.9.4, 333:1 */

	lit = 1;

	if ('\'' != c && '\"' != c) {
		/* XML ref: Attribute: 40 */

		if (HMODE_XML & /* LINTED */ p->hmode)
			return(hparse_syntax(p, ctx, IOERR_BATTR));

		lit = 0;
		if ( ! hparse_rewchar(p, ctx))
			return(0);
		if ( ! hparse_name(p, ctx, INHERIT))
			return(0);
	} else if ( ! hparse_lit(p, ctx, c))
		return(0);

	attrp = hattr_alloc_chars
		(attr, iobuf_buf(p->buf), lit, line, col);

	if (NULL == attrp)
		return(ioctx_error(ctx, IOERR_MEM));

	if ( ! hnode_addattr(p->current, attrp))
		return(hparse_syntax(p, ctx, IOERR_CHLD));

	return(1);
}


static int
hparse_attrlist(struct hparse *p, struct ioctx *ctx)
{
	char		c;

	/* SGML ref: attribute specification list: 7.9, 327:17 */

	if ( ! hparse_whitespace(p, ctx))
		return(0);

	if ( ! hparse_token(p, ctx, &c))
		return(0);

	/* Note: accomodate for XML/proc tag-end. */

	if ('>' == c || '/' == c || '?' == c)
		return(hparse_rewchar(p, ctx));

	if ( ! hparse_rewchar(p, ctx))
		return(0);
	if ( ! hparse_attr(p, ctx))
		return(0);

	/* Call ourselves again. */

	return(hparse_attrlist(p, ctx));
}


static int
hparse_whitespace(struct hparse *p, struct ioctx *ctx)
{
	int		 rc;
	char		 c;

	/* SGML ref: s: 6.2.1, 297:23 */
	/* XML ref: s: 3 */

	/* LINTED */
	while ((rc = hparse_token(p, ctx, &c)))
		switch (c) {
		case (' '):
			/* FALLTHROUGH */
		case ('\n'):
			/* FALLTHROUGH */
		case ('\r'):
			/* FALLTHROUGH */
		case ('\t'):
			break;
		default:
			return(hparse_rewchar(p, ctx));
		}


	assert(0 == rc);
	return(0);
}


static int
hparse_name(struct hparse *p, struct ioctx *ctx, int cs)
{
	char		 c;
	const char	*cp;
	int		 rc;

	/*
	 * FIXME: this section is barmy in XML, which accepts arbitrary
	 * unicode encodings for the first character.
	 */

	iobuf_reset(p->buf);

	/* SGML ref: name: 9.3, 346:17 */

	/* SGML ref: name start character: 9.2.1, 346:3 */

	if ( ! hparse_token(p, ctx, &c))
		return(0);

	if ( ! isalpha((u_char)c))
		return(hparse_syntax(p, ctx, IOERR_BTAG));

	/* FIXME: XML allows ':' and '_' as first chars! */

	if ( ! iobuf_putchar(p->buf, c))
		return(ioctx_error(ctx, IOERR_MEM));

	/* SGML ref: name character: 9.2.1, 345:8 */

	/* LINTED */
	while ((rc = hparse_token(p, ctx, &c)))
		/* FIXME: SGML does not allow ':' and '_' chars. */
		if ('-' != c && '.' != c && 
				'_' != c && ':' != c &&
				! isalnum((u_char)c))
			break;
		else if ( ! iobuf_putchar(p->buf, c))
			return(ioctx_error(ctx, IOERR_MEM));

	if (0 == rc)
		return(0);

	iobuf_final(p->buf);

	if (INHERIT == cs)
		return(hparse_rewchar(p, ctx));

	/* Warn/enforce letter case. */

	for (cp = iobuf_buf(p->buf); '\0' != *cp; cp++) {
		if ( ! isalpha((u_char)*cp))
			continue;
		if (UPPERCASE == cs && isupper((u_char)*cp))
			continue;
		if (LOWERCASE == cs && islower((u_char)*cp))
			continue;
		if ( ! hparse_warn(p, ctx, IOERR_UPPER))
			return(0);
		break;
	}

	return(hparse_rewchar(p, ctx));
}


static int
hparse_elem(struct hparse *p, struct ioctx *ctx)
{
	char		 c;

	/* SGML ref: element: 7.3, 308:1 */
	/* XML ref: STag: 40 */

	if ( ! hparse_token(p, ctx, &c))
		return(0);

	if ('!' == c)
		return(hparse_decl(p, ctx));
	if ('/' == c)
		return(hparse_ctag(p, ctx));
	if ('?' == c)
		return(hparse_proc(p, ctx));

	if ( ! hparse_rewchar(p, ctx))
		return(0);
	return(hparse_otag(p, ctx));
}


static int
hparse_proc(struct hparse *p, struct ioctx *ctx)
{
	enum hproct	 type;
	struct hnode	*n;
	int		 line, col;

	line = p->line;
	col = p->col;

	/* SGML ref: processing instruction: 8, 339:1 */

	if ( ! hparse_name(p, ctx, LOWERCASE))
		return(0);

	type = hproc_find(iobuf_buf(p->buf));

	if (HPROC__MAX == type)
		return(hparse_syntax(p, ctx, IOERR_PROC));

	n = hnode_alloc_proc(type, line, col);
	if (NULL == n)
		return(ioctx_error(ctx, IOERR_MEM));

	if ( ! hnode_addchild(p->current, n))
		return(hparse_syntax(p, ctx, IOERR_CHLD));

	p->current = n;

	if ( ! hparse_attrlist(p, ctx))
		return(0);

	p->current = hnode_parent(p->current);

	if (HMODE_XML & /* LINTED */ p->hmode)
		return(hparse_proc_xml(p, ctx));

	return(hparse_proc_sgml(p, ctx));
}


static int
hparse_proc_sgml(struct hparse *p, struct ioctx *ctx)
{
	char		 c;

	if ( ! hparse_token(p, ctx, &c))
		return(0);
	if ('>' != c)
		return(hparse_syntax(p, ctx, IOERR_BPROC));
	return(1);
}


static int
hparse_proc_xml(struct hparse *p, struct ioctx *ctx)
{
	char		 c;

	if ( ! hparse_token(p, ctx, &c))
		return(0);
	if ('?' != c)
		return(hparse_syntax(p, ctx, IOERR_BPROC));
	if ( ! hparse_token(p, ctx, &c))
		return(0);
	if ('>' != c)
		return(hparse_syntax(p, ctx, IOERR_BPROC));
	return(1);
}


static int
hparse_decl(struct hparse *p, struct ioctx *ctx)
{
	char		 c;

	/* 
	 * Handling SGML declarations for comments (10.3, 391:1),
	 * document types (11.1, 403:1), and link types (12.1, 434:1).
	 */

	if ( ! hparse_token(p, ctx, &c))
		return(0);

	if ('-' == c) {
		if ( ! hparse_token(p, ctx, &c))
			return(0);
		if ('-' != c)
			return(hparse_syntax(p, ctx, IOERR_DECL));
		return(hparse_comment(p, ctx));
	}

	if ( ! hparse_rewchar(p, ctx))
		return(0);

	return(hparse_type(p, ctx));
}


static int
hparse_comment(struct hparse *p, struct ioctx *ctx)
{
	struct hnode	*n;
	char		 c;
	int		 rc, i, line, col;

	/* SGML ref: comment declaration: (10.3, 391:1) */

	/*
	 * Note that we don't really handle SGML comments, just the HTML
	 * suggested ones, which are basically <!--[.]*--[:ws:]*>.
	 */

	iobuf_reset(p->buf);
	line = p->line;
	col = p->col;

	i = 0;

	/* LINTED */
	while ((rc = hparse_token(p, ctx, &c))) {
		if ('-' == c && 2 == ++i) 
			break;
		else if ('-' == c)
			continue;

		/* Append the '-' we skipped over. */

		if (i > 0 && ! iobuf_putchar(p->buf, '-'))
			return(ioctx_error(ctx, IOERR_MEM));

		i = 0;
		
		if ( ! hparse_xcode(p, ctx, c))
			return(0);
	}

	if (0 == rc)
		return(0);

	if ( ! hparse_whitespace(p, ctx))
		return(0);

	if ( ! hparse_token(p, ctx, &c))
		return(0);

	if ('>' != c)
		return(hparse_syntax(p, ctx, IOERR_SGML));

	iobuf_final(p->buf);
	
	n = hnode_alloc_comment(iobuf_buf(p->buf), line, col);
	if (NULL == n)
		return(ioctx_error(ctx, IOERR_MEM));

	if ( ! hnode_addchild(p->current, n))
		return(hparse_syntax(p, ctx, IOERR_CHLD));

	return(1);
}


static int
hparse_ident(struct hparse *p, struct ioctx *ctx)
{
	char		 c;
	struct hident	*identp;
	int		 line, col, lit;

	iobuf_reset(p->buf);

	line = p->line;
	col = p->col;
	lit = 1;

	if ( ! hparse_token(p, ctx, &c))
		return(0);

	if ('\'' != c && '\"' != c) {
		lit = 0;

		if ( ! hparse_rewchar(p, ctx))
			return(0);
		if ( ! hparse_name(p, ctx, 0))
			return(0);
	} else if ( ! hparse_lit(p, ctx, c))
		return(0);

	identp = hident_alloc
		(iobuf_buf(p->buf), lit, line, col);

	if (NULL == identp)
		return(ioctx_error(ctx, IOERR_MEM));

	if ( ! hnode_addident(p->current, identp))
		return(hparse_syntax(p, ctx, IOERR_CHLD));

	return(1);
}


static int
hparse_identlist(struct hparse *p, struct ioctx *ctx)
{
	char		c;

	if ( ! hparse_whitespace(p, ctx))
		return(0);
	if ( ! hparse_token(p, ctx, &c))
		return(0);

	if ('>' == c)
		return(hparse_rewchar(p, ctx));
	if ( ! hparse_rewchar(p, ctx))
		return(0);

	if ( ! hparse_ident(p, ctx))
		return(0);

	/* Call ourselves again. */

	return(hparse_identlist(p, ctx));
}


static int
hparse_doctype(struct hparse *p, struct ioctx *ctx)
{
	char		 c;
	struct hident	*identp;
	int		 line, col, cs;

	if ( ! hparse_whitespace(p, ctx))
		return(0);

	line = p->line;
	col = p->col;

	/* SGML ref: document type name: 11.1, 404:4 */

	/* XML ref: doctypedecl: 28 */
	
	cs = HMODE_XML & /* LINTED */ p->hmode ? 
		LOWERCASE : UPPERCASE;
	if ( ! hparse_name(p, ctx, cs))
		return(0);

	/* NB: only allow HTML for now. */

	if (strcasecmp("HTML", iobuf_buf(p->buf)))
		return(hparse_syntax(p, ctx, IOERR_DTYPE));

	/* Force the HTML declaration name. */

	identp = hident_alloc(iobuf_buf(p->buf), 0, line, col);

	if (NULL == identp)
		return(ioctx_error(ctx, IOERR_MEM));

	if ( ! hnode_addident(p->current, identp))
		return(hparse_syntax(p, ctx, IOERR_CHLD));

	/* SGML ref: external identifiers: 10.1.6, 379:1 */

	if ( ! hparse_identlist(p, ctx))
		return(0);
	if ( ! hparse_token(p, ctx, &c))
		return(0);
	if ('>' != c)
		return(hparse_syntax(p, ctx, IOERR_BDECL));

	return(1);
}


static int
hparse_type(struct hparse *p, struct ioctx *ctx)
{
	enum hdeclt	 type;
	struct hnode	*n;
	int		 line, col;

	line = p->line;
	col = p->col;

	/* SGML ref: document type declaration (11.1, 403:1) */

	if ( ! hparse_name(p, ctx, UPPERCASE))
		return(0);

	type = hdecl_find(iobuf_buf(p->buf));

	if (HDECL__MAX == type)
		return(hparse_syntax(p, ctx, IOERR_DECL));

	n = hnode_alloc_decl(type, line, col);

	if (NULL == n)
		return(ioctx_error(ctx, IOERR_MEM));

	if ( ! hnode_addchild(p->current, n))
		return(hparse_syntax(p, ctx, IOERR_CHLD));

	p->current = n;

	/* NB: only need to worry about doctype for now. */

	assert(HDECL_DOCTYPE == type);
	if ( ! hparse_doctype(p, ctx))
		return(0);

	p->current = hnode_parent(p->current);
	return(1);
}


static int
hparse_ctag(struct hparse *p, struct ioctx *ctx)
{
	enum helemt	 elem;
	char		 c;
	int		 cs;

	/* We can only close out element nodes. */

	cs = HMODE_XML & /* LINTED */ p->hmode ? 
		LOWERCASE : UPPERCASE;

	if (HNODE_ELEM != hnode_type(p->current))
		return(hparse_syntax(p, ctx, IOERR_NCTX));

	/* SGML ref: end-tag: 7.5, 317:4 */

	if ( ! hparse_name(p, ctx, cs))
		return(0);

	elem = helem_find(iobuf_buf(p->buf));
	if (HELEM__MAX == elem)
		return(hparse_syntax(p, ctx, IOERR_TAG));

	if ( ! hparse_whitespace(p, ctx))
		return(0);

	if ( ! hparse_token(p, ctx, &c))
		return(0);

	if ('>' != c)
		return(hparse_syntax(p, ctx, IOERR_BTAG));

	if (elem != hnode_elem(p->current))
		return(hparse_syntax(p, ctx, IOERR_NCTX));

	p->current = hnode_parent(p->current);
	assert(p->current);

	return(1);
}


static int
hparse_otag(struct hparse *p, struct ioctx *ctx)
{
	enum helemt	 elem;
	struct hnode	*n;
	int		 line, col, cs;

	/* SGML ref: start-tag: 7.4, 314:1 */

	line = p->line;
	col = p->col;
	cs = HMODE_XML & /* LINTED */ p->hmode ? 
		LOWERCASE : UPPERCASE;

	if ( ! hparse_name(p, ctx, cs))
		return(0);

	/* FIXME: this doesn't support namespaces! */

	elem = helem_find(iobuf_buf(p->buf));

	if (HELEM__MAX == elem)
		return(hparse_syntax(p, ctx, IOERR_TAG));

	n = hnode_alloc_elem(elem, line, col);
	if (NULL == n)
		return(ioctx_error(ctx, IOERR_MEM));

	if ( ! hnode_addchild(p->current, n))
		return(hparse_syntax(p, ctx, IOERR_CHLD));

	p->current = n;

	if ( ! hparse_attrlist(p, ctx))
		return(0);

	return(HMODE_XML & /* LINTED */ p->hmode ?
			hparse_otag_xml(p, ctx) : 
			hparse_otag_sgml(p, ctx));
}


static int
hparse_otag_sgml(struct hparse *p, struct ioctx *ctx)
{
	char		 c;

	if ( ! hparse_token(p, ctx, &c))
		return(0);
	if ('>' != c)
		return(hparse_syntax(p, ctx, IOERR_BTAG));

	if (helem_empty(hnode_elem(p->current), p->hmode))
		p->current = hnode_parent(p->current);

	return(1);
}


static int
hparse_otag_xml(struct hparse *p, struct ioctx *ctx)
{
	char		 c;

	if ( ! hparse_token(p, ctx, &c))
		return(0);

	/* XML ref: EmptyElemTag: 44 */

	if ('>' == c)
		return(1);
	else if ('/' != c)
		return(hparse_syntax(p, ctx, IOERR_BTAG));

	if ( ! hparse_token(p, ctx, &c))
		return(0);
	if ('>' != c)
		return(hparse_syntax(p, ctx, IOERR_BTAG));

	p->current = hnode_parent(p->current);
	return(1);
}


int
hparse_tree(struct hparse *p, struct ioctx *ctx, struct hnode **res)
{
	struct hnode	*n;
	int		 rc, line, col;
	char		 c;

	p->root = p->current = *res = NULL;
	p->line = p->col = 1;

	if ( ! ioctx_open(ctx))
		return(0);

	iobuf_reset(p->buf);

	n = hnode_alloc_root(p->line, p->col);
	if (NULL == n)
		return(ioctx_error(ctx, IOERR_MEM));

	p->root = p->current = *res = n;

	/* SGML ref: sgml document: 6.1, 294:5 */

	if ( ! hparse_whitespace(p, ctx))
		return(0);

	/* 
	 * We jump directly into text/tags.  This is not necessarily as
	 * per the SGML specification, which says that we begin with
	 * SGML document entity (6.2, 295:13).  Instead we jump right
	 * into what follows.
	 */

	iobuf_reset(p->buf);

	line = p->line;
	col = p->col;

	/* 
	 * Assume we're in a text node: character data entity (6.3,
	 * 300:1), specific data entity (6.3, 300:4), or non-SGML data
	 * entity (6.3, 300:7).
	 */

	while (1 == (rc = hparse_getchar(p, ctx, &c))) {
		/*
		 * The '<' indicates a document type declaration (11.1,
		 * 403:1), link type declaration (12.1, 434:1), comment
		 * (10.3, 391:1), processing instruction (8, 339:1), or
		 * element (7.3, 308:1).
		 */
		if ('<' == c) {
			/* 
			 * First close out existing text. 
			 */
			if ( ! hparse_text(p, ctx, line, col))
				rc = -1;
			else if ( ! hparse_elem(p, ctx))
				rc = -1;
			else {
				line = p->line;
				col = p->col;
				continue;
			}
			break;
		}

		/* See above loop for comments. */

		if (hparse_xcode(p, ctx, c)) 
			continue;

		rc = -1;
		break;
	}

	/* 
	 * End of file or error has been reached.  If end of file, close
	 * out the existing text.
	 */

	if (-1 != rc && ! hparse_text(p, ctx, line, col))
		rc = -1;

	if ( ! ioctx_close(ctx))
		return(0);

	/* Validate the tree, if necessary. */

	return(-1 == rc ? 0 : hparse_end(p, ctx));
}


static int
hparse_syntax(const struct hparse *p, 
		struct ioctx *ctx, enum iosyntax err)
{

	return(ioctx_syntax(ctx, p->line, p->col, err));
}


static int
hparse_warn(const struct hparse *p, 
		struct ioctx *ctx, enum iowarn err)
{

	return(ioctx_warn(ctx, p->line, p->col, err));
}
