.SUFFIXES: .in.3 .3 .3.sgml .sgml .html .c .c.sgml

WWWDIR	 = /usr/vhosts/libhtml.bsd.lv/www/htdocs
INSTDIR	 = /usr/local

CFLAGS	+= -W -Wall -Wstrict-prototypes -Wno-unused-parameter -Wwrite-strings -g

all: libhtml.a test html.3

lint: llib-llibhtml.ln llib-ltest.ln

www: index.html html.3.html libhtml.tar.gz libhtml.md5 test.c.html

LIBSRCS	= fd.c \
	  html.c \
	  iobuf.c \
	  ioctx.c \
	  ioout.c \
	  iovalid.c \
	  node.c \
	  parse.c \
	  validate.c \
	  write.c

LIBOBJS	= fd.o \
	  html.o \
	  iobuf.o \
	  ioctx.o \
	  ioout.o \
	  iovalid.o \
	  node.o \
	  parse.o \
	  validate.o \
	  write.o

LIBLNS	= fd.ln \
	  html.ln \
	  iobuf.ln \
	  ioctx.ln \
	  ioout.ln \
	  iovalid.ln \
	  node.ln \
	  parse.ln \
	  validate.ln \
	  write.ln

install: all
	mkdir -p $(INSTDIR)/lib
	mkdir -p $(INSTDIR)/include
	mkdir -p $(INSTDIR)/man/man3
	mkdir -p $(INSTDIR)/share/examples/libhtml
	install -m 0444 libhtml.a $(INSTDIR)/lib
	install -m 0444 html.h $(INSTDIR)/include
	install -m 0444 html.3 $(INSTDIR)/man/man3
	install -m 0444 test.c $(INSTDIR)/share/examples/libhtml

installwww: www
	mkdir -p $(WWWDIR)
	mkdir -p $(WWWDIR)/archive
	ver=`grep LIBHTML_VER html.h | \
	    sed -e 's!^[^\"]*!!' -e 's!\"!!g'` ; \
	    file=libhtml-$$ver.tar.gz ; \
	    install -m 0444 libhtml.tar.gz $(WWWDIR)/archive/$$file
	install -m 0444 index.html index.css external.png $(WWWDIR)
	install -m 0444 html.3.html style.css $(WWWDIR)
	install -m 0444 test.c.html $(WWWDIR)
	install -m 0444 libhtml.tar.gz libhtml.md5 $(WWWDIR)

clean:
	rm -f test.ln $(LIBLNS)
	rm -f test.o $(LIBOBJS)
	rm -f libhtml.a test html.3
	rm -f llib-llibhtml.ln llib-ltest.ln
	rm -f html.3.sgml html.3.html index.html
	rm -f libhtml.tar.gz libhtml.md5
	rm -f test.c.sgml test.c.html

test: test.o libhtml.a
	$(CC) -o $@ test.o libhtml.a

libhtml.tar.gz:
	mkdir -p .dist/libhtml
	install -m 0444 Makefile .dist/libhtml
	install -m 0444 html.in.3 style.css .dist/libhtml
	install -m 0444 $(LIBSRCS) .dist/libhtml
	install -m 0444 html.h extern.h .dist/libhtml
	install -m 0444 test.c .dist/libhtml
	install -m 0444 index.sgml index.css external.png .dist/libhtml
	(cd .dist && tar zcf ../$@ libhtml)
	rm -rf .dist

libhtml.md5: libhtml.tar.gz
	md5 libhtml.tar.gz >$@

llib-llibhtml.ln: $(LIBLNS)
	$(LINT) -Clibhtml $(LIBLNS)

llib-ltest.ln: test.ln
	$(LINT) -Ctest test.ln

libhtml.a: $(LIBOBJS)
	$(AR) rs $@ $(LIBOBJS)

$(LIBOBJS): html.h extern.h

html.3.html: html.3.sgml

test.c.html: test.c.sgml

.in.3.3:
	mandoc -Tlint -Wstop $<
	cp -f $< $@

.3.3.sgml:
	mandoc -Thtml -Ostyle=style.css $< >$@

.c.c.sgml:
	highlight -I $< >$@

.sgml.html:
	validate --warn $<
	ver=`grep LIBHTML_VER html.h | \
	    sed -e 's!^[^\"]*!!' -e 's!\"!!g'` ; \
	    sed -e "s!@LIBHTML_VER@!$$ver!g" $< >$@
