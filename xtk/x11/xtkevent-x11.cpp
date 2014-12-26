// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "xtkevent-x11.h"

#include <iostream>

namespace Xtk 
{

XtkEventX11::XtkEventX11(Display* display, XtkWindowX11* parent)
  : m_display(display), 
    m_parent(parent) 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    widgets.push_back(m_parent);
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
            if ((Atom)event.xclient.data.l[0] == m_parent->wmDeleteMessage()) {
                if (event.xclient.window == m_parent->window()) {
                    quit = true;
                    break;
                } else {
                    for (unsigned int i = 0; i < widgets.size(); i++) {
                        if (widgets[i]->window() == event.xclient.window) {
                            widgets[i]->close();
                        }
                    }
                }
            }  
            break;
        case Expose:
            for (unsigned int i = 0; i < widgets.size(); i++) {
                if (widgets[i]->window() == event.xexpose.window) {
                    widgets[i]->draw();
                }
            }
            break;
        case ConfigureNotify:
            for (unsigned int i = 0; i < widgets.size(); i++) {
                if (widgets[i]->window() == event.xconfigure.window) {
                    widgets[i]->resize(event.xconfigure.width, 
                                       event.xconfigure.height);
                    widgets[i]->draw();
                }
            }
            break;
        case EnterNotify:
            for (unsigned int i = 0; i < widgets.size(); i++) {
                if (widgets[i]->window() == event.xbutton.window) {
                    widgets[i]->enterNotify();
                }
            }
            break;
        case LeaveNotify:
            for (unsigned int i = 0; i < widgets.size(); i++) {
                if (widgets[i]->window() == event.xbutton.window) {
                    widgets[i]->leaveNotify();
                }
            }
            break;
        case ButtonPress:
            for (unsigned int i = 0; i < widgets.size(); i++) {
                if (widgets[i]->window() == event.xbutton.window) {
                    widgets[i]->buttonPress();
                }
            }
            break;
        default:
            break;
        }
    }
}

};
