// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "xtkutil.h"

#include <iostream>

namespace Xtk 
{

void colorHtmlToCairo(std::string color, double & r, double & g, double & b) 
{
    std::string hex = "0x";
    
    if (color[0] != '#' || color.size() != 7) {
        std::cerr << "ERROR: " << __PRETTY_FUNCTION__ 
                  << " color html format is wrong" << std::endl;
        return;
    }
    
    r = std::stoul(hex + color.substr(1, 2), nullptr, 16) / 255.0;
    g = std::stoul(hex + color.substr(3, 2), nullptr, 16) / 255.0;
    b = std::stoul(hex + color.substr(5, 2), nullptr, 16) / 255.0;
}

};
