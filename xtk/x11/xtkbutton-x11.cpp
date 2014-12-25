// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "xtkbutton-x11.h"
#include "xtkutil.h"

#include <cairo.h>
#include <iostream>

namespace Xtk 
{

XtkButtonX11::XtkButtonX11(XtkWindowX11* parent, 
                           std::string textStr, 
                           int x, 
                           int y, 
                           int width, 
                           int height) 
  : XtkWindowX11(parent->display(), 
                 x, 
                 y, 
                 width, 
                 height, 
                 "xtkbutton-x11", 
                 "#FFFFFF", 
                 parent->window(), 
                 0), 
    m_parent(parent), 
    m_textStr(textStr),
    m_width(width), 
    m_height(height)
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    m_text = new XtkText(this->surface(), m_textStr, 
            0, 0, m_width, m_height, 12, "#000000", CENTER);
}

XtkButtonX11::~XtkButtonX11() 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    if (m_text) {
        delete m_text;
        m_text = nullptr;
    }
}

void XtkButtonX11::setButtonPressCallback(
        BUTTON_PRESS_CALLBACK buttonPressCallback, 
        void* arg) 
{
    m_buttonPressCallback = buttonPressCallback;
    m_arg = arg;
}

void XtkButtonX11::buttonPress() 
{ 
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    if (m_buttonPressCallback) 
        m_buttonPressCallback(this, m_arg); 
}

void XtkButtonX11::draw() 
{
    cairo_t* context = nullptr;
    double r, g, b;

    this->swap();

    context = cairo_create(this->surface());
    if (context == nullptr) 
        return;

    colorHtmlToCairo("#000000", r, g, b);
    cairo_set_source_rgb(context, r, g, b);
    cairo_set_line_width(context, 2);
    cairo_rectangle(context, 0, 0, m_width, m_height);
    cairo_stroke_preserve(context);
    
    m_text->draw();

    cairo_destroy(context);
    context = nullptr;
}

}
