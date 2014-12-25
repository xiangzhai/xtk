libxtkx11includedir = $(includedir)/xtk-1.0/x11
libxtkx11include_HEADERS = xtkdisplay-x11.h	\
						   xtkwidget-x11.h	\
						   xtkwindow-x11.h	\
						   xtkevent-x11.h	\
						   xtkbutton-x11.h

noinst_LTLIBRARIES 		= libxtk-x11.la
libxtk_x11_la_SOURCES 	= xtkdisplay-x11.cpp	\
						  xtkwidget-x11.cpp	\
						  xtkwindow-x11.cpp	\
						  xtkevent-x11.cpp	\
						  xtkbutton-x11.cpp
libxtk_x11_la_CXXFLAGS 	= ${XTK_CFLAGS} -I..
libxtk_x11_la_LIBADD 	= ${XTK_LIBS}
