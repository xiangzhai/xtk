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


noinst_PROGRAMS		= test_window


test_window_SOURCES 	= test_window.cpp
test_window_CXXFLAGS 	= ${XTK_CFLAGS}	-Ix11
test_window_LDADD 		= ${XTK_LIBS} libxtk.la
