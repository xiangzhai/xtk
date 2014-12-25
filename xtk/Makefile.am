SUBDIRS = x11

xtkincludedir = $(includedir)/xtk-1.0
xtkinclude_HEADERS = xtk.h	\
					 xtkutil.h	\
					 xtktext.h

lib_LTLIBRARIES 	= libxtk.la
libxtk_la_SOURCES 	= xtkutil.cpp	\
					  xtktext.cpp
libxtk_la_CXXFLAGS 	= ${XTK_CFLAGS}
libxtk_la_LIBADD 	= ${XTK_LIBS}
libxtk_la_LIBADD    += x11/libxtk-x11.la

noinst_PROGRAMS		= test-window-x11	\
					  test-hello-x11

test_window_x11_SOURCES 	= test-window-x11.cpp
test_window_x11_CXXFLAGS 	= ${XTK_CFLAGS}	-Ix11
test_window_x11_LDADD 		= ${XTK_LIBS} libxtk.la

test_hello_x11_SOURCES 	= test-hello-x11.cpp
test_hello_x11_CXXFLAGS = ${XTK_CFLAGS} -Ix11
test_hello_x11_LDADD	= ${XTK_LIBS} libxtk.la
