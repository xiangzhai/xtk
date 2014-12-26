// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef XTK_EVENT_X11_H
#define XTK_EVENT_X11_H

#include <X11/Xlib.h>
#include <x11/xtkwidget-x11.h>
#include <vector>

namespace Xtk 
{

class XtkEventX11 
{
public:
    XtkEventX11(Display* display, XtkWidgetX11* parent);
    ~XtkEventX11();

    Display* display() const { return m_display; }

    void connect(XtkWidgetX11* widget);
    void disconnect(XtkWidgetX11* widget);
    void run();

private:
    Display* m_display = nullptr;
    XtkWidgetX11* m_parent = nullptr;
    bool quit = false;
    std::vector<XtkWidgetX11*> widgets;
};

};

#endif // XTK_EVENT_X11_H
