AC_INIT([xtk], [1.0], [xiang.zhai@i-soft.com.cn])
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror foreign])
AC_PROG_CXX
AM_PROG_AR
AM_PROG_LIBTOOL
AM_MAINTAINER_MODE
AC_CONFIG_MACRO_DIR([m4])
LT_INIT

CXXFLAGS='-Wall -Werror -std=c++0x'

AC_CHECK_LIB(iniparser, iniparser_load, HAVE_INIPARSER=yes, HAVE_INIPARSE=no)
if test "$HAVE_INIPARSE" = "no"; then                                               
 AC_MSG_ERROR([iniparser is required])                                               
fi                                                                                 
INIPARSER_LIBS=-liniparser
AC_SUBST(INIPARSER_LIBS)

X11_REQUIRED=1.6.2
XTST_REQUIRED=1.2.2
XDAMAGE_REQUIRED=1.1.4
CAIRO_XLIB_REQUIRED=1.12.16

XTK_PACKAGES="x11 >= $X11_REQUIRED xtst >= $XTST_REQUIRED xdamage >= $XDAMAGE_REQUIRED cairo-xlib >= $CAIRO_XLIB_REQUIRED"
PKG_CHECK_MODULES(XTK, $XTK_PACKAGES)
AC_SUBST(XTK_PACKAGES)
AC_SUBST(XTK_CFLAGS)
AC_SUBST(XTK_LIBS)

AC_ARG_ENABLE(debug,                                                                
              [AS_HELP_STRING([--enable-debug=@<:@no/yes@:>@],              
                              [turn on debugging [default=no]])],,
              [enable_debug=no])

if test "x$enable_debug" = "xyes"; then                                             
  test "$cflags_set" = set || CXXFLAGS="$CXXFLAGS -g -DXTK_DEBUG" 
fi

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 xtk-1.0.pc
 data/Makefile
 xtk/x11/Makefile
 xtk/Makefile
])
AC_OUTPUT
