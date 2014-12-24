// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef XTK_EVENT_X11_H
#define XTK_EVENT_X11_H

#include <X11/Xlib.h>

#include "xtkwindow-x11.h"

namespace Xtk 
{

class XtkEventX11 
{
public:
    XtkEventX11(Display* display, XtkWindowX11* window);
    ~XtkEventX11();

    Display* display() const { return m_display; }

private:
    Display* m_display = nullptr;
    XtkWindowX11* m_window = nullptr;
    bool quit = false;
};

};

#endif // XTK_EVENT_X11_H
