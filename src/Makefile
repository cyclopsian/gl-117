# Makefile.in generated automatically by automake 1.5 from Makefile.am.

# Copyright 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001
# Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.



SHELL = /bin/sh

srcdir = .
top_srcdir = ..

prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include
pkgdatadir = $(datadir)/gl-117
pkglibdir = $(libdir)/gl-117
pkgincludedir = $(includedir)/gl-117
top_builddir = ..

ACLOCAL = ${SHELL} /home/tom/gl-117-0.6.1/missing --run aclocal
AUTOCONF = ${SHELL} /home/tom/gl-117-0.6.1/missing --run autoconf
AUTOMAKE = ${SHELL} /home/tom/gl-117-0.6.1/missing --run automake
AUTOHEADER = ${SHELL} /home/tom/gl-117-0.6.1/missing --run autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
INSTALL_HEADER = $(INSTALL_DATA)
transform = s,x,x,
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
AMTAR = ${SHELL} /home/tom/gl-117-0.6.1/missing --run tar
AWK = gawk
CC = gcc
CXX = g++
DEPDIR = .deps
EXEEXT = 
INSTALL_STRIP_PROGRAM = ${SHELL} $(install_sh) -c -s
OBJEXT = o
PACKAGE = gl-117
VERSION = 0.5
am__include = include
am__quote = 
install_sh = /home/tom/gl-117-0.6.1/install-sh

AUTOMAKE_OPTIONS = gnu
bin_PROGRAMS = gl-117
gl_117_SOURCES = main.cpp gl.cpp model.cpp 3ds.cpp audio.cpp land.cpp glland.cpp aiobject.cpp effects.cpp roam.cpp net.cpp mathtab.cpp dirs.cpp objload.cpp pilots.cpp loader_tga.cpp
noinst_HEADERS = 
DEFS = -I. -I..
subdir = src
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = $(top_builddir)/config.h
CONFIG_CLEAN_FILES =
bin_PROGRAMS = gl-117$(EXEEXT)
PROGRAMS = $(bin_PROGRAMS)

am_gl_117_OBJECTS = main.$(OBJEXT) gl.$(OBJEXT) model.$(OBJEXT) \
	3ds.$(OBJEXT) audio.$(OBJEXT) land.$(OBJEXT) glland.$(OBJEXT) \
	aiobject.$(OBJEXT) effects.$(OBJEXT) roam.$(OBJEXT) \
	net.$(OBJEXT) mathtab.$(OBJEXT) dirs.$(OBJEXT) \
	objload.$(OBJEXT) pilots.$(OBJEXT) loader_tga.$(OBJEXT)
gl_117_OBJECTS = $(am_gl_117_OBJECTS)
gl_117_LDADD = $(LDADD)
gl_117_DEPENDENCIES =
gl_117_LDFLAGS =
DEFAULT_INCLUDES =  -I. -I$(srcdir) -I$(top_builddir)
CPPFLAGS = -DDATADIR='"${prefix}/share"'
LDFLAGS =  -lm
LIBS =  -lGL -lGLU -lglut -lSDL -lSDLmain -lSDL_mixer
depcomp = $(SHELL) $(top_srcdir)/depcomp
DEP_FILES = $(DEPDIR)/3ds.Po $(DEPDIR)/aiobject.Po \
	$(DEPDIR)/audio.Po $(DEPDIR)/dirs.Po \
	$(DEPDIR)/effects.Po $(DEPDIR)/gl.Po \
	$(DEPDIR)/glland.Po $(DEPDIR)/land.Po \
	$(DEPDIR)/loader_tga.Po $(DEPDIR)/main.Po \
	$(DEPDIR)/mathtab.Po $(DEPDIR)/model.Po \
	$(DEPDIR)/net.Po $(DEPDIR)/objload.Po \
	$(DEPDIR)/pilots.Po $(DEPDIR)/roam.Po
CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
CXXLD = $(CXX)
CXXLINK = $(CXXLD) $(AM_CXXFLAGS) $(CXXFLAGS) $(AM_LDFLAGS) $(LDFLAGS) \
	-o $@
CXXFLAGS = -g -O2
DIST_SOURCES = $(gl_117_SOURCES)
HEADERS = $(noinst_HEADERS)

DIST_COMMON = $(noinst_HEADERS) Makefile.am Makefile.in
SOURCES = $(gl_117_SOURCES)

all: all-am

.SUFFIXES:
.SUFFIXES: .cpp .o .obj
$(srcdir)/Makefile.in:  Makefile.am  $(top_srcdir)/configure.in $(ACLOCAL_M4)
	cd $(top_srcdir) && \
	  $(AUTOMAKE) --gnu  src/Makefile
Makefile:  $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) && \
	  CONFIG_HEADERS= CONFIG_LINKS= \
	  CONFIG_FILES=$(subdir)/$@ $(SHELL) ./config.status
