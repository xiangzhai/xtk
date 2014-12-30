// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "xtkmenu-x11.h"
#include "xtkutil.h"
#include "xtktext.h"

namespace Xtk 
{

XtkMenuX11::XtkMenuX11(XtkWindowX11* parent, 
                       int x, 
                       int y, 
                       int width, 
                       int height) 
  : XtkWindowX11(parent->display(),
                 parent->theme(), 
                 x, 
                 y, 
                 width, 
                 height, 
                 "xtkmenu-x11", 
                 None, 
                 0, 
                 _NET_WM_WINDOW_TYPE_POPUP_MENU), 
    m_parent(parent), 
    m_y(y),
    m_width(width), 
    m_height(height)
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " " << this->window() 
              << std::endl;
#endif
    context = cairo_create(this->surface());
    if (context == nullptr) 
        std::cerr << "ERROR: fail to create context" << std::endl;
    
    itemHeight = this->theme()->getInt("menu", "itemheight", 30);
}

XtkMenuX11::~XtkMenuX11() 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    for (unsigned int i = 0; i < items.size(); i++) {
        if (items[i]) {
            delete items[i];
            items[i] = nullptr;
        }
    }
    items.clear();
}

void XtkMenuX11::addItem(std::string text, 
                         MENUITEM_CALLBACK menuItemCallback, 
                         void* arg,
                         XtkMenuItem* parent,
                         std::string iconFileName) 
{
    if (parent == nullptr) {
        m_height += itemHeight;
        setSize(m_width, m_height);
    }
    items.push_back(new XtkMenuItem(
                text, menuItemCallback, arg, parent, iconFileName));
}

void XtkMenuX11::addItem(XtkMenuItem* item) 
{
    if (item->parent() == nullptr) {
        m_height += itemHeight;
        setSize(m_width, m_height);
    }
    items.push_back(item);
}

void XtkMenuX11::enterNotify() 
{
}

void XtkMenuX11::leaveNotify() 
{
}

void XtkMenuX11::buttonPress(XButtonEvent event) 
{ 
    curLevel = event.x / m_width;
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " " << curLevel << std::endl;
#endif
    for (unsigned int i = 0; i < curItems.size(); i++) {
        if (event.y < int(i + 1) * itemHeight && event.y > (int)i * itemHeight) {
            /* FIXME: sub-menu
            if (sub == nullptr) {
                sub = new XtkMenuX11(m_parent, 
                    event.x, m_y + event.y, m_width, m_height);
                sub->draw();
            } else {
                delete sub;
                sub = nullptr;
            }
            */
            if (curItems[i]->menuItemCallback()) 
                curItems[i]->menuItemCallback()(this, curItems[i]->arg());
            break;
        }
    }
}

void XtkMenuX11::motionNotify(XButtonEvent event) 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << event.y << std::endl;
#endif
    pointerY = event.y;
    draw();
}

void XtkMenuX11::draw() 
{
    double r, g, b;
    int y = 0;

    this->swap(this->theme()->string("menu", "backgroundcolor", "#ffffff"));

    colorHtmlToCairo(this->theme()->string("menu", "bordercolor", "#ffffff"), 
        r, g, b);
    cairo_set_source_rgb(context, r, g, b);
    cairo_set_line_width(context, this->theme()->getInt("menu", "borderwidth", 1));
    cairo_rectangle(context, 0, 0, m_width, m_height);
    cairo_stroke_preserve(context);

    curItems.clear();
    for (unsigned int i = 0; i < items.size(); i++) {
        if (curLevel == 0) {
            if (items[i]->parent() == nullptr) { 
                std::cout << items[i]->text() << std::endl;
                curItems.push_back(items[i]);
            }
        }
    }
        
    for (unsigned int i = 0; i < curItems.size(); i++) {
        // hover
        if (pointerY < (int)(i + 1) * itemHeight && 
            pointerY > (int)i * itemHeight) {
            colorHtmlToCairo(
                this->theme()->string("menu", "borderhovercolor", "#ffffff"), 
                r, g, b);
            cairo_set_source_rgb(context, r, g, b);
            cairo_set_line_width(context, 
                    this->theme()->getInt("menu", "borderwidth", 1));
            cairo_rectangle(context, 0, i * itemHeight, m_width, itemHeight);
            cairo_stroke(context);
        }
        // text
        XtkText textObj(this->surface(), 
                curItems[i]->text(), 30, y, m_width, itemHeight);
        textObj.draw();
        // increase
        y += itemHeight;
    }
}

}
