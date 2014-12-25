// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "xtkwidget-x11.h"

#include <iostream>

namespace Xtk 
{

XtkWidgetX11::XtkWidgetX11(Display* display)
  : m_display(display) 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
}

XtkWidgetX11::~XtkWidgetX11() 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
}

}
