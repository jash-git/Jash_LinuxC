/*	$Id: node.c,v 1.37 2011/07/06 15:21:58 kristaps Exp $ */
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
#include <sys/queue.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "html.h"
#include "extern.h"

/*
 * Builds up the syntactic SGML or XML tree.  This contains no semantic
 * rules, but enforces minimum syntactic rules, such as comments not
 * having non-text children and so on.
 */

TAILQ_HEAD(hattrq, hattr);

TAILQ_HEAD(hnodeq, hnode);

TAILQ_HEAD(hidentq, hident);

struct	hattr {
	enum hattrt	 type;		/* type of attribute */
	char		*value;		/* value string (NULL-ok) */
	struct hnode	*parent;	/* parent element */
	int		 literal;	/* whether literal */
	int		 line;
	int		 col;

	TAILQ_ENTRY(hattr) entries;
};

struct	hnode {
	enum hnodet	  type;		/* type of node */
	struct hnode	 *parent;	/* parent of node */
	struct hnodeq	  children;	/* children of node */
	struct hattrq	  attrs;	/* HNODE_ELEM/HNODE_PROC */
	struct hidentq	  idents;	/* HNODE_DECL */
	enum helemt	  elem;		/* HNODE_ELEM */
	enum hproct	  proc;		/* HNODE_PROC */
	char		 *text;		/* HNODE_TEXT/HNODE_COMMENT */
	enum hdeclt	  decl;		/* HNODE_DECL */
	int		  line;
	int		  col;
	struct hcnode	 *cache;	/* back-ref for caching */

	TAILQ_ENTRY(hnode) entries;
};

struct	hident {
	char		*value;		/* identifier value */
	struct hnode	*parent;	/* parent of identifier */
	int		 literal;	/* is literal string */
	int		 line;
	int	 	 col;

	TAILQ_ENTRY(hident) entries;
};


struct	hcnode {
	int		 id;
	char		*name;
	struct hnode	*n;
};

struct	hcache {
	struct hnode	*n;
	struct hcnode	*ids;
	int		 maxid;
};


static	char		*alloc_escaped(const char *);
static	void	  	 hattr_free(struct hattr *);
static	void	  	 hattr_unlink(struct hattr *);
static	void	  	 hident_free(struct hident *);
static	void	  	 hident_unlink(struct hident *);
static	struct hnode 	*hnode_alloc(enum hnodet, int, int);
static	void		 hnode_cache(struct hnode *, struct hcache *);
static	struct hnode	*hnode_cclone(struct hnode *, 
				struct hcache *, struct hcache *);
static	int		 hnode_dechildpart_r(struct hnode *, 
				const struct hnode * const *, int);
static	void	  	 hnode_free(struct hnode *);
static	void	  	 hnode_unlink(struct hnode *);


static struct hnode *
hnode_alloc(enum hnodet type, int line, int col)
{
	struct hnode	*p;

	p = calloc(1, sizeof(struct hnode));
	if (NULL == p)
		return(NULL);

	p->type = type;
	p->line = line;
	p->col = col;

	TAILQ_INIT(&p->children);
	TAILQ_INIT(&p->attrs);
	TAILQ_INIT(&p->idents);
	return(p);
}


static char *
alloc_escaped(const char *text)
{
	const char	*frag;
	char		*cp, *pp;
	size_t		 ssz, sz;
	int		 pos;

	sz = strlen(text) + 1;
	pp = malloc(sz);

	if (NULL == pp)
		return(NULL);

	for (frag = NULL, pos = 0; '\0' != *text; text++) {
		switch (*text) {
		case ('<'):
			frag = "&lt;";
			break;
		case ('>'):
			frag = "&gt;";
			break;
		case ('\"'):
			frag = "&quot;";
			break;
		case ('&'):
			frag = "&amp;";
			break;
		default:
			break;
		}

		if (NULL == frag) {
			pp[pos++] = *text;
			continue;
		}

		ssz = strlen(frag);
		assert(ssz > 1);
		sz += ssz - 1;

		if (NULL == (cp = realloc(pp, sz))) {
			free(pp);
			return(NULL);
		}

		pp = cp;
		memcpy(&pp[pos], frag, ssz);
		frag = NULL;
		pos += (int)ssz;
	}

	pp[pos] = '\0';
	return(pp);
}