install-binPROGRAMS: $(bin_PROGRAMS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@list='$(bin_PROGRAMS)'; for p in $$list; do \
	  p1=`echo $$p|sed 's/$(EXEEXT)$$//'`; \
	  if test -f $$p \
	  ; then \
	    f=`echo $$p1|sed '$(transform);s/$$/$(EXEEXT)/'`; \
	   echo " $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/$$f"; \
	   $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/$$f; \
	  else :; fi; \
	done

uninstall-binPROGRAMS:
	@$(NORMAL_UNINSTALL)
	@list='$(bin_PROGRAMS)'; for p in $$list; do \
	  f=`echo $$p|sed 's/$(EXEEXT)$$//;$(transform);s/$$/$(EXEEXT)/'`; \
	  echo " rm -f $(DESTDIR)$(bindir)/$$f"; \
	  rm -f $(DESTDIR)$(bindir)/$$f; \
	done

clean-binPROGRAMS:
	-test -z "$(bin_PROGRAMS)" || rm -f $(bin_PROGRAMS)
gl-117$(EXEEXT): $(gl_117_OBJECTS) $(gl_117_DEPENDENCIES) 
	@rm -f gl-117$(EXEEXT)
	$(CXXLINK) $(gl_117_LDFLAGS) $(gl_117_OBJECTS) $(gl_117_LDADD) $(LIBS)

mostlyclean-compile:
	-rm -f *.$(OBJEXT) core *.core

distclean-compile:
	-rm -f *.tab.c

include $(DEPDIR)/3ds.Po
include $(DEPDIR)/aiobject.Po
include $(DEPDIR)/audio.Po
include $(DEPDIR)/dirs.Po
include $(DEPDIR)/effects.Po
include $(DEPDIR)/gl.Po
include $(DEPDIR)/glland.Po
include $(DEPDIR)/land.Po
include $(DEPDIR)/loader_tga.Po
include $(DEPDIR)/main.Po
include $(DEPDIR)/mathtab.Po
include $(DEPDIR)/model.Po
include $(DEPDIR)/net.Po
include $(DEPDIR)/objload.Po
include $(DEPDIR)/pilots.Po
include $(DEPDIR)/roam.Po

distclean-depend:
	-rm -rf $(DEPDIR)

.cpp.o:
	source='$<' object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Po' tmpdepfile='$(DEPDIR)/$*.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXXCOMPILE) -c -o $@ `test -f $< || echo '$(srcdir)/'`$<

.cpp.obj:
	source='$<' object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Po' tmpdepfile='$(DEPDIR)/$*.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXXCOMPILE) -c -o $@ `cygpath -w $<`
CXXDEPMODE = depmode=gcc
uninstall-info-am:

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP) $(TAGS_FILES)
	list='$(SOURCES) $(HEADERS) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	mkid -fID $$unique $(LISP)

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) \
		$(TAGS_FILES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)$$unique$(LISP)$$tags" \
	  || etags $(ETAGS_ARGS) $$tags  $$unique $(LISP)

GTAGS:
	here=`CDPATH=: && cd $(top_builddir) && pwd` \
	  && cd $(top_srcdir) \
	  && gtags -i $(GTAGS_ARGS) $$here

distclean-tags:
	-rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH

DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)

top_distdir = ..
distdir = $(top_distdir)/$(PACKAGE)-$(VERSION)

distdir: $(DISTFILES)
	@for file in $(DISTFILES); do \
	  if test -f $$file; then d=.; else d=$(srcdir); fi; \
	  dir=`echo "$$file" | sed -e 's,/[^/]*$$,,'`; \
	  if test "$$dir" != "$$file" && test "$$dir" != "."; then \
	    $(mkinstalldirs) "$(distdir)/$$dir"; \
	  fi; \
	  if test -d $$d/$$file; then \
	    cp -pR $$d/$$file $(distdir) \
	    || exit 1; \
	  else \
	    test -f $(distdir)/$$file \
	    || cp -p $$d/$$file $(distdir)/$$file \
	    || exit 1; \
	  fi; \
	done
check-am: all-am
check: check-am
all-am: Makefile $(PROGRAMS) $(HEADERS)

installdirs:
	$(mkinstalldirs) $(DESTDIR)$(bindir)

install: install-am
install-exec: install-exec-am
install-data: install-data-am
uninstall: uninstall-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	  `test -z '$(STRIP)' || \
	    echo "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'"` install
mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES) stamp-h stamp-h[0-9]*

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-am

clean-am: clean-binPROGRAMS clean-generic mostlyclean-am

distclean: distclean-am

distclean-am: clean-am distclean-compile distclean-depend \
	distclean-generic distclean-tags

dvi: dvi-am

dvi-am:

info: info-am

info-am:

install-data-am:

install-exec-am: install-binPROGRAMS

install-info: install-info-am

install-man:

installcheck-am:

maintainer-clean: maintainer-clean-am

maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-am

mostlyclean-am: mostlyclean-compile mostlyclean-generic

uninstall-am: uninstall-binPROGRAMS uninstall-info-am

.PHONY: GTAGS all all-am check check-am clean clean-binPROGRAMS \
	clean-generic distclean distclean-compile distclean-depend \
	distclean-generic distclean-tags distdir dvi dvi-am info \
	info-am install install-am install-binPROGRAMS install-data \
	install-data-am install-exec install-exec-am install-info \
	install-info-am install-man install-strip installcheck \
	installcheck-am installdirs maintainer-clean \
	maintainer-clean-generic mostlyclean mostlyclean-compile \
	mostlyclean-generic tags uninstall uninstall-am \
	uninstall-binPROGRAMS uninstall-info-am

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
