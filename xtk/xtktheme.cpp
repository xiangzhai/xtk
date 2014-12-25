// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "xtktheme.h"

#include <iostream>

namespace Xtk 
{

XtkTheme::XtkTheme(std::string name) 
  : m_name(name)
{
    std::string iniFilePath = std::string(DATADIR) + "/" + m_name + ".ini";
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " " << iniFilePath << std::endl;
#endif
    ini = iniparser_load(iniFilePath.c_str());
    if (ini == nullptr) {
        std::cerr << "ERROR: fail to load ini " << iniFilePath << " file" 
                  << std::endl;
    }
}

XtkTheme::~XtkTheme() 
{
    if (ini) {
        iniparser_freedict(ini);
        ini = nullptr;
    }
}

std::string XtkTheme::string(std::string section, 
                             std::string key, 
                             std::string error) const 
{
    if (ini == nullptr) 
        return "";

    std::string keyStr = section + ":" + key;
    return std::string(iniparser_getstring(ini, keyStr.c_str(), (char*)error.c_str()));
}

}
