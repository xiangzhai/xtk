// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef XTK_MENU_X11_H
#define XTK_MENU_X11_H

#include <x11/xtkwindow-x11.h>
#include <cairo.h>
#include <xtktext.h>

#include <iostream>
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
                XtkMenuItem* parent = nullptr,
                std::string iconFileName = "") 
      : m_text(text), 
        m_menuItemCallback(menuItemCallback), 
        m_arg(arg), 
        m_parent(parent), 
        m_iconFileName(iconFileName)
    {
#if XTK_DEBUG
        std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    }

    std::string text() const { return m_text; }
    MENUITEM_CALLBACK menuItemCallback() { return m_menuItemCallback; }
    void* arg() { return m_arg; }
    XtkMenuItem* parent() { return m_parent; }
    std::string iconFileName() const { return m_iconFileName; }

private:
    std::string m_text = "";
    MENUITEM_CALLBACK m_menuItemCallback = nullptr;
    void* m_arg = nullptr;
    XtkMenuItem* m_parent = nullptr;
    std::string m_iconFileName = "";
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
                 XtkMenuItem* parent = nullptr,
                 std::string iconFileName = "");
    void addItem(XtkMenuItem* item);

    void enterNotify();
    void leaveNotify();
    void buttonPress(XButtonEvent event);
    void motionNotify(XButtonEvent event);
    void focusOut();
    void draw();

private:
    XtkWindowX11* m_parent = nullptr;
    int m_y = 0;
    int m_width = 0;
    int m_height = 0;
    cairo_t* context = nullptr;
    int itemHeight = 30;
    std::vector<XtkMenuItem*> items;
    int pointerY = -1;
    int curLevel = 0;
    std::vector<XtkMenuItem*> curItems;
    XtkMenuX11* sub = nullptr;
};

};

#endif // XTK_MENU_X11_H