const char *
hnode_comment(struct hnode *p)
{

	assert(HNODE_COMMENT == p->type);
	return(p->text);
}


const char *
hnode_text(struct hnode *p)
{

	assert(HNODE_TEXT == p->type);
	return(p->text);
}


const char *
hattr_value(struct hattr *p)
{

	return(p->value);
}


enum hattrt
hattr_type(struct hattr *p)
{

	return(p->type);
}


struct hnode *
hnode_alloc_decl(enum hdeclt t, int line, int col)
{
	struct hnode	*n;

	n = hnode_alloc(HNODE_DECL, line, col);
	if (NULL == n)
		return(NULL);

	n->decl = t;
	return(n);
}


struct hnode *
hnode_alloc_comment(const char *comment, int line, int col)
{
	struct hnode	*p;

	p = hnode_alloc(HNODE_COMMENT, line, col);
	if (NULL == p)
		return(NULL);

	if (NULL != (p->text = strdup(comment)))
		return(p);

	free(p);
	return(NULL);
}


struct hnode *
hnode_alloc_elem(enum helemt type, int line, int col)
{
	struct hnode	*p;

	p = hnode_alloc(HNODE_ELEM, line, col);
	if (NULL == p)
		return(NULL);

	p->elem = type;
	return(p);
}


struct hnode *
hnode_alloc_proc(enum hproct t, int line, int col)
{
	struct hnode	*p;

	p = hnode_alloc(HNODE_PROC, line, col);
	if (NULL == p)
		return(NULL);

	p->proc = t;
	return(p);
}


struct hnode *
hnode_alloc_root(int line, int col)
{

	return(hnode_alloc(HNODE_ROOT, line, col));
}


struct hnode *
hnode_alloc_chars(const char *text, int line, int col)
{
	struct hnode	*p;

	p = hnode_alloc(HNODE_TEXT, line, col);
	if (NULL == p)
		return(NULL);

	if (NULL != (p->text = strdup(text))) 
		return(p);

	free(p);
	return(NULL);
}


struct hnode *
hnode_alloc_text(const char *text, int line, int col)
{
	struct hnode	*p;

	if (NULL == (p = hnode_alloc(HNODE_TEXT, line, col)))
		return(NULL);
	if (NULL != (p->text = alloc_escaped(text)))
		return(p);

	hnode_free(p);
	return(NULL);
}


void
hnode_delete(struct hnode *p)
{
	struct hnode	*pp;

	while (NULL != (pp = TAILQ_FIRST(&p->children)))
		hnode_delete(pp);

	hnode_unlink(p);
	hnode_free(p);
}


static void
hnode_free(struct hnode *p)
{
	struct hattr	*attr;
	struct hident	*ident;

	assert(p);
	while (NULL != (attr = TAILQ_FIRST(&p->attrs))) 
		hattr_delete(attr);
	while (NULL != (ident = TAILQ_FIRST(&p->idents))) 
		hident_delete(ident);

	if (p->text)
		free(p->text);

	free(p);
}


static void
hnode_unlink(struct hnode *p)
{

	assert(p);
	if (NULL == p->parent)
		return;

	TAILQ_REMOVE(&p->parent->children, p, entries);
	p->parent = NULL;
	if (p->cache)
		p->cache->n = NULL;
}


struct hident *
hnode_ident(struct hnode *p)
{

	return(TAILQ_FIRST(&p->idents));
}


struct hattr *
hnode_attr(struct hnode *p)
{

	return(TAILQ_FIRST(&p->attrs));
}


struct hnode *
hnode_child(struct hnode *p)
{

	return(TAILQ_FIRST(&p->children));
}

static int
hnode_dechildpart_r(struct hnode *p, const struct hnode * const *rest, int rsz)
{
	struct hnode	*pp;
	int		 i, rc;

	for (i = 0; i < rsz; i++)
		if (rest[i] == p)
			return(0);

	rc = 1;
	while (NULL != (pp = TAILQ_FIRST(&p->children)))
		if ( ! hnode_dechildpart_r(pp, rest, rsz))
			rc = 0;

	if (rc > 0) {
		hnode_unlink(p);
		hnode_free(p);
	}

	return(rc);
}

