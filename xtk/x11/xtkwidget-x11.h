// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef XTK_WIDGET_X11_H
#define XTK_WIDGET_X11_H

#include <X11/Xlib.h>
#include <string>

namespace Xtk 
{

class XtkWidgetX11 
{
public:
    XtkWidgetX11(Display* display); 
    virtual ~XtkWidgetX11();

    virtual Display* display() const { return m_display; }
    virtual Window window() const { return m_window; }
    virtual Atom wmDeleteMessage() const { return m_wmDeleteMessage; }

    void sendNETWMMessage(Window window, 
                          std::string atomName, 
                          unsigned long l0 = 0, 
                          unsigned long l1 = 0, 
                          unsigned long l2 = 0, 
                          unsigned long l3 = 0, 
                          unsigned long l4 = 0);
    void activateWindow(Window window);
    void minimizeWindow(Window window);
    void quit(Window parent);
    
    virtual void close() {}
    virtual void show() {}
    virtual void hide() {}
    virtual void enterNotify() {}
    virtual void leaveNotify() {}
    virtual void buttonPress(XButtonEvent event) {}
    virtual void setSize(int width, int height) {}
    virtual void resize(int width, int height) {}
    virtual void draw() {}
    virtual void swap(double alpha = 1.0) {}

private:
    Display* m_display = nullptr;
    Window m_window = None;
    Atom m_wmDeleteMessage;
};

};

#endif // XTK_WIDGET_X11_H
