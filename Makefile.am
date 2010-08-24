ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = @DEPS_CFLAGS@

EXTRA_DIST = obj_layout.c obj_font_desc.c

lualibdir = @lualibdir@

lib_LTLIBRARIES = liboopango.la
liboopango_la_SOURCES = oopango.c
liboopango_la_LDFLAGS = -version 0:0:0
liboopango_la_LIBADD = @DEPS_LIBS@

include_HEADERS = oopango.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = oopango.pc

# Make this library loadable by lua
install-exec-hook:
	$(MKDIR_P) $(DESTDIR)$(lualibdir)
	-rm -f $(DESTDIR)$(lualibdir)/oopango.so
	$(LN_S) $(libdir)/liboopango.so $(DESTDIR)$(lualibdir)/oopango.so
uninstall-local:
	-rm -f $(DESTDIR)$(lualibdir)/oopango.so
