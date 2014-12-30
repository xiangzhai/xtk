// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "xtkbutton-x11.h"
#include "xtkutil.h"

#include <iostream>

namespace Xtk 
{

XtkButtonX11::XtkButtonX11(XtkWindowX11* parent, 
                           std::string text, 
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
    m_text(text),
    m_width(width), 
    m_height(height)
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " " << this->window() 
              << std::endl;
#endif
    m_textObj = new XtkText(this->surface(), m_text, 
            0, 0, m_width, m_height, 12, "#000000", MIDDLE_CENTER);
    m_textObj->setFamily(this->theme()->string("font", "family", "Serif"));
    context = cairo_create(this->surface());
    if (context == nullptr) 
        std::cerr << "ERROR: fail to create context" << std::endl;
}

XtkButtonX11::~XtkButtonX11() 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    if (m_textObj) {
        delete m_textObj;
        m_textObj = nullptr;
    }
}

void XtkButtonX11::setButtonPressCallback(
        BUTTON_PRESS_CALLBACK buttonPressCallback, 
        void* arg) 
{
    m_buttonPressCallback = buttonPressCallback;
    m_buttonPressArg = arg;
}

void XtkButtonX11::setRightButtonPressCallback(
        RIGHT_BUTTON_PRESS_CALLBACK rightButtonPressCallback, 
        void* arg) 
{
    m_rightButtonPressCallback = rightButtonPressCallback;
    m_rightButtonPressArg = arg;
}

void XtkButtonX11::enterNotify() 
{
    double r, g, b;

#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    
    this->swap(this->theme()->string("button", "backgroundcolor", "#ffffff"));

    colorHtmlToCairo(this->theme()->string("button", "borderhovercolor", "#ffffff"), 
        r, g, b);                                                                  
    cairo_set_source_rgb(context, r, g, b);                                        
    cairo_set_line_width(context, 2);                                              
    cairo_rectangle(context, 0, 0, m_width, m_height);                             
    cairo_stroke_preserve(context);                                                
                                                                                   
    m_textObj->draw();                                                                
}

void XtkButtonX11::leaveNotify() 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    draw();
}

void XtkButtonX11::buttonPress(XButtonEvent event) 
{ 
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " " << event.button << std::endl;
#endif
    if (event.button == Button1 && m_buttonPressCallback) {
        m_buttonPressCallback(this, m_buttonPressArg); 
        return;
    }

    if (event.button == Button3 && m_rightButtonPressCallback) {
        m_rightButtonPressCallback(this, m_rightButtonPressArg);
        return;
    }
}

void XtkButtonX11::setText(const std::string & text) 
{
    if (m_text == text) 
        return;

    m_text = text;
    m_textObj->setText(m_text);
    draw();
}

// FIXME: need send Expose event?
void XtkButtonX11::setSize(int width, int height) 
{
    if (m_width == width && m_height == height) 
        return;

    m_width = width;
    m_height = height;
    XResizeWindow(this->display(), this->window(), width, height);
    resize(width, height);
    m_textObj->setSize(width, height);
    draw();
}

void XtkButtonX11::draw() 
{
    double r, g, b;

    this->swap(this->theme()->string("button", "backgroundcolor", "#ffffff"));

    colorHtmlToCairo(this->theme()->string("button", "bordercolor", "#ffffff"), 
        r, g, b);
    cairo_set_source_rgb(context, r, g, b);
    cairo_set_line_width(context, this->theme()->getInt("button", "borderwidth", 1));
    cairo_rectangle(context, 0, 0, m_width, m_height);
    cairo_stroke_preserve(context);
    
    m_textObj->draw();
}

}
