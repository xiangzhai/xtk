// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "xtkwidget-x11.h"

#include <iostream>
#include <string.h>

namespace Xtk 
{

XtkWidgetX11::XtkWidgetX11(Display* display)
  : m_display(display) 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
}

XtkWidgetX11::~XtkWidgetX11() 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
}

void XtkWidgetX11::sendNETWMMessage(Window window, 
                                    std::string atomName, 
                                    unsigned long l0, 
                                    unsigned long l1, 
                                    unsigned long l2, 
                                    unsigned long l3, 
                                    unsigned long l4) 
{
    XClientMessageEvent event;                                                     
    event.type = ClientMessage;                                                    
    event.window = window;                                                         
    event.message_type = XInternAtom(m_display, atomName.c_str(), False);
    event.format = 32;                                                             
    event.data.l[0] = l0;                                                          
    event.data.l[1] = l1;                                                          
    event.data.l[2] = l2;                                                          
    event.data.l[3] = l3;                                                          
    event.data.l[4] = l4;                                                          
    XSendEvent(m_display, 
               DefaultRootWindow(m_display), 
               False, 
               SubstructureNotifyMask | SubstructureRedirectMask, 
               reinterpret_cast<XEvent*>(&event));
}

void XtkWidgetX11::activateWindow(Window window) 
{
    XWindowChanges wc;                                                             
    wc.stack_mode = Above;                                                         
    XConfigureWindow(m_display, window, CWStackMode, &wc);               
    sendNETWMMessage(window, "_NET_ACTIVE_WINDOW", 2, CurrentTime);
}

void XtkWidgetX11::minimizeWindow(Window window) 
{
    XIconifyWindow(m_display, window, DefaultScreen(m_display));
}

void XtkWidgetX11::closeWindow(Window window) 
{
    sendNETWMMessage(window, "_NET_CLOSE_WINDOW", CurrentTime, 2);
}

void XtkWidgetX11::quit(Window parent) 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    XClientMessageEvent event;
    event.type = ClientMessage;
    event.window = parent;
    event.message_type = XInternAtom(m_display, "WM_PROTOCOLS", False);
    event.format = 32;
    event.data.l[0] = XInternAtom(m_display, "WM_DELETE_WINDOW", False);
    event.data.l[1] = CurrentTime;
    XSendEvent(m_display, 
               parent, 
               False, 
               NoEventMask, 
               reinterpret_cast<XEvent*>(&event));
}

}
