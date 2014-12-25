// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "xtkwindow-x11.h"
#include "xtkutil.h"

#include <iostream>

namespace Xtk 
{

XtkWindowX11::XtkWindowX11(Display* display, 
                           int x, 
                           int y, 
                           int width, 
                           int height, 
                           std::string name, 
                           std::string color, 
                           Window parent, 
                           int border_width, 
                           X11WindowType type)
  : XtkWidgetX11(display), 
    m_display(display), 
    m_width(width), 
    m_height(height),
    m_name(name), 
    m_color(color) 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    XVisualInfo vinfo;
    XMatchVisualInfo(m_display, DefaultScreen(m_display), 32, TrueColor, &vinfo);
    m_visual = vinfo.visual;

    // window attribute
    XSetWindowAttributes attr;
    attr.override_redirect = False;
    attr.colormap = XCreateColormap(m_display, 
                        parent == None ? DefaultRootWindow(m_display) : parent, 
                        vinfo.visual, AllocNone);
    attr.border_pixel = 0;
    attr.background_pixel = 0;
    attr.event_mask = ExposureMask | PropertyChangeMask | StructureNotifyMask |
                      ButtonReleaseMask | ButtonPressMask | ButtonMotionMask | 
                      EnterWindowMask | LeaveWindowMask | 
                      KeyPressMask;
    
    // create window
    m_window = XCreateWindow(m_display, 
            parent == None ? DefaultRootWindow(m_display) : parent, 
            x, y, m_width, m_height, border_width, vinfo.depth, 
            CopyFromParent, /* class */
            m_visual, 
            CWOverrideRedirect | CWColormap | CWBorderPixel | CWBackPixel | 
            CWEventMask, 
            &attr);

    // window name
    if (m_name == "") 
        m_name = "xtk";
    XStoreName(m_display, m_window, m_name.c_str());

    // window manager delete message
    m_wmDeleteMessage = XInternAtom(m_display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(m_display, m_window, &m_wmDeleteMessage, 1);

    if (type == _NET_WM_WINDOW_TYPE_FRAMELESS) {
        Atom mwmHintsProperty = XInternAtom(m_display, "_MOTIF_WM_HINTS", False);
        struct MwmHints hints;

        hints.flags = MWM_HINTS_DECORATIONS;
        hints.decorations = 0;
        XChangeProperty(m_display, m_window, mwmHintsProperty, 
                        mwmHintsProperty, 32, PropModeReplace, 
                        (unsigned char*) &hints, 5);
    } else if (type == _NET_WM_WINDOW_TYPE_DESKTOP) {
        Atom type, state[4];

        // sticky and below other window
        type = 0xFFFFFFFF;
        XChangeProperty(m_display, m_window, 
                        XInternAtom(m_display, "_NET_WM_DESKTOP", False), 
                        XA_CARDINAL, 32, PropModeReplace, 
                        (unsigned char*) &type, 1);
        state[0] = XInternAtom(m_display, "_NET_WM_STATE_SKIP_PAGER", False);
        state[1] = XInternAtom(m_display, "_NET_WM_STATE_SKIP_TASKBAR", False);
        state[2] = XInternAtom(m_display, "_NET_WM_STATE_STICKY", False);
        state[3] = XInternAtom(m_display, "_NET_WM_STATE_BELOW", False);
        XChangeProperty(m_display, m_window, 
                        XInternAtom(display, "_NET_WM_STATE", False), 
                        XA_ATOM, 32, PropModeReplace, 
                        (unsigned char*) state, 4);
    } else if (type == _NET_WM_WINDOW_TYPE_DOCK) {
        long struts[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        Atom type;
        XSizeHints size_hints;
        XWMHints wmhints;

        // window type
        type = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_DOCK", False);
        XChangeProperty(m_display, m_window,  
                        XInternAtom(m_display, "_NET_WM_WINDOW_TYPE", False), 
                        XA_ATOM, 
                        32, // format 32-bit
                        PropModeReplace, // mode
                        (unsigned char*) &type, // data
                        1); // the number of elements
    
        // reserved space
#if POSITION_TOP
        struts[2] = height;
        struts[8] = 0;
        struts[9] = 0 + m_width;
#else
        struts[3] = height;
        struts[10] = 0;
        struts[11] = 0 + m_width;
#endif
        XChangeProperty(m_display, m_window, 
                        XInternAtom(m_display, "_NET_WM_STRUT", False), 
                        XA_CARDINAL, 32, PropModeReplace, 
                        (unsigned char*) &struts, 4);
        XChangeProperty(m_display, m_window, 
                        XInternAtom(m_display, "_NET_WM_STRUT_PARTIAL", False), 
                        XA_CARDINAL, 32, PropModeReplace, 
                        (unsigned char*) &struts, 12);

        // fixed position
        size_hints.flags = PPosition;
        XChangeProperty(m_display, m_window, XA_WM_NORMAL_HINTS, 
                        XA_WM_SIZE_HINTS, 32, PropModeReplace, 
                        (unsigned char*) &size_hints, sizeof(XSizeHints) / 4);
   
        // unfocusable
        wmhints.flags = InputHint;
        wmhints.input = False;
        XChangeProperty(m_display, m_window, XA_WM_HINTS, XA_WM_HINTS, 
                        32, PropModeReplace, 
                        (unsigned char*) &wmhints, sizeof(XWMHints) / 4);
    }

    XMapWindow(m_display, m_window);

    // cairo surface
    m_surface = cairo_xlib_surface_create(m_display, 
                                          m_window, // Drawable 
                                          m_visual, 
                                          0, 0);
    if (m_surface) { 
        cairo_xlib_surface_set_size(m_surface, m_width, m_height);
        context = cairo_create(m_surface);
    }
}

XtkWindowX11::~XtkWindowX11() 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    if (context) {
        cairo_destroy(context);
        context = nullptr;
    }

    if (m_surface) {
        cairo_surface_destroy(m_surface);
        m_surface = nullptr;
    }

    if (m_display) {
        if (m_window != None) 
            XDestroyWindow(m_display, m_window);
    }
}

void XtkWindowX11::resize(int width, int height) 
{
    m_width = width;
    m_height = height;
    cairo_xlib_surface_set_size(m_surface, m_width, m_height);
}

void XtkWindowX11::draw() { this->swap(); }

void XtkWindowX11::swap(double alpha) 
{
    double r, g, b;

    colorHtmlToCairo(m_color, r, g, b);
    cairo_save(context);
    cairo_set_source_rgba(context, r, g, b, alpha);
    cairo_set_operator(context, CAIRO_OPERATOR_SOURCE);
    cairo_paint(context);
    cairo_restore(context);
}

}
