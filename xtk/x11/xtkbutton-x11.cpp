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
                 parent->theme(), 
                 x, 
                 y, 
                 width, 
                 height, 
                 "xtkbutton-x11", 
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
    m_text->setFamily(this->theme()->string("font", "family", "Serif"));
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

void XtkButtonX11::enterNotify() 
{
    cairo_t* context = nullptr;
    double r, g, b;

#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    
    this->swap(this->theme()->string("button", "backgroundcolor", "#ffffff"));

    context = cairo_create(this->surface());
    if (context == nullptr) 
        return;

    colorHtmlToCairo(this->theme()->string("button", "borderhovercolor", "#ffffff"), 
        r, g, b);                                                                  
    cairo_set_source_rgb(context, r, g, b);                                        
    cairo_set_line_width(context, 2);                                              
    cairo_rectangle(context, 0, 0, m_width, m_height);                             
    cairo_stroke_preserve(context);                                                
                                                                                   
    m_text->draw();                                                                
                                                                                   
    cairo_destroy(context);                                                        
    context = nullptr;
}

void XtkButtonX11::leaveNotify() 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif

    draw();
}

void XtkButtonX11::buttonPress() 
{ 
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    if (m_buttonPressCallback) 
        m_buttonPressCallback(this, m_arg); 
}

// FIXME: need send Expose event?
void XtkButtonX11::setSize(int width, int height) 
{
    if (m_width == width && m_height == height) 
        return;

    m_width = width;
    m_height = height;
    XResizeWindow(this->display(), this->window(), width, height);
    XSync(this->display(), False);
    resize(width, height);
    m_text->setSize(width, height);
    draw();
}

void XtkButtonX11::draw() 
{
    cairo_t* context = nullptr;
    double r, g, b;

    this->swap(this->theme()->string("button", "backgroundcolor", "#ffffff"));

    context = cairo_create(this->surface());
    if (context == nullptr) 
        return;

    colorHtmlToCairo(this->theme()->string("button", "bordercolor", "#ffffff"), 
        r, g, b);
    cairo_set_source_rgb(context, r, g, b);
    cairo_set_line_width(context, this->theme()->getInt("button", "borderwidth", 1));
    cairo_rectangle(context, 0, 0, m_width, m_height);
    cairo_stroke_preserve(context);
    
    m_text->draw();

    cairo_destroy(context);
    context = nullptr;
}

}