void
hnode_dechildpart(struct hnode *p, const struct hnode * const *rest, int rsz)
{

	while (hnode_child(p))
		hnode_dechildpart_r(hnode_child(p), rest, rsz);
}

void
hnode_dechild(struct hnode *p)
{

	while (hnode_child(p))
		hnode_delete(hnode_child(p));
}


struct hnode *
hnode_sibling(struct hnode *p)
{

	return(TAILQ_NEXT(p, entries));
}


enum hnodet
hnode_type(struct hnode *p)
{
	
	return(p->type);
}


struct hnode *
hnode_parent(struct hnode *p)
{

	return(p->parent);
}


enum hdeclt
hnode_decl(struct hnode *p)
{

	assert(HNODE_DECL == p->type);
	return(p->decl);
}


enum helemt
hnode_elem(struct hnode *p)
{

	assert(HNODE_ELEM == p->type);
	return(p->elem);
}


enum hproct
hnode_proc(struct hnode *p)
{

	assert(HNODE_PROC == p->type);
	return(p->proc);
}


int
hnode_addchild(struct hnode *p, struct hnode *c)
{

	/* Disallow non-element (or non-root) parents. */

	if (HNODE_ELEM != p->type && HNODE_ROOT != p->type)
		return(0);

	/* 
	 * Disallow processing instruction and declarations as children
	 * of anything but the root.
	 */

	if (HNODE_PROC == c->type || HNODE_DECL == c->type)
		if (HNODE_ROOT != p->type)
			return(0);

	/* Disallow the root as the child of anything. */

	if (HNODE_ROOT == c->type)
		return(0);

	hnode_unlink(c);

	TAILQ_INSERT_TAIL(&p->children, c, entries);
	c->parent = p;
	return(1);
}



struct hident *
hident_sibling(struct hident *p)
{

	return(TAILQ_NEXT(p, entries));
}


struct hattr *
hattr_sibling(struct hattr *p)
{

	return(TAILQ_NEXT(p, entries));
}


struct hattr *
hattr_clone(struct hattr *p)
{

	assert(p);
	return(hattr_alloc_chars
			(p->type, p->value, p->literal, 
			 p->line, p->col));
}


struct hattr *
hattr_alloc_text(enum hattrt type, const char *value, 
		int literal, int line, int col)
{
	struct hattr	*p;

	p = calloc(1, sizeof(struct hattr));
	if (NULL == p)
		return(NULL);

	p->type = type;
	p->line = line;
	p->col = col;
	p->literal = literal;

	if (NULL == value)
		return(p);
	if (NULL != (p->value = alloc_escaped(value)))
		return(p);

	free(p);
	return(NULL);
}


struct hattr *
hattr_alloc_chars(enum hattrt type, const char *value, 
		int literal, int line, int col)
{
	struct hattr	*p;

	p = calloc(1, sizeof(struct hattr));
	if (NULL == p)
		return(NULL);

	p->type = type;
	p->line = line;
	p->col = col;
	p->literal = literal;

	if (NULL == value)
		return(p);

	p->value = strdup(value);
	if (NULL == p->value) {
		free(p);
		return(NULL);
	}
	return(p);
}


void
hattr_delete(struct hattr *p)
{

	assert(p);
	hattr_unlink(p);
	hattr_free(p);
}


static void
hattr_unlink(struct hattr *p)
{

	assert(p);
	if (NULL == p->parent)
		return;
	TAILQ_REMOVE(&p->parent->attrs, p, entries);
	p->parent = NULL;
}


int
hnode_repattr(struct hnode *p, struct hattr *c)
{
	struct hattr	*cp;

	if (HNODE_ELEM != p->type && HNODE_PROC != p->type)
		return(0);

	TAILQ_FOREACH(cp, &p->attrs, entries)
		if (hattr_type(cp) == hattr_type(c)) {
			hattr_delete(cp);
			break;
		}

	return(hnode_addattr(p, c));
}


