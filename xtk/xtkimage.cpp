// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "xtkimage.h"

#include <iostream>

namespace Xtk 
{

XtkImage::XtkImage(cairo_surface_t* parent, 
                   std::string filename, 
                   int x, 
                   int y, 
                   int width, 
                   int height) 
{
    surface = cairo_surface_create_for_rectangle(parent, x, y, width, height);
    if (surface == nullptr) 
        std::cerr << "ERROR: fail to create image surface" << std::endl;
    else {
        context = cairo_create(surface);
        image_surface = cairo_image_surface_create_from_png(filename.c_str());
        if (image_surface == nullptr) {
            std::cerr << "ERROR: fail to create image surface from png" 
                      << std::endl;
        } else {
            if (cairo_surface_status(image_surface) == 
                    CAIRO_STATUS_FILE_NOT_FOUND) {
                std::cerr << "ERROR: file " << filename << " not found" 
                          << std::endl;
            } else {
                int w = cairo_image_surface_get_width(image_surface);
                int h = cairo_image_surface_get_height(image_surface);
                if (w && h) {
                    cairo_scale(context, 
                            (double)width / (double)w, 
                            (double)height / (double)h);
                }
            }
        }
    }
}

XtkImage::~XtkImage() 
{
    if (image_surface) {
        cairo_surface_destroy(image_surface);
        image_surface = nullptr;
    }

    if (context) {
        cairo_destroy(context);
        context = nullptr;
    }

    if (surface) {
        cairo_surface_destroy(surface);
        surface = nullptr;
    }
}

void XtkImage::draw() 
{
    cairo_set_source_surface(context, image_surface, 0, 0);
    cairo_paint(context);
}

}
