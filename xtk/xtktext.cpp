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
    m_align(align)
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    cairo_text_extents_t extents;
    double r, g, b;
    
    surface = cairo_surface_create_for_rectangle(parent, x, y, width, height);
    if (surface == nullptr) 
        std::cout << "ERROR: fail to create text surface" << std::endl;
    else {
        context = cairo_create(surface);
        if (context == nullptr) 
            std::cerr << "ERROR: fail to create text context" << std::endl;
        else {
            cairo_select_font_face(context, family.c_str(), slant, weight);
            cairo_set_font_size(context, size);
            cairo_text_extents(context, m_text.c_str(), &extents);
            text_width = extents.width;
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

void XtkText::draw() 
{
    int y = m_size + (m_height - m_size) / 2;

    if (m_align == LEFT) {
        cairo_move_to(context, 0, y);
    } else if (m_align == CENTER) {
        cairo_move_to(context, (m_width - text_width) / 2, y);
    }
    cairo_show_text(context, m_text.c_str());
}

}
