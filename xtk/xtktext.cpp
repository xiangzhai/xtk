// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "xtktext.h"
#include "xtkutil.h"

#include <iostream>

namespace Xtk 
{

XtkText::XtkText(cairo_surface_t* parent, 
                 std::string text, 
                 int x, 
                 int y, 
                 int width, 
                 int height, 
                 int size, 
                 std::string color, 
                 TextAlign align, 
                 cairo_font_slant_t slant, 
                 cairo_font_weight_t weight, 
                 std::string family) 
  : m_text(text),
    m_width(width),
    m_height(height), 
    m_size(size), 
    m_align(align),
    m_slant(slant),
    m_weight(weight),
    m_family(family)
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    double r, g, b;
    
    surface = cairo_surface_create_for_rectangle(parent, x, y, width, height);
    if (surface == nullptr) 
        std::cout << "ERROR: fail to create text surface" << std::endl;
    else {
        context = cairo_create(surface);
        if (context == nullptr) 
            std::cerr << "ERROR: fail to create text context" << std::endl;
        else {
            calcTextSize();
            colorHtmlToCairo(color, r, g, b);
            cairo_set_source_rgb(context, r, g, b);
        }
    }
}

XtkText::~XtkText() 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    if (context) {
        cairo_destroy(context);
        context = nullptr;
    }
    
    if (surface) {
        cairo_surface_destroy(surface);
        surface = nullptr;
    }
}

void XtkText::calcTextSize() 
{
    cairo_text_extents_t extents;                                                  
    cairo_select_font_face(context, m_family.c_str(), m_slant, m_weight);          
    cairo_set_font_size(context, m_size);                                          
    cairo_text_extents(context, m_text.c_str(), &extents);                         
    textWidth = extents.width;
}

void XtkText::setFamily(const std::string & family) 
{
    if (m_family == family) 
        return;

    m_family = family;
    calcTextSize();
    draw();
}

void XtkText::setText(const std::string & text) 
{
    if (m_text == text) 
        return;

    m_text = text;
    calcTextSize();
    draw();
}

void XtkText::setSize(int width, int height) 
{
    if (m_width == width && m_height == height) 
        return;

    m_width = width;
    m_height = height;
    draw();
}

void XtkText::draw() 
{
    int y = m_size + (m_height - m_size) / 2;

    if (m_align == MIDDLE_LEFT) {
        cairo_move_to(context, 0, y);
    } else if (m_align == MIDDLE_CENTER) {
        cairo_move_to(context, (m_width - textWidth) / 2, y);
    }
    cairo_show_text(context, m_text.c_str());
}

}
