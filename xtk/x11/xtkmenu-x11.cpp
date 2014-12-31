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
                       XtkMenuX11* parentMenu,
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
    m_parent((XtkMenuX11*)parent), 
    m_x(x),
    m_y(y),
    m_width(width), 
    m_parentMenu(parentMenu),
    m_parentItem(parentItem),
    m_height(height)
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " " << this->window() 
              << std::endl;
#endif
    m_context = cairo_create(this->surface());
    m_itemHeight = this->theme()->getInt("menu", "itemheight", 30);
}

XtkMenuX11::~XtkMenuX11() 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    if (m_event)
        m_event->disconnect(this);
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
        m_height += m_itemHeight;
        setSize(m_width, m_height);
    }
    m_items.push_back(new XtkMenuItem(
                text, menuItemCallback, arg, parent, iconFileName));
}

void XtkMenuX11::addItem(XtkMenuItem* item) 
{
    if (item->parent() == m_parentItem) {
        m_height += m_itemHeight;
        setSize(m_width, m_height);
    }
    m_items.push_back(item);
}

void XtkMenuX11::addItems(std::vector<XtkMenuItem*> items) 
{
    for (unsigned int i = 0; i < items.size(); i++) {
        if (items[i]->parent() == m_parentItem) {
            m_height += m_itemHeight;
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

void XtkMenuX11::setChildMenu(XtkMenuX11* childMenu) 
{
    m_childMenu = childMenu;
}

void XtkMenuX11::m_closeParentMenu(XtkMenuX11* menu) 
{
    XtkMenuX11* child = nullptr;

    if (menu == nullptr)
        return;

    child = menu->childMenu();
    if (child) {
        delete child;
        child = nullptr;
    }
    // root menu item
    if (menu->parentMenu() == nullptr) {
        for (unsigned int i = 0; i < m_items.size(); i++) {
            if (m_items[i]) {
                delete m_items[i]; 
                m_items[i] = nullptr;
            }
        }
        delete menu;
        menu = nullptr;
        return;
    }
    m_closeParentMenu(menu->parentMenu());
}

void XtkMenuX11::m_walkChildMenu(int y) 
{
    bool hasChildren = false;
    XtkMenuX11* child = nullptr;
    
    for (unsigned int i = 0; i < m_items.size(); i++) {
        if (m_items[i]->parent() == m_curItem) {
            hasChildren = true;
            break;
        }
    }

    if (hasChildren) {
        if (m_subMenu) {
            if (m_event) 
                m_event->disconnect(m_subMenu);
            // close child menu
            child = m_subMenu->childMenu();
            if (child) {
                delete child;
                child = nullptr;
            }
            delete m_subMenu;
            m_subMenu = nullptr;
        }
        
        // TODO: menu`s position (y) needs to adjust base on its height
        // and what if width is changable?
        m_subMenu = new XtkMenuX11(this, // parent widget
                                   m_x + m_width, 
                                   y, 
                                   m_width, 
                                   this, // parent menu 
                                   m_curItem); // parent (menu) item
        this->setChildMenu(m_subMenu);
        m_subMenu->setEvent(m_event);
        if (m_event) 
            m_event->connect(m_subMenu);
        m_subMenu->addItems(m_items);
        m_subMenu->draw();
    } else { 
        if (!m_isMotionNotify)
            m_closeParentMenu(this);
    }
}

void XtkMenuX11::buttonPress(XButtonEvent event) 
{
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__  << std::endl;
    for (unsigned int i = 0; i < m_curItems.size(); i++) {
        if (event.y < int(i + 1) * m_itemHeight && 
            event.y > (int)i * m_itemHeight) {
            m_isMotionNotify = false;
            m_curItem = m_curItems[i];
            
            if (m_curItem->menuItemCallback())
                m_curItem->menuItemCallback()(this, m_curItem->arg());
            
            m_walkChildMenu(m_y + event.y);
            
            break;
        }
    }
}

void XtkMenuX11::motionNotify(XButtonEvent event) 
{
    for (unsigned int i = 0; i < m_curItems.size(); i++) {
        if (event.y < int(i + 1) * m_itemHeight && 
            event.y > (int)i * m_itemHeight) {
            m_isMotionNotify = true;
            if (m_curItem == m_curItems[i])
                return;

            m_curItem = m_curItems[i];
            m_walkChildMenu(m_y + event.y);
            
            break;
        }
    }
}

void XtkMenuX11::draw() 
{
    double r, g, b;
    int y = 0;

    this->swap(this->theme()->string("menu", "backgroundcolor", "#ffffff"));

    colorHtmlToCairo(this->theme()->string("menu", "bordercolor", "#ffffff"), 
        r, g, b);
    cairo_set_source_rgb(m_context, r, g, b);
    cairo_set_line_width(m_context, this->theme()->getInt("menu", "borderwidth", 1));
    cairo_rectangle(m_context, 0, 0, m_width, m_height);
    cairo_stroke_preserve(m_context);

    m_curItems.clear();
    for (unsigned int i = 0; i < m_items.size(); i++) {
        if (m_items[i]->parent() == m_parentItem)  
            m_curItems.push_back(m_items[i]);
    }
        
    for (unsigned int i = 0; i < m_curItems.size(); i++) {
        // FIXME: hover is LOW efficiency ;(
#if 0
        // hover
        if (m_pointerY < (int)(i + 1) * m_itemHeight && 
            m_pointerY > (int)i * m_itemHeight) {
            colorHtmlToCairo(
                this->theme()->string("menu", "borderhovercolor", "#ffffff"), 
                r, g, b);
            cairo_set_source_rgb(m_context, r, g, b);
            cairo_set_line_width(m_context, 
                    this->theme()->getInt("menu", "borderwidth", 1));
            cairo_rectangle(m_context, 0, i * m_itemHeight, m_width, m_itemHeight);
            cairo_stroke(m_context);
        }
#endif
        // text
        XtkText textObj(this->surface(), 
                m_curItems[i]->text(), 30, y, m_width, m_itemHeight);
        textObj.draw();
        // increase
        y += m_itemHeight;
    }
}

}
