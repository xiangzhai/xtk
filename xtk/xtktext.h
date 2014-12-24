// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef XTK_TEXT_H
#define XTK_TEXT_H

#include <cairo.h>
#include <string>

namespace Xtk 
{

typedef enum {
    LEFT, 
    CENTER, 
    RIGHT
} TextAlign;

class XtkText 
{
public:
    XtkText(cairo_surface_t* parent, 
            std::string text, 
            int x, 
            int y, 
            int width, 
            int height, 
            int size = 12, 
            std::string color = "#000000", 
            TextAlign align = LEFT, 
            cairo_font_slant_t slant = CAIRO_FONT_SLANT_NORMAL, 
            cairo_font_weight_t weight = CAIRO_FONT_WEIGHT_NORMAL, 
            std::string family = "Serif");
    ~XtkText();

    void draw();

private:
    cairo_surface_t* surface = nullptr;
    cairo_t* context = nullptr;
    std::string m_text;
    int m_width = 0;
    int m_height = 0;
    int m_size = 0;
    TextAlign m_align;
    int text_width = 0;
};

};

#endif // XTK_TEXT_H
