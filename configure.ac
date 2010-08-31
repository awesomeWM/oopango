AC_INIT([oopango], [0.9], [psychon@znc.in])
AC_CONFIG_SRCDIR([oopango.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])
LT_INIT([disable-static])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_LN_S
PKG_CHECK_MODULES(DEPS, lua5.1 cairo oocairo >= 1.3 pangocairo)

AC_MSG_CHECKING([lua library dir])
# First, we need to figure out the prefix
MY_PREFIX=${prefix}
if test "x${MY_PREFIX}" = "xNONE" ; then
	MY_PREFIX=${ac_default_prefix}
fi
lualibdir=`$PKG_CONFIG lua5.1 --define-variable=prefix=${MY_PREFIX} --variable=INSTALL_CMOD`
AC_SUBST([lualibdir])
AC_MSG_RESULT([$lualibdir])

AC_CONFIG_FILES([Makefile oopango.pc oopango-uninstalled.pc])
AC_OUTPUT
