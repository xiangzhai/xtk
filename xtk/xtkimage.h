// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef XTK_IMAGE_H
#define XTK_IMAGE_H

#include <cairo.h>
#include <string>

namespace Xtk 
{

class XtkImage 
{
public:
    XtkImage(cairo_surface_t* parent, 
             std::string filename, 
             int x, 
             int y, 
             int width, 
             int height);
    ~XtkImage();

    void draw();

private:
    cairo_surface_t* surface = nullptr;
    cairo_t* context = nullptr;
    cairo_surface_t* image_surface = nullptr;
};

};

#endif // XTK_IMAGE_H
