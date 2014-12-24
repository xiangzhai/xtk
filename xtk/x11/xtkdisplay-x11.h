// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef XTK_WINDOW_DISPLAY_H
#define XTK_WINDOW_DISPLAY_H

#include <X11/Xlib.h>

namespace Xtk 
{

class XtkDisplayX11 
{
public:
    XtkDisplayX11(char* name = NULL); 
    ~XtkDisplayX11();

    Display* display() const { return m_display; }

private:
    Display* m_display = nullptr;
};

};

#endif // XTK_WINDOW_DISPLAY_H
