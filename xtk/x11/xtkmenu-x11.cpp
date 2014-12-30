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
                       XtkMenuItem* parentItem, 
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
                 _NET_WM_WINDOW_TYPE_MENU), 
    m_parent(parent), 
    m_x(x),
    m_y(y),
    m_width(width), 
    m_parentItem(parentItem),
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
    for (unsigned int i = 0; i < m_items.size(); i++) {
        if (m_items[i]) {
            delete m_items[i];
            m_items[i] = nullptr;
        }
    }
    m_items.clear();
}

void XtkMenuX11::setEvent(XtkEventX11* event) 
{
    if (m_event == event) 
        return;

    m_event = event;
}

void XtkMenuX11::addItem(std::string text, 
                         MENUITEM_CALLBACK menuItemCallback, 
                         void* arg,
                         XtkMenuItem* parent,
                         std::string iconFileName) 
{
    if (parent == m_parentItem) {
        m_height += itemHeight;
        setSize(m_width, m_height);
    }
    m_items.push_back(new XtkMenuItem(
                text, menuItemCallback, arg, parent, iconFileName));
}

void XtkMenuX11::addItem(XtkMenuItem* item) 
{
    if (item->parent() == m_parentItem) {
        m_height += itemHeight;
        setSize(m_width, m_height);
    }
    m_items.push_back(item);
}

void XtkMenuX11::addItems(std::vector<XtkMenuItem*> items) 
{
    for (unsigned int i = 0; i < items.size(); i++) {
        if (items[i]->parent() == m_parentItem) {
            m_height += itemHeight;
            setSize(m_width, m_height);
        }
        m_items.push_back(items[i]);
    }
}

void XtkMenuX11::enterNotify() 
{
}

void XtkMenuX11::leaveNotify() 
{
}

void XtkMenuX11::setChild(XtkMenuX11* child) 
{
    m_child = child;
}

void XtkMenuX11::buttonPress(XButtonEvent event) 
{ 
    for (unsigned int i = 0; i < curItems.size(); i++) {
        if (event.y < int(i + 1) * itemHeight && 
            event.y > (int)i * itemHeight) {
            if (curItem == curItems[i]) 
                return;

            curItem = curItems[i];
            bool hasChildren = false;

            for (unsigned int i = 0; i < m_items.size(); i++) {
                if (m_items[i]->parent() == curItem) {
                    hasChildren = true;
                    break;
                }
            }
            if (hasChildren) {
                if (sub != nullptr) {
                    if (m_event) 
                        m_event->disconnect(sub);

                    if (sub->child()) 
                        sub->child()->close();

                    sub->close();
                    sub = nullptr;
                }
                
                sub = new XtkMenuX11(this, 
                        m_x + m_width, m_y + event.y, m_width, curItem);
                this->setChild(sub);
                sub->setEvent(m_event);
                if (m_event) 
                    m_event->connect(sub);

                sub->addItems(m_items);
                sub->draw();
            } else {
                // TODO: click the leaf node, close ALL menus
            }

#if 0
            if (parentItem->menuItemCallback()) 
                parentItem->menuItemCallback()(this, parentItem->arg());
#endif
            break;
        }
    }
}

void XtkMenuX11::motionNotify(XButtonEvent event) 
{
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
    for (unsigned int i = 0; i < m_items.size(); i++) {
        if (m_items[i]->parent() == m_parentItem)  
            curItems.push_back(m_items[i]);
    }
        
    for (unsigned int i = 0; i < curItems.size(); i++) {
        // FIXME: hover is LOW efficiency ;(
#if 0
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
#endif
        // text
        XtkText textObj(this->surface(), 
                curItems[i]->text(), 30, y, m_width, itemHeight);
        textObj.draw();
        // increase
        y += itemHeight;
    }
}

}