int
hnode_addattr(struct hnode *p, struct hattr *c)
{

	/* 
	 * Disallow anything but element or processing instruction
	 * parents. 
	 */
	if (HNODE_ELEM != p->type && HNODE_PROC != p->type)
		return(0);

	hattr_unlink(c);
	c->parent = p;
	TAILQ_INSERT_TAIL(&p->attrs, c, entries);
	return(1);
}


static void
hattr_free(struct hattr *p)
{

	assert(p);
	if (p->value)
		free(p->value);
	free(p);
}


int
hnode_column(struct hnode *p)
{

	return(p->col);
}


int
hnode_line(struct hnode *p)
{

	return(p->line);
}


int
hident_column(struct hident *p)
{

	return(p->col);
}


int
hattr_column(struct hattr *p)
{

	return(p->col);
}


int
hattr_literal(struct hattr *p)
{

	return(p->literal);
}


int
hattr_line(struct hattr *p)
{

	return(p->line);
}


int
hident_line(struct hident *p)
{

	return(p->line);
}


struct hnode *
hattr_parent(struct hattr *p)
{

	return(p->parent);
}


struct hident *
hident_clone(struct hident *p)
{

	assert(p);
	return(hident_alloc(p->value, p->literal, p->line, p->col));
}


struct hident *
hident_alloc(const char *val, int literal, int line, int col)
{
	struct hident	*p;

	p = calloc(1, sizeof(struct hident));
	if (NULL == p)
		return(NULL);

	p->value = strdup(val);
	if (NULL == p->value) {
		free(p);
		return(NULL);
	}

	p->line = line;
	p->col = col;
	p->literal = literal;

	return(p);
}


static void
hident_free(struct hident *p)
{

	assert(p);
	if (p->value)
		free(p->value);
	free(p);
}


void
hident_delete(struct hident *p)
{

	assert(p);
	hident_unlink(p);
	hident_free(p);
}


static void
hident_unlink(struct hident *p)
{

	assert(p);
	if (NULL == p->parent)
		return;
	TAILQ_REMOVE(&p->parent->idents, p, entries);
	p->parent = NULL;
}


const char *
hident_value(struct hident *p)
{

	assert(p);
	return(p->value);
}


int
hident_literal(struct hident *p)
{

	assert(p);
	return(p->literal);
}


int
hnode_addident(struct hnode *p, struct hident *c)
{

	/* Disallow anything but a declaration as parent. */

	if (HNODE_DECL != p->type)
		return(0);

	hident_unlink(c);
	c->parent = p;
	TAILQ_INSERT_TAIL(&p->idents, c, entries);
	return(1);
}


static struct hnode *
hnode_cclone(struct hnode *p, struct hcache *old, struct hcache *new)
{
	struct hnode	*c, *cp, *np;
	struct hattr	*ap, *nap;
	struct hident	*ip, *nip;
	int		 rc, id;

	c = hnode_alloc(p->type, p->line, p->col);
	if (NULL == c)
		return(NULL);

	c->elem = p->elem;
	c->decl = p->decl;

	if (p->text) {
		c->text = strdup(p->text);
		if (NULL == c->text) {
			free(c);
			return(NULL);
		}
	}

	TAILQ_FOREACH(ap, &p->attrs, entries) {
		/* 
		 * We DO NOT copy over the ID attribute, as it must be
		 * unique within a document.  This is a crucial part of
		 * this function.
		 */
		if (HATTR_ID == ap->type && new) {
			/* Handle dupes. */
			if (NULL == p->cache)
				continue;

			id = p->cache->id;
			assert(new->maxid > id);
			new->ids[id].n = c;
			c->cache = &new->ids[id];
			continue;
		} else if (HATTR_ID == ap->type)
			continue;

		nap = hattr_clone(ap);
		if (NULL == nap) {
			hnode_delete(c);
			return(NULL);
		}
		rc = hnode_addattr(c, nap);
		assert(rc);
	}

	TAILQ_FOREACH(ip, &p->idents, entries) {
		nip = hident_clone(ip);
		if (NULL == nip) {
			hnode_delete(c);
			return(NULL);
		}
		rc = hnode_addident(c, nip);
		assert(rc);
	}

	TAILQ_FOREACH(cp, &p->children, entries) {
		np = hnode_cclone(cp, old, new);
		if (NULL == np) {
			hnode_delete(c);
			return(NULL);
		}
		rc = hnode_addchild(c, np);
		assert(rc);
	}

	return(c);
}


