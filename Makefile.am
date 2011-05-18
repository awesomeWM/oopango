ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = @DEPS_CFLAGS@

EXTRA_DIST = obj_layout.c obj_font_desc.c obj_font_map.c pango_cairo.c

lualibdir = $(LUALIBDIR)

lib_LTLIBRARIES = liboopango.la
liboopango_la_SOURCES = oopango.c
liboopango_la_LDFLAGS = -version 0:0:0 -no-undefined
liboopango_la_LIBADD = @DEPS_LIBS@

include_HEADERS = oopango.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = oopango.pc

# TODO: How can this be fixed to work everywhere?
FOO='/home/psychon/projects/lua-oocairo/.libs/lib?.so'
FOP='$(LUALIBDIR)/?.so'
TESTS_ENVIRONMENT = echo 'lunit.main({...})' | LUA_PATH='@srcdir@/?.lua' LUA_CPATH='.libs/lib?.so;$(FOO);$(FOP)' lua -llunit -
TESTS  = test/font_desc.lua
TESTS += test/font_map.lua
TESTS += test/general.lua
TESTS += test/layout.lua
EXTRA_DIST += $(TESTS) lunit.lua lunit-console.lua test-loading.lua

# Make this library loadable by lua
install-exec-hook:
	$(MKDIR_P) $(DESTDIR)$(LUALIBDIR)
	-rm -f $(DESTDIR)$(LUALIBDIR)/oopango.so
	$(LN_S) $(libdir)/liboopango.so $(DESTDIR)$(LUALIBDIR)/oopango.so
uninstall-local:
	-rm -f $(DESTDIR)$(LUALIBDIR)/oopango.so

# Test whether loading oopango works
installcheck-local:
	LUA_PATH='${srcdir}/?.lua' LUA_CPATH='${LUALIBDIR}/?.so' lua ${srcdir}/test-loading.lua
