/*	$Id: html.c,v 1.26 2010/05/18 08:05:36 kristaps Exp $ */
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
#include <unistd.h>

#include "html.h"
#include "extern.h"

#define	EMPTY		   1	/* FIXME: clean up */

/*
 * Index of an HTML version.  This must be kept up-to-date with any new
 * enum hmode values; it allows to have an indexed array.
 */
enum	hindex {
	HINDEX_HTML_4_01,
	HINDEX_XHTML_1_0,
	HINDEX__MAX
};

/* 
 * Elements allowed/disallowed ownership of an attribute.  Since an
 * attribute may have its definition change over versions, we index into
 * the version to get the lists.
 */
struct	hattrperm {
	int		   melem;
#define	HPERM_ALLOW	   0
#define	HPERM_DENY	   1
#define	HPERM_NONE	   2
	union {
		const enum helemt *allow[HINDEX__MAX];
		const enum helemt *deny[HINDEX__MAX];
	} pelem;
	int		   mproc;
	union {
		const enum hproct *allow[HINDEX__MAX];
		const enum hproct *deny[HINDEX__MAX];
	} pproc;
};

/* 
 * Each type of node (processing instruction, element, attribute, and so
 * on) is associated with certain information that may be referred to be
 * using the node type as an index into an array.  These are defined
 * here.
 */

struct	hdecldef {
	const char	  *enumname; /* name of enumeration */
	const char	  *name; /* name of declaration */
};

struct	hprocdef {
	const char	  *enumname; /* name of enumeration */
	const char	  *name; /* name of declaration */
	int		   modes; /* bit-wise enum hmode */
};

struct	hattrdef {
	const char	  *enumname; /* name of enumeration */
	const char	  *name; /* name of HTML attribute */
	int		   flags;
	int		   modes; /* bit-wise enum hmode */
	const struct hattrperm *perms; /* element permissions */
};

struct	helemdef {
	const char	  *enumname; /* name of enumeration */
	const char	  *name; /* name of HTML element */
	int		   flags;
	int		   modes; /* bit-wise enum hmode */
	const enum helemt *excludes; /* HTML exclusions */
};

struct	hcrefdef {
	const char	  *name; /* name of char ref */
};


/*
 * Exclusions: some elements aren't allowed to contain other elements as
 * defined by the HTML specification.  This section contains per-element
 * exclusions lists.
 */

const	enum helemt helem_excludes_a[] = {
	HELEM_A,
	HELEM__MAX
};

const	enum helemt helem_excludes_pre[] = {
	HELEM_BIG,
	HELEM_IMG,
	HELEM_OBJECT,
	HELEM_SMALL,
	HELEM_SUB,
	HELEM_SUP,
	HELEM__MAX
};

const	enum helemt helem_excludes_button[] = {
	HELEM_BUTTON, 
	HELEM_FIELDSET, 
	HELEM_FORM, 
	HELEM_IFRAME,
	HELEM_INPUT, 
	HELEM_ISINDEX,
	HELEM_LABEL, 
	HELEM_SELECT, 
	HELEM_TEXTAREA, 
	HELEM__MAX
};

const	enum helemt helem_excludes_label[] = {
	HELEM_LABEL,
	HELEM__MAX
};

const	enum helemt helem_excludes_form[] = {
	HELEM_FORM,
	HELEM__MAX
};

/*
 * Permissions of attributes to be in certain elements as keyed by HTML
 * version.
 */

