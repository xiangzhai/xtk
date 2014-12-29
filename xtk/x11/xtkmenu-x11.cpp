// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "xtkmenu-x11.h"
#include "xtkutil.h"
#include "xtktext.h"

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
    
    itemHeight = this->theme()->getInt("menu", "itemheight", 30);
}

XtkMenuX11::~XtkMenuX11() 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    for (unsigned int i = 0; i < items.size(); i++) {
        if (items[i]) {
            delete items[i];
            items[i] = nullptr;
        }
    }
    items.clear();
}

void XtkMenuX11::addItem(std::string text, 
                         MENUITEM_CALLBACK menuItemCallback, 
                         void* arg, 
                         std::string iconFileName) 
{
    m_height += itemHeight;
    setSize(m_width, m_height);
    items.push_back(new XtkMenuItem(text, menuItemCallback, arg, iconFileName));
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
    sleep(1);
    hide();
}

void XtkMenuX11::buttonPress(XButtonEvent event) 
{ 
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    for (unsigned int i = 0; i < items.size(); i++) {
        if (event.y < int(i + 1) * itemHeight && event.y > (int)i * itemHeight) {
            if (items[i]->menuItemCallback) 
                items[i]->menuItemCallback(this, items[i]->arg);
        }
    }
}

void XtkMenuX11::motionNotify(XButtonEvent event) 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << event.y << std::endl;
#endif
    pointerY = event.y;
    draw();
}

void XtkMenuX11::draw() 
{
    double r, g, b;
    int y = 0;

    this->swap(this->theme()->string("menu", "backgroundcolor", "#ffffff"));

    colorHtmlToCairo(this->theme()->string("menu", "bordercolor", "#ffffff"), 
        r, g, b);
    cairo_set_source_rgb(context, r, g, b);
    cairo_set_line_width(context, this->theme()->getInt("menu", "borderwidth", 1));
    cairo_rectangle(context, 0, 0, m_width, m_height);
    cairo_stroke_preserve(context);

    for (unsigned int i = 0; i < items.size(); i++) {
        // hover
        if (pointerY < (int)(i + 1) * itemHeight && 
            pointerY > (int)i * itemHeight) {
            colorHtmlToCairo(
                this->theme()->string("menu", "borderhovercolor", "#ffffff"), 
                r, g, b);
            cairo_set_source_rgb(context, r, g, b);
            cairo_set_line_width(context, 
                    this->theme()->getInt("menu", "borderwidth", 1));
            cairo_rectangle(context, 0, i * itemHeight, m_width, itemHeight);
            cairo_stroke(context);
        }
        // text
        XtkText textObj(this->surface(), items[i]->text, 30, y, m_width, itemHeight);
        textObj.draw();
        // increase
        y += itemHeight;
    }
}

}
