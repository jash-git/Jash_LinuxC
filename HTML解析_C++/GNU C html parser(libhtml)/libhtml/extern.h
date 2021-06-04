/*	$Id: extern.h,v 1.22 2010/03/12 05:11:13 kristaps Exp $ */
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
#ifndef EXTERN_H
#define EXTERN_H

__BEGIN_DECLS

enum	ioerror {
	IOERR_MEM,			/* out of memory */
	IOERR_EMAX
};

enum	iosyntax {
	IOERR_NEOF,			/* unexpected eof */
	IOERR_TAG,			/* bad tag name */
	IOERR_BTAG,			/* bad tag syntax */
	IOERR_NCTX,			/* invalid scope */
	IOERR_ATTR,			/* bad attr name */
	IOERR_BATTR,			/* bad attr syntax */
	IOERR_SGML,			/* sgml unsupported */
	IOERR_BDECL,			/* bad decl syntax */
	IOERR_DECL,			/* bad decl name */
	IOERR_DTYPE,			/* bad doctype name */
	IOERR_BPROC,			/* bad proc syntax */
	IOERR_PROC,			/* bad proc name */
	IOERR_CHLD,			/* bad child of parent */
	IOERR_BCASE,			/* bad character case */
	IOERR_SMAX
};

enum	iowarn {
	IOERR_UPPER,			/* suggest uppercase */
	IOERR_LOWER,			/* suggest lowercase */
	IOERR_WMAX
};


enum	ioinvalid {
	IOERR_ATTRVER,		/* not in html version */
	IOERR_ATTRVAL,		/* bad attribute value */
	IOERR_ELEMEX,		/* element exclusion */
	IOERR_BCHARREF,		/* character reference syntax */
	IOERR_CHARREF,		/* unknown character reference */
	IOERR_BELEMATTR,	/* bad attribute for element */
	IOERR_IMAX
};

struct	ioctx;
struct	iobuf;
struct	ioout;
struct	iovalid;

int	  	 ioctx_close(struct ioctx *);
int	  	 ioctx_error(struct ioctx *, enum ioerror);
int	  	 ioctx_getchar(struct ioctx *, char *);
int	  	 ioctx_open(struct ioctx *);
int	  	 ioctx_rew(struct ioctx *);
int	  	 ioctx_syntax(struct ioctx *, int, int, enum iosyntax);
int	  	 ioctx_warn(struct ioctx *, int, int, enum iowarn);
int		 ioctx_xcode(struct ioctx *, char, char **, size_t *);

struct	iobuf	*iobuf_alloc(void);
const char	*iobuf_buf(const struct iobuf *);
size_t		 iobuf_size(const struct iobuf *);
void	  	 iobuf_final(struct iobuf *);
int	  	 iobuf_grow(struct iobuf *);
void	  	 iobuf_free(struct iobuf *);
int	  	 iobuf_putchar(struct iobuf *, char);
int	  	 iobuf_puts(struct iobuf *, const char *, size_t);
void	  	 iobuf_reset(struct iobuf *);

int	  	 ioout_open(struct ioout *);
int	  	 ioout_close(struct ioout *);
int	  	 ioout_puts(struct ioout *, const char *);
int	  	 ioout_putchar(struct ioout *, char);

int		 iovalid_attr(struct hattr *, struct iovalid *);
int		 iovalid_battr(struct hattr *, 
			enum ioinvalid, struct iovalid *);
int		 iovalid_node(struct hnode *, struct iovalid *);
int		 iovalid_bnode(struct hnode *, 
			enum ioinvalid, struct iovalid *);
int		 iovalid_ident(struct hident *, struct iovalid *);

int		 helem_empty(enum helemt, enum hmode);
const enum helemt *helem_excludes(enum helemt, enum hmode);
int		 hattr_elem_perm(enum hattrt, enum helemt, enum hmode);
int		 hattr_proc_perm(enum hattrt, enum hproct, enum hmode);

__END_DECLS

#endif /*! EXTERN_H */
