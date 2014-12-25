// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef XTK_THEME_H
#define XTK_THEME_H

#include <string>
extern "C" {
#include <iniparser.h>
}

namespace Xtk 
{

class XtkTheme 
{
public:
    XtkTheme(std::string name = "oxygen");
    ~XtkTheme();

    std::string name() const { return m_name; }
    std::string string(std::string section, 
                       std::string key, 
                       std::string error) const;

private:
    std::string m_name;
    dictionary* ini = nullptr;
};

};

#endif // XTK_THEME_H
