/*	$Id: test.c,v 1.30 2011/07/05 10:04:54 kristaps Exp $ */
/*
 * Copyright (c) 2009, 2010, 2011 Kristaps Dzonsons <kristaps@bsd.lv>
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
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "html.h"

static	void	  fd_error(const char *, void *);
static	void	  fd_perror(const char *, void *);
static	void	  fd_syntax(const char *, int, int, void *);
static	int	  fd_warn(const char *, int, int, void *);
static	void	  usage(void);
static	int	  val_attr(struct hattr *, void *);
static	void	  val_battr(struct hattr *, const char *, void *);
static	int	  val_ident(struct hident *, void *);
static	int	  val_node(struct hnode *, void *);
static	void	  val_bnode(struct hnode *, const char *, void *);
static	void	  val_elem(struct hnode *, struct iofd *);
static	void	  val_comm(struct hnode *, struct iofd *);
static	void	  val_decl(struct hnode *, struct iofd *);
static	void	  val_proc(struct hnode *, struct iofd *);
static	void	  val_root(struct hnode *, struct iofd *);
static	void	  val_text(struct hnode *, struct iofd *);

/*
 * This is an example for libhtml (http://bsd.lv/~kristaps/libhtml).
 * Iterate through the command-line arguments, read them as HTML
 * documents, parse them, then output them back to the screen.
 */
int
main(int argc, char *argv[])
{
	int		 c;
	struct iofd	 fd;
	struct hcid	 id;
	struct hcache	*cache, *cclone;
	struct hnode	*root;
	struct hparse	*parse;
	struct hwrite	*writer;
	struct hvalid	*validator;
	struct ioctx	 ctx;
	struct ioout	 out;
	struct iovalid	 valid;
	enum hmode	 inmode, outmode;

	extern int	 optind;

	inmode = outmode = HTML_4_01;

	while (-1 != (c = getopt(argc, argv, "VxX")))
		switch (c) {
		case ('x'):
			inmode = XHTML_1_0;
			break;
		case ('X'):
			outmode = XHTML_1_0;
			break;
		case ('V'):
			printf("libhtml %s\n", LIBHTML_VER);
			return(EXIT_SUCCESS);
		default:
			usage();
			return(EXIT_FAILURE);
		}

	argc -= optind;
	argv += optind;

	/* Initialise output context. */

	memset(&out, 0, sizeof(struct ioout));

	out.puts = iostdout_puts;
	out.putchar = iostdout_putchar;

	/* Initialise input context. */

	memset(&ctx, 0, sizeof(struct ioctx));

	ctx.open = iofd_open;
	ctx.getchar = iofd_getchar;
	ctx.close = iofd_close;
	ctx.rew = iofd_rew;
	ctx.error = fd_error;
	ctx.syntax = fd_syntax;
	ctx.warn = fd_warn;
	ctx.arg = &fd;

	/* Initialise validation. */

	memset(&valid, 0, sizeof(struct iovalid));

	valid.valid_attr = val_attr;
	valid.valid_battr = val_battr;
	valid.valid_ident = val_ident;
	valid.valid_node = val_node;
	valid.valid_bnode = val_bnode;
	valid.arg = &fd;

	/* Initialise parser. */

	parse = hparse_alloc(inmode);

	if (NULL == parse) {
		perror(NULL);
		return(EXIT_FAILURE);
	}

	/* Initialise writer. */

	writer = hwrite_alloc(outmode);

	if (NULL == writer) {
		perror(NULL);
		hparse_delete(parse);
		return(EXIT_FAILURE);
	}

	/* Initialise validator. */

	validator = hvalid_alloc(inmode);

	if (NULL == validator) {
		perror(NULL);
		hwrite_delete(writer);
		hparse_delete(parse);
		return(EXIT_FAILURE);
	}

	root = NULL;
	cclone = cache = NULL;

	id.id = 0;
	id.name = "foo";

	/* Iterate over command-line arguments. */

	while (argc > 0) {
		memset(&fd, 0, sizeof(struct iofd));

		fd.fd = -1;
		fd.file = *argv;
		fd.perr = fd_perror;

		/* Parse, validate, write! */

		if ( ! hparse_tree(parse, &ctx, &root)) 
			break;

		assert(root);
		if ( ! hvalid_tree(validator, &valid, root))
			break;
		if ( ! hwrite_tree(writer, &out, root))
			break;

		cache = hcache_alloc
			(root, &id, id.id + 1, 1);

		if (NULL == cache)
			break;

		root = NULL;

		if ( ! hcache_verify(cache))
			puts("Cached tag missing!");
		else
			printf("Cached tag found: %d:%d.\n", 
				hnode_line(hcache_get(cache, id.id)),
				hnode_column(hcache_get(cache, id.id)));

		cclone = hcache_clone(cache);

		if (NULL == cclone)
			break;

		if ( ! hcache_verify(cclone))
			puts("Cached tag missing!");
		else
			printf("Cached tag found: %d:%d.\n", 
				hnode_line(hcache_get(cclone, id.id)),
				hnode_column(hcache_get(cclone, id.id)));

		hcache_delete(cache);
		hcache_delete(cclone);

		cache = cclone = NULL;

		argc--;
		argv++;
	}

	/* Free the root if it wasn't freed already. */

	if (root)
		hnode_delete(root);
	if (cache)
		hcache_delete(cache);
	if (cclone)
		hcache_delete(cclone);

	hvalid_delete(validator);
	hwrite_delete(writer);
	hparse_delete(parse);

	return(0 == argc ? EXIT_SUCCESS : EXIT_FAILURE);
}

