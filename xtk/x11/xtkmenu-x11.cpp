// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "xtkmenu-x11.h"
#include "xtkutil.h"

#include <iostream>

namespace Xtk 
{

XtkMenuX11::XtkMenuX11(XtkWindowX11* parent, 
                       int x, 
                       int y, 
                       int width, 
                       int height) 
  : XtkWindowX11(parent->display(),
                 parent->theme(), 
                 x, 
                 y, 
                 width, 
                 height, 
                 "xtkmenu-x11", 
                 None, 
                 0, 
                 _NET_WM_WINDOW_TYPE_MENU), 
    m_parent(parent), 
    m_width(width), 
    m_height(height)
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    context = cairo_create(this->surface());
    if (context == nullptr) 
        std::cerr << "ERROR: fail to create context" << std::endl;
}

XtkMenuX11::~XtkMenuX11() 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
}

void XtkMenuX11::setMenuPressCallback(MENU_PRESS_CALLBACK menuPressCallback, 
                                      void* arg) 
{
    m_menuPressCallback = menuPressCallback;
    m_arg = arg;
}

void XtkMenuX11::enterNotify() 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
}

void XtkMenuX11::leaveNotify() 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    sleep(2);
    hide();
}

void XtkMenuX11::buttonPress() 
{ 
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    if (m_menuPressCallback) 
        m_menuPressCallback(this, m_arg); 
}

void XtkMenuX11::draw() 
{
    double r, g, b;

    this->swap(this->theme()->string("menu", "backgroundcolor", "#ffffff"));

    colorHtmlToCairo(this->theme()->string("menu", "bordercolor", "#ffffff"), 
        r, g, b);
    cairo_set_source_rgb(context, r, g, b);
    cairo_set_line_width(context, this->theme()->getInt("menu", "borderwidth", 1));
    cairo_rectangle(context, 0, 0, m_width, m_height);
    cairo_stroke_preserve(context);
}

}
