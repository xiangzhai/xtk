// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef XTK_MENU_X11_H
#define XTK_MENU_X11_H

#include <x11/xtkwindow-x11.h>
#include <x11/xtkevent-x11.h>
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
               XtkMenuX11* parentMenu = nullptr,
               XtkMenuItem* parentItem = nullptr,
               int height = 1);
    ~XtkMenuX11();

    XtkWindowX11* parent() const { return m_parent; }
    XtkMenuX11* parentMenu() const { return m_parentMenu; }
    XtkMenuX11* childMenu() const { return m_childMenu; }
    void setChildMenu(XtkMenuX11* childMenu);
    std::vector<XtkMenuItem*> items() const { return m_items; }

    void setEvent(XtkEventX11* event);
    void addItem(std::string text, 
                 MENUITEM_CALLBACK menuItemCallback = nullptr, 
                 void* arg = nullptr,
                 XtkMenuItem* parent = nullptr,
                 std::string iconFileName = "");
    void addItem(XtkMenuItem* item);
    void addItems(std::vector<XtkMenuItem*> items);

    void enterNotify();
    void leaveNotify();
    void buttonPress(XButtonEvent event);
    void motionNotify(XButtonEvent event);
    void focusOut();
    void draw();

private:
    void m_closeParentMenu(XtkMenuX11* menu);
    void m_walkChildMenu(int y = 0);

private:
    XtkWindowX11* m_parent = nullptr;
    int m_x = 0;
    int m_y = 0;
    int m_width = 0;
    XtkMenuX11* m_parentMenu = nullptr;
    XtkMenuItem* m_parentItem = nullptr;
    int m_height = 0;
    cairo_t* m_context = nullptr;
    XtkEventX11* m_event = nullptr;
    int m_itemHeight = 30;
    std::vector<XtkMenuItem*> m_items;
    int m_pointerY = -1;
    std::vector<XtkMenuItem*> m_curItems;
    XtkMenuX11* m_subMenu = nullptr;
    XtkMenuX11* m_childMenu = nullptr;
    XtkMenuItem* m_curItem = nullptr;
    bool m_isMotionNotify = false;
};

};

#endif // XTK_MENU_X11_H
