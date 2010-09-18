ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = @DEPS_CFLAGS@

EXTRA_DIST = obj_layout.c obj_font_desc.c obj_font_map.c pango_cairo.c

lualibdir = $(LUALIBDIR)

lib_LTLIBRARIES = liboopango.la
liboopango_la_SOURCES = oopango.c
liboopango_la_LDFLAGS = -version 0:0:0
liboopango_la_LIBADD = @DEPS_LIBS@

include_HEADERS = oopango.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = oopango.pc

# Make this library loadable by lua
install-exec-hook:
	$(MKDIR_P) $(DESTDIR)$(LUALIBDIR)
	-rm -f $(DESTDIR)$(LUALIBDIR)/oopango.so
	$(LN_S) $(libdir)/liboopango.so $(DESTDIR)$(LUALIBDIR)/oopango.so
uninstall-local:
	-rm -f $(DESTDIR)$(LUALIBDIR)/oopango.so
