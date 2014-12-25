// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "xtkdisplay-x11.h"

#include <iostream>

namespace Xtk 
{

XtkDisplayX11::XtkDisplayX11(char* name) 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    m_display = XOpenDisplay(name);
    if (m_display == nullptr) 
        std::cerr << "ERROR: fail to open display" << std::endl;
}

XtkDisplayX11::~XtkDisplayX11() 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    if (m_display) {
        XCloseDisplay(m_display);
        m_display = nullptr;
    }
}

}
