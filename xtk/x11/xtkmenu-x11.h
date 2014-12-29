// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef XTK_MENU_X11_H
#define XTK_MENU_X11_H

#include <x11/xtkwindow-x11.h>
#include <cairo.h>
#include <xtktext.h>

#include <string>

namespace Xtk 
{

class XtkMenuX11;

typedef void* (*MENU_PRESS_CALLBACK)(XtkMenuX11*, void*);

class XtkMenuX11 : public XtkWindowX11 
{
public:
    XtkMenuX11(XtkWindowX11* parent, 
               int x, 
               int y, 
               int width, 
               int height);
    ~XtkMenuX11();

    void setMenuPressCallback(MENU_PRESS_CALLBACK menuPressCallback, 
                              void* arg = nullptr);

    void enterNotify();
    void leaveNotify();
    void buttonPress();
    void draw();

private:
    XtkWindowX11* m_parent = nullptr;
    int m_width = 0;
    int m_height = 0;
    cairo_t* context = nullptr;
    MENU_PRESS_CALLBACK m_menuPressCallback = nullptr;
    void* m_arg = nullptr;
};

};

#endif // XTK_MENU_X11_H