static void
usage(void)
{

	fputs("usage: test [-Vx] file\n", stderr);
}

static int
fd_warn(const char *msg, int line, int col, void *arg)
{
	struct iofd	*fd;

	assert(arg && msg);
	fd = (struct iofd *)arg;

	fprintf(stderr, "%s:%d:%d: %s\n", 
			fd->file, line, col, msg);

	return(1);
}

static void
fd_syntax(const char *msg, int line, int col, void *arg)
{
	struct iofd	*fd;

	assert(arg && msg);
	fd = (struct iofd *)arg;

	fprintf(stderr, "%s:%d:%d: %s\n", 
			fd->file, line, col, msg);
}

/* ARGSUSED */
static void
fd_perror(const char *msg, void *arg)
{

	perror(msg);
}

/* ARGSUSED */
static void
fd_error(const char *msg, void *arg)
{

	fprintf(stderr, "%s\n", msg);
}

static int
val_node(struct hnode *p, void *arg)
{

	assert(arg);
	switch (hnode_type(p)) {
	case (HNODE_ELEM):
		val_elem(p, (struct iofd *)arg);
		break;
	case (HNODE_COMMENT):
		val_comm(p, (struct iofd *)arg);
		break;
	case (HNODE_DECL):
		val_decl(p, (struct iofd *)arg);
		break;
	case (HNODE_PROC):
		val_proc(p, (struct iofd *)arg);
		break;
	case (HNODE_ROOT):
		val_root(p, (struct iofd *)arg);
		break;
	case (HNODE_TEXT):
		val_text(p, (struct iofd *)arg);
		break;
	default:
		abort();
		/* NOTREACHED */
	}

	return(1);
}

static void
val_bnode(struct hnode *p, const char *msg, void *arg)
{
	struct iofd	*fd;

	assert(arg && msg);
	fd = (struct iofd *)arg;

	fprintf(stderr, "%s:%d:%d: %s\n", fd->file, 
			hnode_line(p), hnode_column(p), msg);
}


static void
val_battr(struct hattr *p, const char *msg, void *arg)
{
	struct iofd	*fd;

	assert(arg && msg);
	fd = (struct iofd *)arg;

	fprintf(stderr, "%s:%d:%d: %s\n", fd->file, 
			hattr_line(p), hattr_column(p), msg);
}

static int
val_attr(struct hattr *p, void *arg)
{
	struct iofd	*fd;

	assert(arg);
	fd = (struct iofd *)arg;

	fprintf(stderr, "%s:%d:%d: \"%s\" attribute valid\n", 
			fd->file, hattr_line(p), hattr_column(p),
			hattr_name(hattr_type(p)));
	return(1);
}

static int
val_ident(struct hident *p, void *arg)
{
	struct iofd	*fd;

	assert(arg);
	fd = (struct iofd *)arg;

	fprintf(stderr, "%s:%d:%d: \"%s\" identifier valid\n", 
			fd->file, hident_line(p), 
			hident_column(p), hident_value(p));
	return(1);
}

static void
val_decl(struct hnode *p, struct iofd *fd)
{

	fprintf(stderr, "%s:%d:%d: \"%s\" declaration valid\n", 
			fd->file, hnode_line(p), hnode_column(p), 
			hdecl_name(hnode_decl(p)));
}

static void
val_proc(struct hnode *p, struct iofd *fd)
{

	fprintf(stderr, "%s:%d:%d: \"%s\" pic valid\n", 
			fd->file, hnode_line(p), hnode_column(p), 
			hproc_name(hnode_proc(p)));
}

static void
val_comm(struct hnode *p, struct iofd *fd)
{

	fprintf(stderr, "%s:%d:%d: %zu bytes comment valid\n", 
			fd->file, hnode_line(p), hnode_column(p), 
			strlen(hnode_comment(p)));
}

static void
val_text(struct hnode *p, struct iofd *fd)
{

	fprintf(stderr, "%s:%d:%d: %zu bytes text valid\n", 
			fd->file, hnode_line(p), hnode_column(p), 
			strlen(hnode_text(p)));
}

static void
val_root(struct hnode *p, struct iofd *fd)
{

	fprintf(stderr, "%s:%d:%d: root valid\n", 
			fd->file, hnode_line(p), hnode_column(p));
}

static void
val_elem(struct hnode *p, struct iofd *fd)
{

	fprintf(stderr, "%s:%d:%d: \"%s\" element valid\n", 
			fd->file, hnode_line(p), hnode_column(p), 
			helem_name(hnode_elem(p)));
}
