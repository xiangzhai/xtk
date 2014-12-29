// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef XTK_MENU_X11_H
#define XTK_MENU_X11_H

#include <x11/xtkwindow-x11.h>
#include <cairo.h>
#include <xtktext.h>

#include <string>
#include <vector>

namespace Xtk 
{

class XtkMenuX11;

typedef void* (*MENUITEM_CALLBACK)(XtkMenuX11*, void*);

class XtkMenuItem 
{
public:
    XtkMenuItem(std::string text, 
                MENUITEM_CALLBACK menuItemCallback = nullptr, 
                void* arg = nullptr,
                std::string iconFileName = "") 
    {
        this->text = text;
        this->menuItemCallback = menuItemCallback;
        this->arg = arg;
        this->iconFileName = iconFileName;
    }

public:
    std::string text = "";
    MENUITEM_CALLBACK menuItemCallback = nullptr;
    void* arg = nullptr;
    std::string iconFileName = "";
};

class XtkMenuX11 : public XtkWindowX11 
{
public:
    XtkMenuX11(XtkWindowX11* parent, 
               int x, 
               int y, 
               int width, 
               int height = 1);
    ~XtkMenuX11();

    void addItem(std::string text, 
                 MENUITEM_CALLBACK menuItemCallback = nullptr, 
                 void* arg = nullptr, 
                 std::string iconFileName = "");

    void enterNotify();
    void leaveNotify();
    void buttonPress(XButtonEvent event);
    void motionNotify(XButtonEvent event);
    void draw();

private:
    XtkWindowX11* m_parent = nullptr;
    int m_width = 0;
    int m_height = 0;
    cairo_t* context = nullptr;
    int itemHeight = 30;
    std::vector<XtkMenuItem*> items;
    int pointerY = -1;
};

};

#endif // XTK_MENU_X11_H