struct hnode *
hnode_clone(struct hnode *p)
{

	return(hnode_cclone(p, NULL, NULL));
}


struct hnode *
hcache_root(struct hcache *p)
{

	return(p->n);
}


struct hnode *
hcache_get(struct hcache *p, int id)
{

	assert(id < p->maxid);
	assert(p->ids[id].n);
	return(p->ids[id].n);
}


int
hcache_verify(struct hcache *p)
{
	int		 i;

	for (i = 0; i < p->maxid; i++)
		if (NULL == p->ids[i].n)
			return(0);

	return(1);
}


void
hcache_delete(struct hcache *p)
{
	int		 i;

	if (p->n)
		hnode_delete(p->n);

	if (p->ids) {
		assert(p->maxid);
		for (i = 0; i < p->maxid; i++)
			if (p->ids[i].name)
				free(p->ids[i].name);
		free(p->ids);
	} else
		assert(0 == p->maxid);

	free(p);
}


struct hcache *
hcache_clone(struct hcache *hp)
{
	struct hcache	*p;
	int		 i;

	p = calloc(1, sizeof(struct hcache));
	if (NULL == p)
		return(p);

	if (0 == (p->maxid = hp->maxid)) {
		/* No cached entries to clone. */
		p->n = hnode_clone(hp->n);
		if (NULL == p->n) {
			free(p);
			return(NULL);
		}
		return(p);
	}

	p->ids = calloc((size_t)hp->maxid, sizeof(struct hcnode));
	if (NULL == p->ids) {
		free(p);
		return(NULL);
	}

	for (i = 0; i < p->maxid; i++)
		p->ids[i].id = i;

	/* 
	 * We don't care about the identifiers, as we're just going to
	 * trigger copying the cloned node if it's already cached.
	 */

	p->n = hnode_cclone(hp->n, hp, p);
	if (NULL == p->n) {
		hcache_delete(p);
		return(NULL);
	}
	return(p);
}


struct hcache *
hcache_alloc(struct hnode *n, const struct hcid *id, 
		int maxid, size_t idsz)
{
	struct hcache	*p;
	int		 i, j;

	p = calloc(1, sizeof(struct hcache));
	if (NULL == p)
		return(p);

	p->n = n;

	if (0 == (p->maxid = maxid))
		return(p);

	p->ids = calloc((size_t)maxid, sizeof(struct hcnode));
	if (NULL == p->ids) {
		free(p);
		return(NULL);
	}

	for (i = 0; i < maxid; i++)
		p->ids[i].id = i;

	for (i = 0; i < (int)idsz; i++) {
		j = id[i].id;
		assert(j < maxid);
		assert(p->ids[j].id == i);
		p->ids[j].name = strdup(id[i].name);
		if (p->ids[i].name)
			continue;

		hcache_delete(p);
		return(NULL);
	}

	hnode_cache(n, p);
	return(p);
}


static void
hnode_cache(struct hnode *n, struct hcache *hp)
{
	struct hattr	*a;
	int		 i;
	const char	*cp;

	if (hnode_child(n))
		hnode_cache(hnode_child(n), hp);

	if (HNODE_ELEM == hnode_type(n)) {
		for (a = hnode_attr(n); a; a = hattr_sibling(a)) {
			if (HATTR_ID != hattr_type(a))
				continue;
			cp = hattr_value(a);
			for (i = 0; i < hp->maxid; i++) {
				if (NULL == hp->ids[i].name)
					continue;
				if (strcmp(hp->ids[i].name, cp))
					continue;
				
				/* Handle bogus multi-ids. */
				if (hp->ids[i].n) {
					assert(hp->ids[i].n->cache);
					hp->ids[i].n->cache->n = NULL;
				}
				assert(NULL == n->cache);
				hp->ids[i].n = n;
				n->cache = &hp->ids[i];
				break;
			}
		}
	}

	if (hnode_sibling(n))
		hnode_cache(hnode_sibling(n), hp);
}
