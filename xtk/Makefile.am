SUBDIRS = x11

xtkincludedir = $(includedir)/xtk-1.0
xtkinclude_HEADERS = xtk.h	\
					 xtkutil.h	\
					 xtktheme.h	\
					 xtktext.h	\
					 xtkimage.h

lib_LTLIBRARIES 	= libxtk.la
libxtk_la_SOURCES 	= xtkutil.cpp	\
					  xtktheme.cpp	\
					  xtktext.cpp	\
					  xtkimage.cpp
libxtk_la_CXXFLAGS 	= ${XTK_CFLAGS}	-DDATADIR=\""$(datadir)/xtk"\"
libxtk_la_LIBADD 	= ${XTK_LIBS} ${INIPARSER_LIBS}
libxtk_la_LIBADD    += x11/libxtk-x11.la

noinst_PROGRAMS		= test-window-x11	\
					  test-hello-x11

test_window_x11_SOURCES 	= test-window-x11.cpp
test_window_x11_CXXFLAGS 	= ${XTK_CFLAGS} -Ix11
test_window_x11_LDADD 		= libxtk.la

test_hello_x11_SOURCES 	= test-hello-x11.cpp
test_hello_x11_CXXFLAGS = ${XTK_CFLAGS} -Ix11
test_hello_x11_LDADD	= libxtk.la
