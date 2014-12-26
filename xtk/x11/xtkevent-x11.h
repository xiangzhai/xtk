// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef XTK_EVENT_X11_H
#define XTK_EVENT_X11_H

#include <X11/Xlib.h>
#include <x11/xtkwindow-x11.h>
#include <x11/xtkwidget-x11.h>
#include <vector>

namespace Xtk 
{

class XtkEventX11 
{
public:
    XtkEventX11(Display* display, XtkWindowX11* window);
    ~XtkEventX11();

    Display* display() const { return m_display; }

    void connect(XtkWidgetX11* widget);
    void run();

private:
    Display* m_display = nullptr;
    XtkWindowX11* m_window = nullptr;
    bool quit = false;
    std::vector<XtkWidgetX11*> widgets;
};

};

#endif // XTK_EVENT_X11_H
