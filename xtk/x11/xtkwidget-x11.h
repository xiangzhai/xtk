// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef XTK_WIDGET_X11_H
#define XTK_WIDGET_X11_H

#include <X11/Xlib.h>

namespace Xtk 
{

class XtkWidgetX11 
{
public:
    XtkWidgetX11(Display* display); 
    virtual ~XtkWidgetX11();

    Display* display() const { return m_display; }
    virtual Window window() const { return m_window; }

    virtual void enterNotify() {}
    virtual void leaveNotify() {}
    virtual void buttonPress() {}
    virtual void resize(int width, int height) {}
    virtual void draw() {}
    virtual void swap(double alpha = 1.0) {}

private:
    Display* m_display = nullptr;
    Window m_window = None;
};

};

#endif // XTK_WIDGET_X11_H
