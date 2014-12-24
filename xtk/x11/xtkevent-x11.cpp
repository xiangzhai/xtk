// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "xtkevent-x11.h"

namespace Xtk 
{

XtkEventX11::XtkEventX11(Display* display, XtkWindowX11* window)
  : m_display(display), 
    m_window(window) 
{
    XEvent event;
    while (quit == false && !XNextEvent(m_display, &event)) {
        switch (event.type) {
        case ClientMessage:
            if ((Atom)event.xclient.data.l[0] == window->wmDeleteMessage()) 
                quit = true;
            break;
        case Expose:
            window->swap();
            break;
        case ConfigureNotify:
            window->resize(event.xconfigure.width, event.xconfigure.height);
            window->swap();
            break;
        default:
            break;
        }
    }
}

XtkEventX11::~XtkEventX11() 
{
}

};
