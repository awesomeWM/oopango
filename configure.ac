AC_INIT([oopango], [1.1], [psychon@znc.in])
AC_CONFIG_SRCDIR([oopango.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])
LT_INIT([disable-static])
AM_INIT_AUTOMAKE([-Wall -Werror foreign dist-bzip2])

WFLAGS="-Wall -Wextra -Wcast-align -Wmissing-declarations -Winit-self -Wundef"
WFLAGS="$WFLAGS -Wredundant-decls -Wwrite-strings -Wformat=2 -Wlogical-op"
WFLAGS="$WFLAGS -Wmissing-include-dirs -Wformat-nonliteral -Wformat-security"
WFLAGS="$WFLAGS -Wmissing-format-attribute -Wstrict-aliasing -pedantic"

CC_CHECK_CFLAGS_APPEND([$WFLAGS])
CC_NOUNDEFINED
AC_PROG_CC_C99
LDFLAGS="$LDFLAGS $LDFLAGS_NOUNDEFINED"

# Lua gets different names in different distros :(
PKG_PROG_PKG_CONFIG
AC_CACHE_CHECK([for lua], [oopango_cv_lua_name],
    [for lua_name in lua lua5.1 lua5.2
    do
      PKG_CHECK_EXISTS([$lua_name], [oopango_cv_lua_name=$lua_name ; break])
    done
    ])
LUA_NAME=$oopango_cv_lua_name
AS_IF([test "x$LUA_NAME" = "x"],
      [AC_ERROR([Could not find lua via pkg-config!]) ])

AC_PROG_LN_S
PKG_CHECK_MODULES([DEPS], [$LUA_NAME cairo oocairo >= 1.3 pangocairo])
LUA_LIBDIR([$LUA_NAME], [AC_SUBST([LUALIBDIR], [$VALUE])])

AC_CONFIG_FILES([Makefile oopango.pc oopango-uninstalled.pc])
AC_OUTPUT
