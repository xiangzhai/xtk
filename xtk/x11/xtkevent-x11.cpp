// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "xtkevent-x11.h"

#include <iostream>

namespace Xtk 
{

XtkEventX11::XtkEventX11(Display* display, XtkWindowX11* window)
  : m_display(display), 
    m_window(window) 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    
}

XtkEventX11::~XtkEventX11() 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
}

void XtkEventX11::connect(XtkWidgetX11* widget) { widgets.push_back(widget); }

void XtkEventX11::run() 
{
    XEvent event;

    while (quit == false && !XNextEvent(m_display, &event)) {
        switch (event.type) {
        case ClientMessage:
            if ((Atom)event.xclient.data.l[0] == m_window->wmDeleteMessage()) 
                quit = true;
            break;
        case Expose:
            m_window->draw();
            break;
        case ConfigureNotify:
            m_window->resize(event.xconfigure.width, event.xconfigure.height);
            m_window->draw();
            break;
        case EnterNotify:
            for (unsigned int i = 0; i < widgets.size(); i++) {
                if (widgets[i]->window() == event.xbutton.window) {
                    widgets[i]->enterNotify();
                    break;
                }
            }
            break;
        case LeaveNotify:
            for (unsigned int i = 0; i < widgets.size(); i++) {
                if (widgets[i]->window() == event.xbutton.window) {
                    widgets[i]->leaveNotify();
                    break;
                }
            }
            break;
        case ButtonPress:
            for (unsigned int i = 0; i < widgets.size(); i++) {
                if (widgets[i]->window() == event.xbutton.window) {
                    widgets[i]->buttonPress();
                    break;
                }
            }
            break;
        default:
            break;
        }
    }
}

};
