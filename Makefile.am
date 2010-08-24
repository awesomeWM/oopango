ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = @DEPS_CFLAGS@

EXTRA_DIST = obj_layout.c obj_font_desc.c

lualibdir = @lualibdir@

lib_LTLIBRARIES = liboopango.la
liboopango_la_SOURCES = oopango.c
liboopango_la_LDFLAGS = -version 0:0:0
liboopango_la_LIBADD = @DEPS_LIBS@

include_HEADERS = oopango.h

# Make this library loadable by lua
install-exec-hook:
	$(MKDIR_P) $(DESTDIR)$(lualibdir)
	-rm -f $(DESTDIR)$(lualibdir)/oocairo.so
	$(LN_S) $(libdir)/liboocairo.so $(DESTDIR)$(lualibdir)/oocairo.so
uninstall-local:
	-rm -f $(DESTDIR)$(lualibdir)/oocairo.so