#define	PERM_BEGIN(name) \
const	enum helemt	 name ## _perm[] =
#define	PPERM_BEGIN(name) \
const	enum hproct	 name ## _perm[] =
#define	PERM_END(perm, name) ; \
const	struct hattrperm name = { \
	perm, { { name ## _perm, name ## _perm } }, \
	HPERM_NONE, { { NULL, NULL } } \
}

#define	PERM_DENY_BEGIN(name) PERM_BEGIN(name)
#define	PERM_DENY_END(name) PERM_END(HPERM_DENY, name)

#define	PERM_ALLOW_BEGIN(name) PERM_BEGIN(name)
#define	PERM_ALLOW_END(name) PERM_END(HPERM_ALLOW, name)

PERM_ALLOW_BEGIN(hperm_abbr) {
	HELEM_TD, HELEM_TH, HELEM__MAX
} PERM_ALLOW_END(hperm_abbr);

PERM_ALLOW_BEGIN(hperm_accept) {
	HELEM_FORM, HELEM_INPUT, HELEM__MAX
} PERM_ALLOW_END(hperm_accept);

PERM_ALLOW_BEGIN(hperm_accept_charset) {
	HELEM_FORM, HELEM__MAX
} PERM_ALLOW_END(hperm_accept_charset);

PERM_ALLOW_BEGIN(hperm_accesskey) {
	HELEM_A, HELEM_AREA, HELEM_BUTTON, HELEM_INPUT, 
	HELEM_LABEL, HELEM_LEGEND, HELEM_TEXTAREA, HELEM__MAX
} PERM_ALLOW_END(hperm_accesskey);

PERM_ALLOW_BEGIN(hperm_action) {
	HELEM_FORM, HELEM__MAX
} PERM_ALLOW_END(hperm_action);

PERM_ALLOW_BEGIN(hperm_align) {
	HELEM_COL, HELEM_COLGROUP, HELEM_TBODY, HELEM_TD, 
	HELEM_TFOOT, HELEM_TH, HELEM_THEAD, HELEM_TR, HELEM__MAX
} PERM_ALLOW_END(hperm_align);

PERM_ALLOW_BEGIN(hperm_alt) {
	HELEM_AREA, HELEM_IMG, HELEM_INPUT, HELEM__MAX
} PERM_ALLOW_END(hperm_alt);

PERM_ALLOW_BEGIN(hperm_archive) {
	HELEM_OBJECT, HELEM__MAX
} PERM_ALLOW_END(hperm_archive);

PERM_ALLOW_BEGIN(hperm_axis) {
	HELEM_TD, HELEM_TH, HELEM__MAX
} PERM_ALLOW_END(hperm_axis);

PERM_ALLOW_BEGIN(hperm_border) {
	HELEM_TABLE, HELEM__MAX
} PERM_ALLOW_END(hperm_border);

PERM_ALLOW_BEGIN(hperm_cellpadding) {
	HELEM_TABLE, HELEM__MAX
} PERM_ALLOW_END(hperm_cellpadding);

PERM_ALLOW_BEGIN(hperm_cellspacing) {
	HELEM_TABLE, HELEM__MAX
} PERM_ALLOW_END(hperm_cellspacing);

PERM_ALLOW_BEGIN(hperm_char) {
	HELEM_COL, HELEM_COLGROUP, HELEM_TBODY, HELEM_TD, 
	HELEM_TFOOT, HELEM_TH, HELEM_THEAD, HELEM_TR, HELEM__MAX
} PERM_ALLOW_END(hperm_char);

PERM_ALLOW_BEGIN(hperm_charoff) {
	HELEM_COL, HELEM_COLGROUP, HELEM_TBODY, HELEM_TD, 
	HELEM_TFOOT, HELEM_TH, HELEM_THEAD, HELEM_TR, HELEM__MAX
} PERM_ALLOW_END(hperm_charoff);

PERM_ALLOW_BEGIN(hperm_charset) {
	HELEM_A, HELEM_LINK, HELEM_SCRIPT, HELEM__MAX
} PERM_ALLOW_END(hperm_charset);

PERM_ALLOW_BEGIN(hperm_checked) {
	HELEM_INPUT, HELEM__MAX
} PERM_ALLOW_END(hperm_checked);

PERM_ALLOW_BEGIN(hperm_cite) {
	HELEM_BLOCKQUOTE, HELEM_Q, HELEM_DEL, HELEM_INS, HELEM__MAX
} PERM_ALLOW_END(hperm_cite);

PERM_DENY_BEGIN(hperm_class) {
	HELEM_BASE, HELEM_BASEFONT, HELEM_HEAD, HELEM_HTML, 
	HELEM_META, HELEM_PARAM, HELEM_SCRIPT, HELEM_STYLE, 
	HELEM_TITLE, HELEM__MAX
} PERM_DENY_END(hperm_class);

PERM_ALLOW_BEGIN(hperm_classid) {
	HELEM_OBJECT, HELEM__MAX
} PERM_ALLOW_END(hperm_classid);

PERM_ALLOW_BEGIN(hperm_codebase) {
	HELEM_OBJECT, HELEM__MAX
} PERM_ALLOW_END(hperm_codebase);

PERM_ALLOW_BEGIN(hperm_codetype) {
	HELEM_OBJECT, HELEM__MAX
} PERM_ALLOW_END(hperm_codetype);

PERM_ALLOW_BEGIN(hperm_cols) {
	HELEM_FRAMESET, HELEM_TEXTAREA, HELEM__MAX
} PERM_ALLOW_END(hperm_cols);

PERM_ALLOW_BEGIN(hperm_colspan) {
	HELEM_TD, HELEM_TH, HELEM__MAX
} PERM_ALLOW_END(hperm_colspan);

PERM_ALLOW_BEGIN(hperm_content) {
	HELEM_META, HELEM__MAX
} PERM_ALLOW_END(hperm_content);

PERM_ALLOW_BEGIN(hperm_coords) {
	HELEM_AREA, HELEM_A, HELEM__MAX
} PERM_ALLOW_END(hperm_coords);

PERM_ALLOW_BEGIN(hperm_data) {
	HELEM_OBJECT, HELEM__MAX
} PERM_ALLOW_END(hperm_data);

PERM_ALLOW_BEGIN(hperm_datetime) {
	HELEM_DEL, HELEM_INS, HELEM__MAX
} PERM_ALLOW_END(hperm_datetime);

PERM_ALLOW_BEGIN(hperm_declare) {
	HELEM_OBJECT, HELEM__MAX
} PERM_ALLOW_END(hperm_declare);

PERM_ALLOW_BEGIN(hperm_defer) {
	HELEM_SCRIPT, HELEM__MAX
} PERM_ALLOW_END(hperm_defer);

PERM_DENY_BEGIN(hperm_dir) {
	HELEM_APPLET, HELEM_BASE, HELEM_BASEFONT, HELEM_BDO, 
	HELEM_BR, HELEM_FRAME, HELEM_FRAMESET, HELEM_IFRAME, 
	HELEM_PARAM, HELEM_SCRIPT, HELEM__MAX
} PERM_DENY_END(hperm_dir);

PERM_ALLOW_BEGIN(hperm_disabled) {
	HELEM_BUTTON, HELEM_INPUT, HELEM_OPTGROUP, HELEM_OPTION, 
	HELEM_SELECT, HELEM_TEXTAREA, HELEM__MAX
} PERM_ALLOW_END(hperm_disabled);

PERM_ALLOW_BEGIN(hperm_enctype) {
	HELEM_FORM, HELEM__MAX
} PERM_ALLOW_END(hperm_enctype);

PERM_ALLOW_BEGIN(hperm_for) {
	HELEM_LABEL, HELEM__MAX
} PERM_ALLOW_END(hperm_for);

PERM_ALLOW_BEGIN(hperm_frame) {
	HELEM_TABLE, HELEM__MAX
} PERM_ALLOW_END(hperm_frame);

PERM_ALLOW_BEGIN(hperm_frameborder) {
	HELEM_FRAME, HELEM_IFRAME, HELEM__MAX
} PERM_ALLOW_END(hperm_frameborder);

PERM_ALLOW_BEGIN(hperm_headers) {
	HELEM_TD, HELEM_TH, HELEM__MAX
} PERM_ALLOW_END(hperm_headers);

PERM_ALLOW_BEGIN(hperm_height) {
	HELEM_IMG, HELEM_OBJECT, HELEM__MAX
} PERM_ALLOW_END(hperm_height);

PERM_ALLOW_BEGIN(hperm_href) {
	HELEM_A, HELEM_AREA, HELEM_LINK, HELEM_BASE, HELEM__MAX
} PERM_ALLOW_END(hperm_href);

PERM_ALLOW_BEGIN(hperm_hreflang) {
	HELEM_A, HELEM_LINK, HELEM__MAX
} PERM_ALLOW_END(hperm_hreflang);

PERM_ALLOW_BEGIN(hperm_http_equiv) {
	HELEM_META, HELEM__MAX
} PERM_ALLOW_END(hperm_http_equiv);

PERM_DENY_BEGIN(hperm_id) {
	HELEM_BASE, HELEM_HEAD, HELEM_HTML, HELEM_META, HELEM_SCRIPT, 
	HELEM_STYLE, HELEM_TITLE, HELEM__MAX
} PERM_DENY_END(hperm_id);

PERM_ALLOW_BEGIN(hperm_ismap) {
	HELEM_IMG, HELEM_INPUT, HELEM__MAX
} PERM_ALLOW_END(hperm_ismap);

PERM_ALLOW_BEGIN(hperm_label) {
	HELEM_OPTION, HELEM_OPTGROUP, HELEM__MAX
} PERM_ALLOW_END(hperm_label);

PERM_DENY_BEGIN(hperm_lang) {
	HELEM_APPLET, HELEM_BASE, HELEM_BASEFONT, HELEM_BR, 
	HELEM_FRAME, HELEM_FRAMESET, HELEM_IFRAME, HELEM_PARAM, 
	HELEM_SCRIPT, HELEM__MAX
} PERM_DENY_END(hperm_lang);

PERM_ALLOW_BEGIN(hperm_longdesc) {
	HELEM_IMG, HELEM_FRAME, HELEM_IFRAME, HELEM__MAX
} PERM_ALLOW_END(hperm_longdesc);

PERM_ALLOW_BEGIN(hperm_marginwidth) {
	HELEM_FRAME, HELEM_IFRAME, HELEM__MAX
} PERM_ALLOW_END(hperm_marginwidth);

PERM_ALLOW_BEGIN(hperm_marginheight) {
	HELEM_FRAME, HELEM_IFRAME, HELEM__MAX
} PERM_ALLOW_END(hperm_marginheight);

PERM_ALLOW_BEGIN(hperm_maxlength) {
	HELEM_INPUT, HELEM__MAX
} PERM_ALLOW_END(hperm_maxlength);

PERM_ALLOW_BEGIN(hperm_media) {
	HELEM_STYLE, HELEM_LINK, HELEM__MAX
} PERM_ALLOW_END(hperm_media);

PERM_ALLOW_BEGIN(hperm_method) {
	HELEM_FORM, HELEM__MAX
} PERM_ALLOW_END(hperm_method);

PERM_ALLOW_BEGIN(hperm_multiple) {
	HELEM_SELECT, HELEM__MAX
} PERM_ALLOW_END(hperm_multiple);

PERM_BEGIN(hperm_name_html_4_01) {
	HELEM_BUTTON, HELEM_TEXTAREA, HELEM_SELECT, HELEM_FORM, 
	HELEM_FRAME, HELEM_IFRAME, HELEM_IMG, HELEM_A, HELEM_INPUT, 
	HELEM_OBJECT, HELEM_MAP, HELEM_PARAM, HELEM_META, HELEM__MAX
};

PERM_BEGIN(hperm_name_xhtml_1_0) {
	HELEM_BUTTON, HELEM_TEXTAREA, HELEM_SELECT, HELEM_INPUT, 
	HELEM_OBJECT, HELEM_PARAM, HELEM_META, HELEM__MAX 
};

const	struct hattrperm hperm_name = {
	HPERM_ALLOW,
	{ { hperm_name_html_4_01_perm, 
	    hperm_name_xhtml_1_0_perm } },
	HPERM_NONE, { { NULL, NULL } }
};

PERM_ALLOW_BEGIN(hperm_nohref) {
	HELEM_AREA, HELEM__MAX
} PERM_ALLOW_END(hperm_nohref);

PERM_ALLOW_BEGIN(hperm_noresize) {
	HELEM_FRAME, HELEM__MAX
} PERM_ALLOW_END(hperm_noresize);

PERM_DENY_BEGIN(hperm_onintrinsic) {
	HELEM_APPLET, HELEM_BASE, HELEM_BASEFONT, HELEM_BDO, HELEM_BR,
	HELEM_FONT, HELEM_FRAME, HELEM_FRAMESET, HELEM_HEAD, 
	HELEM_HTML, HELEM_IFRAME, HELEM_ISINDEX, HELEM_META, 
	HELEM_PARAM, HELEM_SCRIPT, HELEM_STYLE, HELEM_TITLE, 
	HELEM__MAX
} PERM_DENY_END(hperm_onintrinsic);

PERM_ALLOW_BEGIN(hperm_onblur) {
	HELEM_A, HELEM_AREA, HELEM_BUTTON, HELEM_INPUT, HELEM_LABEL, 
	HELEM_SELECT, HELEM_TEXTAREA, HELEM__MAX
} PERM_ALLOW_END(hperm_onblur);

PERM_ALLOW_BEGIN(hperm_onchange) {
	HELEM_INPUT, HELEM_SELECT, HELEM_TEXTAREA, HELEM__MAX
} PERM_ALLOW_END(hperm_onchange);

PERM_ALLOW_BEGIN(hperm_onfocus) {
	HELEM_A, HELEM_AREA, HELEM_BUTTON, HELEM_INPUT, HELEM_LABEL, 
	HELEM_SELECT, HELEM_TEXTAREA, HELEM__MAX
} PERM_ALLOW_END(hperm_onfocus);

PERM_ALLOW_BEGIN(hperm_onload) {
	HELEM_FRAMESET, HELEM_BODY, HELEM__MAX
} PERM_ALLOW_END(hperm_onload);

PERM_ALLOW_BEGIN(hperm_onreset) {
	HELEM_FORM, HELEM__MAX
} PERM_ALLOW_END(hperm_onreset);

PERM_ALLOW_BEGIN(hperm_onselect) {
	HELEM_INPUT, HELEM_TEXTAREA, HELEM__MAX
} PERM_ALLOW_END(hperm_onselect);

PERM_ALLOW_BEGIN(hperm_onsubmit) {
	HELEM_FORM, HELEM__MAX
} PERM_ALLOW_END(hperm_onsubmit);

PERM_ALLOW_BEGIN(hperm_onunload) {
	HELEM_FRAMESET, HELEM_BODY, HELEM__MAX
} PERM_ALLOW_END(hperm_onunload);

PERM_ALLOW_BEGIN(hperm_profile) {
	HELEM_HEAD, HELEM__MAX
} PERM_ALLOW_END(hperm_profile);

PERM_ALLOW_BEGIN(hperm_readonly) {
	HELEM_TEXTAREA, HELEM_INPUT, HELEM__MAX
} PERM_ALLOW_END(hperm_readonly);

PERM_ALLOW_BEGIN(hperm_rel) {
	HELEM_A, HELEM_LINK, HELEM__MAX
} PERM_ALLOW_END(hperm_rel);

PERM_ALLOW_BEGIN(hperm_rev) {
	HELEM_A, HELEM_LINK, HELEM__MAX
} PERM_ALLOW_END(hperm_rev);

PERM_ALLOW_BEGIN(hperm_rows) {
	HELEM_FRAMESET, HELEM_TEXTAREA, HELEM__MAX
} PERM_ALLOW_END(hperm_rows);

PERM_ALLOW_BEGIN(hperm_rowspan) {
	HELEM_TD, HELEM_TH, HELEM__MAX
} PERM_ALLOW_END(hperm_rowspan);

PERM_ALLOW_BEGIN(hperm_rules) {
	HELEM_TABLE, HELEM__MAX
} PERM_ALLOW_END(hperm_rules);

PERM_ALLOW_BEGIN(hperm_scheme) {
	HELEM_META, HELEM__MAX
} PERM_ALLOW_END(hperm_scheme);

PERM_ALLOW_BEGIN(hperm_scope) {
	HELEM_TD, HELEM_TH, HELEM__MAX
} PERM_ALLOW_END(hperm_scope);

PERM_ALLOW_BEGIN(hperm_scrolling) {
	HELEM_FRAME, HELEM_IFRAME, HELEM__MAX
} PERM_ALLOW_END(hperm_scrolling);

PERM_ALLOW_BEGIN(hperm_selected) {
	HELEM_OPTION, HELEM__MAX
} PERM_ALLOW_END(hperm_selected);

PERM_ALLOW_BEGIN(hperm_shape) {
	HELEM_AREA, HELEM_A, HELEM__MAX
} PERM_ALLOW_END(hperm_shape);

PERM_ALLOW_BEGIN(hperm_size) {
	HELEM_INPUT, HELEM_SELECT, HELEM__MAX
} PERM_ALLOW_END(hperm_size);

PERM_ALLOW_BEGIN(hperm_span) {
	HELEM_COL, HELEM_COLGROUP, HELEM__MAX
} PERM_ALLOW_END(hperm_span);

PERM_ALLOW_BEGIN(hperm_src) {
	HELEM_FRAME, HELEM_IFRAME, HELEM_SCRIPT, HELEM_INPUT, 
	HELEM_IMG, HELEM__MAX
} PERM_ALLOW_END(hperm_src);

PERM_ALLOW_BEGIN(hperm_standby) {
	HELEM_OBJECT, HELEM__MAX
} PERM_ALLOW_END(hperm_standby);

PERM_ALLOW_BEGIN(hperm_start) {
	HELEM_OL, HELEM__MAX
} PERM_ALLOW_END(hperm_start);

PERM_DENY_BEGIN(hperm_style) {
	HELEM_BASE, HELEM_BASEFONT, HELEM_HEAD, HELEM_HTML, 
	HELEM_META, HELEM_PARAM, HELEM_SCRIPT, HELEM_STYLE, 
	HELEM_TITLE, HELEM__MAX
} PERM_DENY_END(hperm_style);

PERM_ALLOW_BEGIN(hperm_summary) {
	HELEM_TABLE, HELEM__MAX
} PERM_ALLOW_END(hperm_summary);

PERM_ALLOW_BEGIN(hperm_tabindex) {
	HELEM_A, HELEM_AREA, HELEM_BUTTON, HELEM_INPUT, HELEM_OBJECT,
	HELEM_SELECT, HELEM_TEXTAREA, HELEM__MAX
} PERM_ALLOW_END(hperm_tabindex);

PERM_ALLOW_BEGIN(hperm_target) {
	HELEM_A, HELEM_AREA, HELEM_BASE, HELEM_FORM, HELEM_LINK, 
	HELEM__MAX
} PERM_ALLOW_END(hperm_target);

PERM_DENY_BEGIN(hperm_title) {
	HELEM_BASE, HELEM_BASEFONT, HELEM_HEAD, HELEM_HTML, 
	HELEM_META, HELEM_PARAM, HELEM_SCRIPT, HELEM_TITLE,
	HELEM__MAX
} PERM_DENY_END(hperm_title);

PERM_ALLOW_BEGIN(hperm_type) {
	HELEM_A, HELEM_LINK, HELEM_OBJECT, HELEM_PARAM, HELEM_SCRIPT,
	HELEM_STYLE, HELEM_INPUT, HELEM_BUTTON, HELEM__MAX
} PERM_ALLOW_END(hperm_type);

PERM_ALLOW_BEGIN(hperm_usemap) {
	HELEM_IMG, HELEM_INPUT, HELEM_OBJECT, HELEM__MAX
} PERM_ALLOW_END(hperm_usemap);

PERM_ALLOW_BEGIN(hperm_valign) {
	HELEM_COL, HELEM_COLGROUP, HELEM_TBODY, HELEM_TD, 
	HELEM_TFOOT, HELEM_TH, HELEM_THEAD, HELEM_TR, HELEM__MAX
} PERM_ALLOW_END(hperm_valign);

PERM_ALLOW_BEGIN(hperm_value) {
	HELEM_INPUT, HELEM_OPTION, HELEM_PARAM, HELEM_BUTTON, 
	HELEM__MAX
} PERM_ALLOW_END(hperm_value);

PERM_ALLOW_BEGIN(hperm_valuetype) {
	HELEM_PARAM, HELEM__MAX
} PERM_ALLOW_END(hperm_valuetype);

PPERM_BEGIN(hperm_version_xhtml_1_0) {
	HPROC_XML, HPROC__MAX
};

const	struct hattrperm hperm_version = {
	HPERM_NONE, { { NULL, NULL } },
	HPERM_ALLOW,
	{ { NULL, hperm_version_xhtml_1_0_perm } }
};

PERM_ALLOW_BEGIN(hperm_width) {
	HELEM_IFRAME, HELEM_IMG, HELEM_OBJECT, HELEM_TABLE, HELEM_COL,
	HELEM_COLGROUP, HELEM__MAX
} PERM_ALLOW_END(hperm_width);

PERM_BEGIN(hperm_xmlns_xhtml_1_0) {
	HELEM_HTML, HELEM__MAX 
};

const	struct hattrperm hperm_xmlns = {
	HPERM_ALLOW,
	{ { NULL, hperm_xmlns_xhtml_1_0_perm } },
	HPERM_NONE, { { NULL, NULL } }
};

/*
 * Per-node information tables.  This is the may bulk of HTML validation
 * and semantic structural rules.
 */

const	struct hcrefdef	hcrefdefs[HCREF__MAX] = {
	{ "AElig" },
	{ "Aacute" },
	{ "Acirc" },
	{ "Agrave" },
	{ "Alpha" },
	{ "Aring" },
	{ "Atilde" },
	{ "Auml" },
	{ "Beta" },
	{ "Ccedil" },
	{ "Chi" },
	{ "Dagger" },
	{ "Delta" },
	{ "ETH" },
	{ "Eacute" },
	{ "Ecirc" },
	{ "Egrave" },
	{ "Epsilon" },
	{ "Eta" },
	{ "Euml" },
	{ "Gamma" },
	{ "Iacute" },
	{ "Icirc" },
	{ "Igrave" },
	{ "Iota" },
	{ "Iuml" },
	{ "Kappa" },
	{ "Lambda" },
	{ "Mu" },
	{ "Ntilde" },
	{ "Nu" },
	{ "OElig" },
	{ "Oacute" },
	{ "Ocirc" },
	{ "Ograve" },
	{ "Omega" },
	{ "Omicron" },
	{ "Oslash" },
	{ "Otilde" },
	{ "Ouml" },
	{ "Phi" },
	{ "Pi" },
	{ "Prime" },
	{ "Psi" },
	{ "Rho" },
	{ "Scaron" },
	{ "Sigma" },
	{ "THORN" },
	{ "Tau" },
	{ "Theta" },
	{ "Uacute" },
	{ "Ucirc" },
	{ "Ugrave" },
	{ "Upsilon" },
	{ "Uuml" },
	{ "Xi" },
	{ "Yacute" },
	{ "Yuml" },
	{ "Zeta" },
	{ "aacute" },
	{ "acirc" },
	{ "acute" },
	{ "aelig" },
	{ "agrave" },
	{ "alefsym" },
	{ "alpha" },
	{ "amp" },
	{ "and" },
	{ "ang" },
	{ "aring" },
	{ "asymp" },
	{ "atilde" },
	{ "auml" },
	{ "bdquo" },
	{ "beta" },
	{ "brvbar" },
	{ "bull" },
	{ "cap" },
	{ "ccedil" },
	{ "cedil" },
	{ "cent" },
	{ "chi" },
	{ "circ" },
	{ "clubs" },
	{ "cong" },
	{ "copy" },
	{ "crarr" },
	{ "cup" },
	{ "curren" },
	{ "dArr" },
	{ "dagger" },
	{ "darr" },
	{ "deg" },
	{ "delta" },
	{ "diams" },
	{ "divide" },
	{ "eacute" },
	{ "ecirc" },
	{ "egrave" },
	{ "empty" },
	{ "emsp" },
	{ "ensp" },
	{ "epsilon" },
	{ "equiv" },
	{ "eta" },
	{ "eth" },
	{ "euml" },
	{ "euro" },
	{ "exist" },
	{ "fnof" },
	{ "forall" },
	{ "frac12" },
	{ "frac14" },
	{ "frac34" },
	{ "frasl" },
	{ "gamma" },
	{ "ge" },
	{ "gt" },
	{ "hArr" },
	{ "harr" },
	{ "hearts" },
	{ "hellip" },
	{ "iacute" },
	{ "icirc" },
	{ "iexcl" },
	{ "igrave" },
	{ "image" },
	{ "infin" },
	{ "int" },
	{ "iota" },
	{ "iquest" },
	{ "isin" },
	{ "iuml" },
	{ "kappa" },
	{ "lArr" },
	{ "lambda" },
	{ "lang" },
	{ "laquo" },
	{ "larr" },
	{ "lceil" },
	{ "ldquo" },
	{ "le" },
	{ "lfloor" },
	{ "lowast" },
	{ "loz" },
	{ "lrm" },
	{ "lsaquo" },
	{ "lsquo" },
	{ "lt" },
	{ "macr" },
	{ "mdash" },
	{ "micro" },
	{ "middot" },
	{ "minus" },
	{ "mu" },
	{ "nabla" },
	{ "nbsp" },
	{ "ndash" },
	{ "ne" },
	{ "ni" },
	{ "not" },
	{ "notin" },
	{ "nsub" },
	{ "ntilde" },
	{ "nu" },
	{ "oacute" },
	{ "ocirc" },
	{ "oelig" },
	{ "ograve" },
	{ "oline" },
	{ "omega" },
	{ "omicron" },
	{ "oplus" },
	{ "or" },
	{ "ordf" },
	{ "ordm" },
	{ "oslash" },
	{ "otilde" },
	{ "otimes" },
	{ "ouml" },
	{ "para" },
	{ "part" },
	{ "permil" },
	{ "perp" },
	{ "phi" },
	{ "pi" },
	{ "piv" },
	{ "plusmn" },
	{ "pound" },
	{ "prime" },
	{ "prod" },
	{ "prop" },
	{ "psi" },
	{ "quot" },
	{ "rArr" },
	{ "radic" },
	{ "rang" },
	{ "raquo" },
	{ "rarr" },
	{ "rceil" },
	{ "rdquo" },
	{ "real" },
	{ "reg" },
	{ "rfloor" },
	{ "rho" },
	{ "rlm" },
	{ "rsaquo" },
	{ "rsquo" },
	{ "sbquo" },
	{ "scaron" },
	{ "sdot" },
	{ "sect" },
	{ "shy" },
	{ "sigma" },
	{ "sigmaf" },
	{ "sim" },
	{ "spades" },
	{ "sub" },
	{ "sube" },
	{ "sum" },
	{ "sup" },
	{ "sup1" },
	{ "sup2" },
	{ "sup3" },
	{ "supe" },
	{ "szlig" },
	{ "tau" },
	{ "there4" },
	{ "theta" },
	{ "thetasym" },
	{ "thinsp" },
	{ "thorn" },
	{ "tilde" },
	{ "times" },
	{ "trade" },
	{ "uArr" },
	{ "uacute" },
	{ "uarr" },
	{ "ucirc" },
	{ "ugrave" },
	{ "uml" },
	{ "upsih" },
	{ "upsilon" },
	{ "uuml" },
	{ "weierp" },
	{ "xi" },
	{ "yacute" },
	{ "yen" },
	{ "yuml" },
	{ "zeta" },
	{ "zwj" },
	{ "zwnj" }
};

const	struct hdecldef hdecldefs[HDECL__MAX] = {
	{ "HDECL_DOCTYPE", "DOCTYPE" },
};

const	struct hprocdef hprocdefs[HPROC__MAX] = {
	{ "HPROC_XML", "xml", HMODE_XML },
};

const	struct hattrdef	 hattrdefs[HATTR__MAX] = {
	{ "HATTR_ABBR", "ABBR", 0, HMODE_ALL, &hperm_abbr },
	{ "HATTR_ACCEPT", "ACCEPT", 0, HMODE_ALL, &hperm_accept },
	{ "HATTR_ACCEPT_CHARSET", "ACCEPT-CHARSET", 0, HMODE_ALL, &hperm_accept_charset },
	{ "HATTR_ACCESSKEY", "ACCESSKEY", 0, HMODE_ALL, &hperm_accesskey },
	{ "HATTR_ACTION", "ACTION", 0, HMODE_ALL, &hperm_action },
	{ "HATTR_ALIGN", "ALIGN", 0, HMODE_ALL, &hperm_align },
	{ "HATTR_ALINK", "ALINK", 0, HMODE_ALL, NULL },
	{ "HATTR_ALT", "ALT", 0, HMODE_ALL, &hperm_alt },
	{ "HATTR_ARCHIVE", "ARCHIVE", 0, HMODE_ALL, &hperm_archive },
	{ "HATTR_AXIS", "AXIS", 0, HMODE_ALL, &hperm_axis },
	{ "HATTR_BACKGROUND", "BACKGROUND", 0, HMODE_ALL, NULL },
	{ "HATTR_BGCOLOR", "BGCOLOR", 0, HMODE_ALL, NULL },
	{ "HATTR_BORDER", "BORDER", 0, HMODE_ALL, &hperm_border },
	{ "HATTR_CELLPADDING", "CELLPADDING", 0, HMODE_ALL, &hperm_cellpadding },
	{ "HATTR_CELLSPACING", "CELLSPACING", 0, HMODE_ALL, &hperm_cellspacing },
	{ "HATTR_CHAR", "CHAR", 0, HMODE_ALL, &hperm_char },
	{ "HATTR_CHAROFF", "CHAROFF", 0, HMODE_ALL, &hperm_charoff },
	{ "HATTR_CHARSET", "CHARSET", 0, HMODE_ALL, &hperm_charset },
	{ "HATTR_CHECKED", "CHECKED", EMPTY, HMODE_ALL, &hperm_checked },
	{ "HATTR_CITE", "CITE", 0, HMODE_ALL, &hperm_cite },
	{ "HATTR_CLASS", "CLASS", 0, HMODE_ALL, &hperm_class },
	{ "HATTR_CLASSID", "CLASSID", 0, HMODE_ALL, &hperm_classid },
	{ "HATTR_CLEAR", "CLEAR", 0, HMODE_ALL, NULL },
	{ "HATTR_CODE", "CODE", 0, HMODE_ALL, NULL },
	{ "HATTR_CODEBASE", "CODEBASE", 0, HMODE_ALL, &hperm_codebase },
	{ "HATTR_CODETYPE", "CODETYPE", 0, HMODE_ALL, &hperm_codetype },
	{ "HATTR_COLOR", "COLOR", 0, HMODE_ALL, NULL },
	{ "HATTR_COLS", "COLS", 0, HMODE_ALL, &hperm_cols },
	{ "HATTR_COLSPAN", "COLSPAN", 0, HMODE_ALL, &hperm_colspan },
	{ "HATTR_COMPACT", "COMPACT", 0, HMODE_ALL, NULL },
	{ "HATTR_CONTENT", "CONTENT", 0, HMODE_ALL, &hperm_content },
	{ "HATTR_COORDS", "COORDS", 0, HMODE_ALL, &hperm_coords },
	{ "HATTR_DATA", "DATA", 0, HMODE_ALL, &hperm_data },
	{ "HATTR_DATETIME", "DATETIME", 0, HMODE_ALL, &hperm_datetime },
	{ "HATTR_DECLARE", "DECLARE", EMPTY, HMODE_ALL, &hperm_declare },
	{ "HATTR_DEFER", "DEFER", EMPTY, HMODE_ALL, &hperm_defer },
	{ "HATTR_DIR", "DIR", 0, HMODE_ALL, &hperm_dir },
	{ "HATTR_DISABLED", "DISABLED", EMPTY, HMODE_ALL, &hperm_disabled },
	{ "HATTR_ENCODING", "ENCODING", 0, HMODE_ALL, NULL },
	{ "HATTR_ENCTYPE", "ENCTYPE", 0, HMODE_ALL, &hperm_enctype },
	{ "HATTR_FACE", "FACE", 0, HMODE_ALL, NULL },
	{ "HATTR_FOR", "FOR", 0, HMODE_ALL, &hperm_for },
	{ "HATTR_FRAME", "FRAME", 0, HMODE_ALL, &hperm_frame },
	{ "HATTR_FRAMEBORDER", "FRAMEBORDER", 0, HMODE_ALL, &hperm_frameborder },
	{ "HATTR_HEADERS", "HEADERS", 0, HMODE_ALL, &hperm_headers },
	{ "HATTR_HEIGHT", "HEIGHT", 0, HMODE_ALL, &hperm_height },
	{ "HATTR_HREF", "HREF", 0, HMODE_ALL, &hperm_href },
	{ "HATTR_HREFLANG", "HREFLANG", 0, HMODE_ALL, &hperm_hreflang },
	{ "HATTR_HSPACE", "HSPACE", 0, HMODE_ALL, NULL },
	{ "HATTR_HTTP_EQUIV", "HTTP-EQUIV", 0, HMODE_ALL, &hperm_http_equiv },
	{ "HATTR_ID", "ID", 0, HMODE_ALL, &hperm_id },
	{ "HATTR_ISMAP", "ISMAP", EMPTY, HMODE_ALL, &hperm_ismap },
	{ "HATTR_LABEL", "LABEL", 0, HMODE_ALL, &hperm_label },
	{ "HATTR_LANG", "LANG", 0, HMODE_ALL, &hperm_lang },
	{ "HATTR_LANGUAGE", "LANGUAGE", 0, HMODE_ALL, NULL },
	{ "HATTR_LINK", "LINK", 0, HMODE_ALL, NULL },
	{ "HATTR_LONGDESC", "LONGDESC", 0, HMODE_ALL, &hperm_longdesc },
	{ "HATTR_MARGINHEIGHT", "MARGINHEIGHT", 0, HMODE_ALL, &hperm_marginheight },
	{ "HATTR_MARGINWIDTH", "MARGINWIDTH", 0, HMODE_ALL, &hperm_marginwidth },
	{ "HATTR_MAX", "MAX", 0, HMODE_ALL, NULL },
	{ "HATTR_MAXLENGTH", "MAXLENGTH", 0, HMODE_ALL, &hperm_maxlength },
	{ "HATTR_MEDIA", "MEDIA", 0, HMODE_ALL, &hperm_media },
	{ "HATTR_METHOD", "METHOD", 0, HMODE_ALL, &hperm_method },
	{ "HATTR_MULTIPLE", "MULTIPLE", EMPTY, HMODE_ALL, &hperm_multiple },
	{ "HATTR_NAME", "NAME", 0, HMODE_ALL, &hperm_name },
	{ "HATTR_NOHREF", "NOHREF", EMPTY, HMODE_ALL, &hperm_nohref },
	{ "HATTR_NORESIZE", "NORESIZE", EMPTY, HMODE_ALL, &hperm_noresize },
	{ "HATTR_NOSHADE", "NOSHADE", EMPTY, HMODE_ALL, NULL },
	{ "HATTR_NOWRAP", "NOWRAP", EMPTY, HMODE_ALL, NULL },
	{ "HATTR_OBJECT", "OBJECT", 0, HMODE_ALL, NULL },
	{ "HATTR_ONBLUR", "ONBLUR", 0, HMODE_ALL, &hperm_onblur },
	{ "HATTR_ONCHANGE", "ONCHANGE", 0, HMODE_ALL, &hperm_onchange },
	{ "HATTR_ONCLICK", "ONCLICK", 0, HMODE_ALL, &hperm_onintrinsic },
	{ "HATTR_ONDBLCLICK", "ONDBLCLICK", 0, HMODE_ALL, &hperm_onintrinsic },
	{ "HATTR_ONFOCUS", "ONFOCUS", 0, HMODE_ALL, &hperm_onfocus },
	{ "HATTR_ONKEYDOWN", "ONKEYDOWN", 0, HMODE_ALL, &hperm_onintrinsic },
	{ "HATTR_ONKEYPRESS", "ONKEYPRESS", 0, HMODE_ALL, &hperm_onintrinsic },
	{ "HATTR_ONKEYUP", "ONKEYUP", 0, HMODE_ALL, &hperm_onintrinsic },
	{ "HATTR_ONLOAD", "ONLOAD", 0, HMODE_ALL, &hperm_onload },
	{ "HATTR_ONMOUSEDOWN", "ONMOUSEDOWN", 0, HMODE_ALL, &hperm_onintrinsic },
	{ "HATTR_ONMOUSEMOVE", "ONMOUSEMOVE", 0, HMODE_ALL, &hperm_onintrinsic },
	{ "HATTR_ONMOUSEOUT", "ONMOUSEOUT", 0, HMODE_ALL, &hperm_onintrinsic },
	{ "HATTR_ONMOUSEOVER", "ONMOUSEOVER", 0, HMODE_ALL, &hperm_onintrinsic },
	{ "HATTR_ONMOUSEUP", "ONMOUSEUP", 0, HMODE_ALL, &hperm_onintrinsic },
	{ "HATTR_ONRESET", "ONRESET", 0, HMODE_ALL, &hperm_onreset },
	{ "HATTR_ONSELECT", "ONSELECT", 0, HMODE_ALL, &hperm_onselect },
	{ "HATTR_ONSUBMIT", "ONSUBMIT", 0, HMODE_ALL, &hperm_onsubmit },
	{ "HATTR_ONUNLOAD", "ONUNLOAD", 0, HMODE_ALL, &hperm_onunload },
	{ "HATTR_PROFILE", "PROFILE", 0, HMODE_ALL, &hperm_profile },
	{ "HATTR_PROMPT", "PROMPT", 0, HMODE_ALL, NULL },
	{ "HATTR_READONLY", "READONLY", EMPTY, HMODE_ALL, &hperm_readonly },
	{ "HATTR_REL", "REL", 0, HMODE_ALL, &hperm_rel },
	{ "HATTR_REV", "REV", 0, HMODE_ALL, &hperm_rev },
	{ "HATTR_ROWS", "ROWS", 0, HMODE_ALL, &hperm_rows },
	{ "HATTR_ROWSPAN", "ROWSPAN", 0, HMODE_ALL, &hperm_rowspan },
	{ "HATTR_RULES", "RULES", 0, HMODE_ALL, &hperm_rules },
	{ "HATTR_SCHEME", "SCHEME", 0, HMODE_ALL, &hperm_scheme },
	{ "HATTR_SCOPE", "SCOPE", 0, HMODE_ALL, &hperm_scope },
	{ "HATTR_SCROLLING", "SCROLLING", 0, HMODE_ALL, &hperm_scrolling },
	{ "HATTR_SELECTED", "SELECTED" , EMPTY, HMODE_ALL, &hperm_selected },
	{ "HATTR_SHAPE", "SHAPE", 0, HMODE_ALL, &hperm_shape },
	{ "HATTR_SIZE", "SIZE", 0, HMODE_ALL, &hperm_size },
	{ "HATTR_SPAN", "SPAN", 0, HMODE_ALL, &hperm_span },
	{ "HATTR_SRC", "SRC", 0, HMODE_ALL, &hperm_src },
	{ "HATTR_STANDBY", "STANDBY", 0, HMODE_ALL, &hperm_standby },
	{ "HATTR_START", "START", 0, HMODE_ALL, &hperm_start },
	{ "HATTR_STYLE", "STYLE", 0, HMODE_ALL, &hperm_style },
	{ "HATTR_SUMMARY", "SUMMARY", 0, HMODE_ALL, &hperm_summary },
	{ "HATTR_TABINDEX", "TABINDEX", 0, HMODE_ALL, &hperm_tabindex },
	{ "HATTR_TARGET", "TARGET", 0, HMODE_ALL, &hperm_target },
	{ "HATTR_TEXT", "TEXT", 0, HMODE_ALL, NULL },
	{ "HATTR_TITLE", "TITLE", 0, HMODE_ALL, &hperm_title },
	{ "HATTR_TYPE", "TYPE", 0, HMODE_ALL, &hperm_type },
	{ "HATTR_USEMAP", "USEMAP", 0, HMODE_ALL, &hperm_usemap },
	{ "HATTR_VALIGN", "VALIGN", 0, HMODE_ALL, &hperm_valign },
	{ "HATTR_VALUE", "VALUE", 0, HMODE_ALL, &hperm_value },
	{ "HATTR_VALUETYPE", "VALUETYPE", 0, HMODE_ALL, &hperm_valuetype },
	{ "HATTR_VERSION", "VERSION", 0, HMODE_XML, &hperm_version },
	{ "HATTR_VLINK", "VLINK", 0, HMODE_ALL, NULL },
	{ "HATTR_VSPACE", "VSPACE", 0, HMODE_ALL, NULL },
	{ "HATTR_WIDTH", "WIDTH", 0, HMODE_ALL, &hperm_width },
	{ "HATTR_XMLNS", "XMLNS", 0, HMODE_XML, &hperm_xmlns },
};

static	const struct helemdef helemdefs[HELEM__MAX] = {
	{ "HELEM_A", "A", 0, HMODE_ALL, helem_excludes_a },
	{ "HELEM_ABBR", "ABBR", 0, HMODE_ALL, NULL },
	{ "HELEM_ACRONYM", "ACRONYM", 0, HMODE_ALL, NULL },
	{ "HELEM_ADDRESS", "ADDRESS", 0, HMODE_ALL, NULL },
	{ "HELEM_APPLET", "APPLET", 0, HMODE_ALL, NULL },
	{ "HELEM_AREA", "AREA", EMPTY, HMODE_ALL, NULL },
	{ "HELEM_B", "B", 0, HMODE_ALL, NULL },
	{ "HELEM_BASE", "BASE", EMPTY, HMODE_ALL, NULL },
	{ "HELEM_BASEFONT", "BASEFONT", EMPTY, HMODE_ALL, NULL },
	{ "HELEM_BDO", "BDO", 0, HMODE_ALL, NULL },
	{ "HELEM_BIG", "BIG", 0, HMODE_ALL, NULL },
	{ "HELEM_BLOCKQUOTE", "BLOCKQUOTE", 0, HMODE_ALL, NULL },
	{ "HELEM_BODY", "BODY", 0, HMODE_ALL, NULL },
	{ "HELEM_BR", "BR", EMPTY, HMODE_ALL, NULL },
	{ "HELEM_BUTTON", "BUTTON", 0, HMODE_ALL, helem_excludes_button },
	{ "HELEM_CAPTION", "CAPTION", 0, HMODE_ALL, NULL },
	{ "HELEM_CENTER", "CENTER", 0, HMODE_ALL, NULL },
	{ "HELEM_CITE", "CITE", 0, HMODE_ALL, NULL },
	{ "HELEM_CODE", "CODE", 0, HMODE_ALL, NULL },
	{ "HELEM_COL", "COL", EMPTY, HMODE_ALL, NULL },
	{ "HELEM_COLGROUP", "COLGROUP", 0, HMODE_ALL, NULL },
	{ "HELEM_DD", "DD", 0, HMODE_ALL, NULL },
	{ "HELEM_DEL", "DEL", 0, HMODE_ALL, NULL },
	{ "HELEM_DFN", "DFN", 0, HMODE_ALL, NULL },
	{ "HELEM_DIR", "DIR", 0, HMODE_ALL, NULL },
	{ "HELEM_DIV", "DIV", 0, HMODE_ALL, NULL },
	{ "HELEM_DL", "DL", 0, HMODE_ALL, NULL },
	{ "HELEM_DT", "DT", 0, HMODE_ALL, NULL },
	{ "HELEM_EM", "EM", 0, HMODE_ALL, NULL },
	{ "HELEM_FIELDSET", "FIELDSET", 0, HMODE_ALL, NULL },
	{ "HELEM_FONT", "FONT", 0, HMODE_ALL, NULL },
	{ "HELEM_FORM", "FORM", 0, HMODE_ALL, helem_excludes_form },
	{ "HELEM_FRAME", "FRAME", EMPTY, HMODE_ALL, NULL },
	{ "HELEM_FRAMESET", "FRAMESET", 0, HMODE_ALL, NULL },
	{ "HELEM_H1", "H1", 0, HMODE_ALL, NULL },
	{ "HELEM_H2", "H2", 0, HMODE_ALL, NULL },
	{ "HELEM_H3", "H3", 0, HMODE_ALL, NULL },
	{ "HELEM_H4", "H4", 0, HMODE_ALL, NULL },
	{ "HELEM_H5", "H5", 0, HMODE_ALL, NULL },
	{ "HELEM_H6", "H6", 0, HMODE_ALL, NULL },
	{ "HELEM_HEAD", "HEAD", 0, HMODE_ALL, NULL },
	{ "HELEM_HR", "HR", EMPTY, HMODE_ALL, NULL },
	{ "HELEM_HTML", "HTML", 0, HMODE_ALL, NULL },
	{ "HELEM_I", "I", 0, HMODE_ALL, NULL },
	{ "HELEM_IFRAME", "IFRAME", 0, HMODE_ALL, NULL },
	{ "HELEM_IMG", "IMG", EMPTY, HMODE_ALL, NULL },
	{ "HELEM_INPUT", "INPUT", EMPTY, HMODE_ALL, NULL },
	{ "HELEM_INS", "INS", 0, HMODE_ALL, NULL },
	{ "HELEM_ISINDEX", "ISINDEX", EMPTY, HMODE_ALL, NULL },
	{ "HELEM_KBD", "KBD", 0, HMODE_ALL, NULL },
	{ "HELEM_LABEL", "LABEL", 0, HMODE_ALL, helem_excludes_label },
	{ "HELEM_LEGEND", "LEGEND", 0, HMODE_ALL, NULL },
	{ "HELEM_LI", "LI", 0, HMODE_ALL, NULL },
	{ "HELEM_LINK", "LINK", EMPTY, HMODE_ALL, NULL },
	{ "HELEM_MAP", "MAP", 0, HMODE_ALL, NULL },
	{ "HELEM_MENU", "MENU", 0, HMODE_ALL, NULL },
	{ "HELEM_META", "META", EMPTY, HMODE_ALL, NULL },
	{ "HELEM_NOFRAMES", "NOFRAMES", 0, HMODE_ALL, NULL },
	{ "HELEM_NOSCRIPT", "NOSCRIPT", 0, HMODE_ALL, NULL },
	{ "HELEM_OBJECT", "OBJECT", 0, HMODE_ALL, NULL },
	{ "HELEM_OL", "OL", 0, HMODE_ALL, NULL },
	{ "HELEM_OPTGROUP", "OPTGROUP", 0, HMODE_ALL, NULL },
	{ "HELEM_OPTION", "OPTION", 0, HMODE_ALL, NULL },
	{ "HELEM_P", "P", 0, HMODE_ALL, NULL },
	{ "HELEM_PARAM", "PARAM", EMPTY, HMODE_ALL, NULL },
	{ "HELEM_PRE", "PRE", 0, HMODE_ALL, helem_excludes_pre },
	{ "HELEM_Q", "Q", 0, HMODE_ALL, NULL },
	{ "HELEM_S", "S", 0, HMODE_ALL, NULL },
	{ "HELEM_SAMP", "SAMP", 0, HMODE_ALL, NULL },
	{ "HELEM_SCRIPT", "SCRIPT", 0, HMODE_ALL, NULL },
	{ "HELEM_SELECT", "SELECT", 0, HMODE_ALL, NULL },
	{ "HELEM_SMALL", "SMALL", 0, HMODE_ALL, NULL },
	{ "HELEM_SPAN", "SPAN", 0, HMODE_ALL, NULL },
	{ "HELEM_STRIKE", "STRIKE", 0, HMODE_ALL, NULL },
	{ "HELEM_STRONG", "STRONG", 0, HMODE_ALL, NULL },
	{ "HELEM_STYLE", "STYLE", 0, HMODE_ALL, NULL },
	{ "HELEM_SUB", "SUB", 0, HMODE_ALL, NULL },
	{ "HELEM_SUP", "SUP", 0, HMODE_ALL, NULL },
	{ "HELEM_TABLE", "TABLE", 0, HMODE_ALL, NULL },
	{ "HELEM_TBODY", "TBODY", 0, HMODE_ALL, NULL },
	{ "HELEM_TD", "TD", 0, HMODE_ALL, NULL },
	{ "HELEM_TEXTAREA", "TEXTAREA", 0, HMODE_ALL, NULL },
	{ "HELEM_TFOOT", "TFOOT", 0, HMODE_ALL, NULL },
	{ "HELEM_TH", "TH", 0, HMODE_ALL, NULL },
	{ "HELEM_THEAD", "THEAD", 0, HMODE_ALL, NULL },
	{ "HELEM_TITLE", "TITLE", 0, HMODE_ALL, NULL },
	{ "HELEM_TR", "TR", 0, HMODE_ALL, NULL },
	{ "HELEM_TT", "TT", 0, HMODE_ALL, NULL },
	{ "HELEM_U", "U", 0, HMODE_ALL, NULL },
	{ "HELEM_UL", "UL", 0, HMODE_ALL, NULL },
	{ "HELEM_VAR", "VAR", 0, HMODE_ALL, NULL },
};


/*
 * Whether a tag is supposed to auto-close.  In SGML, this refers to
 * tags like <meta> and <br>, which do not have children.  In XML, this
 * refers to "EmptyElemTag" (empty elements), which can of course be
 * anything, so all XML derivatives will return true unilaterally.
 */
/* ARGSUSED */
int
helem_empty(enum helemt elem, enum hmode mode)
{

	return(helemdefs[elem].flags & EMPTY);
}


const char *
hattr_name(enum hattrt t)
{

	assert(t < HATTR__MAX);
	return(hattrdefs[t].name);
}


const char *
hattr_enumname(enum hattrt t)
{

	assert(t < HATTR__MAX);
	return(hattrdefs[t].enumname);
}


enum hattrt
hattr_find(const char *p)
{
	int		 i;

	/* TODO: write into a proper hashtable. */
	for (i = 0; i < (int)HATTR__MAX; i++)
		if (0 == strcasecmp(hattrdefs[i].name, p))
			return((enum hattrt)i);

	return(HATTR__MAX);
}


const char *
helem_name(enum helemt t)
{

	assert(t < HELEM__MAX);
	return(helemdefs[t].name);
}


enum helemt
helem_find(const char *p)
{
	int		 i;

	/* TODO: write into a proper hashtable. */
	for (i = 0; i < (int)HELEM__MAX; i++)
		if (0 == strcasecmp(helemdefs[i].name, p))
			return((enum helemt)i);

	return(HELEM__MAX);
}


/* ARGSUSED */
const enum helemt *
helem_excludes(enum helemt elem, enum hmode mode)
{

	assert(elem < HELEM__MAX);
	return(helemdefs[elem].excludes);
}


const char *
helem_enumname(enum helemt elem)
{

	assert(elem < HELEM__MAX);
	return(helemdefs[elem].enumname);
}


enum hdeclt
hdecl_find(const char *p)
{
	int		 i;

	/* TODO: write into a proper hashtable. */
	for (i = 0; i < (int)HDECL__MAX; i++)
		if (0 == strcmp(hdecldefs[i].name, p))
			return((enum hdeclt)i);

	return(HDECL__MAX);
}


const char *
hdecl_enumname(enum hdeclt t)
{

	assert(t < HDECL__MAX);
	return(hdecldefs[t].enumname);
}


const char *
hdecl_name(enum hdeclt t)
{

	assert(t < HDECL__MAX);
	return(hdecldefs[t].name);
}


const char *
hproc_enumname(enum hproct t)
{

	assert(t < HPROC__MAX);
	return(hprocdefs[t].enumname);
}


const char *
hproc_name(enum hproct t)
{

	assert(t < HPROC__MAX);
	return(hprocdefs[t].name);
}


enum hproct
hproc_find(const char *p)
{
	int		 i;

	/* TODO: write into a proper hashtable. */
	for (i = 0; i < (int)HPROC__MAX; i++)
		if (0 == strcmp(hprocdefs[i].name, p))
			return((enum hproct)i);

	return(HPROC__MAX);
}


enum hcreft
hcref_find(const char *p)
{
	int		 i;

	/* TODO: write into a proper hashtable. */
	for (i = 0; i < (int)HCREF__MAX; i++)
		if (0 == strcmp(hcrefdefs[i].name, p))
			return((enum hcreft)i);

	return(HCREF__MAX);
}


const char *
hcref_name(enum hcreft t)
{

	assert(t < HCREF__MAX);
	return(hcrefdefs[(int)t].name);
}


/*
 * Check permissions for an attribute.  This should be checked by a node
 * once its attributes have been validated: nodes may have either
 * allowed or disallowed attributes.
 */
int
hattr_elem_perm(enum hattrt attr, enum helemt elem, enum hmode mode)
{
	enum hindex	   i;
	int		   mm;
	const enum helemt *tt;

	/* By default: disallow. */

	assert(attr < HATTR__MAX);
	if (NULL == hattrdefs[(int)attr].perms)
		return(0);

	/* Only accept known modes.  TODO: clean this up. */

	if (mode & /* LINTED */ HTML_4_01)
		i = HINDEX_HTML_4_01;
	else if (mode & /* LINTED */ XHTML_1_0)
		i = HINDEX_XHTML_1_0;
	else
		abort();

	/* Check for allow-only clauses. */

	mm = hattrdefs[attr].perms->melem;
	switch (mm) {
	case (HPERM_ALLOW):
		tt = hattrdefs[attr].perms->pelem.allow[i];
		assert(tt);
		for ( ; HELEM__MAX != *tt; tt++)
			if (*tt == elem)
				return(1);
		return(0);
	case (HPERM_DENY):
		tt = hattrdefs[attr].perms->pelem.deny[i];
		assert(tt);
		for ( ; HELEM__MAX != *tt; tt++)
			if (*tt == elem)
				return(0);
		return(1);
	default:
		break;
	}

	return(0);
}


int
hattr_proc_perm(enum hattrt attr, enum hproct proc, enum hmode mode)
{
	enum hindex	   i;
	int		   mm;
	const enum hproct *tt;

	/* By default: disallow. */

	assert(attr < HATTR__MAX);
	if (NULL == hattrdefs[attr].perms)
		return(0);

	/* Only accept known modes.  TODO: clean this up. */

	if (mode & /* LINTED */ HTML_4_01)
		i = HINDEX_HTML_4_01;
	else if (mode & /* LINTED */ XHTML_1_0)
		i = HINDEX_XHTML_1_0;
	else
		abort();

	/* Check for allow-only clauses. */

	mm = hattrdefs[attr].perms->mproc;
	switch (mm) {
	case (HPERM_ALLOW):
		tt = hattrdefs[attr].perms->pproc.allow[i];
		assert(tt);
		for ( ; HPROC__MAX != *tt; tt++)
			if (*tt == proc)
				return(1);
		return(0);
	case (HPERM_DENY):
		tt = hattrdefs[attr].perms->pproc.deny[i];
		assert(tt);
		for ( ; HPROC__MAX != *tt; tt++)
			if (*tt == proc)
				return(0);
		return(1);
	default:
		break;
	}

	return(0);
}
