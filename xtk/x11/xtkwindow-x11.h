// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef XTK_WINDOW_X11_H
#define XTK_WINDOW_X11_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <xtktheme.h>
#include <x11/xtkwidget-x11.h>
#include <x11/xtkevent-x11.h>
#include <string>
#include <vector>

namespace Xtk 
{

// http://standards.freedesktop.org/wm-spec/latest/ar01s05.html                    
typedef enum {                                                                     
    _NET_WM_WINDOW_TYPE_NORMAL,                                                    
    _NET_WM_WINDOW_TYPE_DIALOG,                                                    
    _NET_WM_WINDOW_TYPE_MENU,
    _NET_WM_WINDOW_TYPE_DOCK,                                                      
    _NET_WM_WINDOW_TYPE_DESKTOP,                                                   
    _NET_WM_WINDOW_TYPE_FRAMELESS                                                  
} X11WindowType;

struct MwmHints {
    unsigned long flags;
    unsigned long functions;
    unsigned long decorations;
    long input_mode;
    unsigned long status;
};

enum {
    MWM_HINTS_FUNCTIONS = (1L << 0),
    MWM_HINTS_DECORATIONS =  (1L << 1),
    MWM_FUNC_ALL = (1L << 0),
    MWM_FUNC_RESIZE = (1L << 1),
    MWM_FUNC_MOVE = (1L << 2),
    MWM_FUNC_MINIMIZE = (1L << 3),
    MWM_FUNC_MAXIMIZE = (1L << 4),
    MWM_FUNC_CLOSE = (1L << 5)
};

class XtkWindowX11 : public XtkWidgetX11 
{
public:
    XtkWindowX11(Display* display,
                 XtkTheme* theme, 
                 int x, 
                 int y, 
                 int width, 
                 int height, 
                 std::string name = "", 
                 Window parent = None, 
                 int borderWidth = 0, 
                 X11WindowType type = _NET_WM_WINDOW_TYPE_NORMAL);
    virtual ~XtkWindowX11();

    Display* display() const { return m_display; }
    XtkTheme* theme() const { return m_theme; }
    XtkEventX11* event() const { return m_event; }
    void setEvent(XtkEventX11* event) { m_event = event; }
    Window window() const { return m_window; }
    Visual* visual() const { return m_visual; }
    Atom wmDeleteMessage() const { return m_wmDeleteMessage; }
    int x() const { return m_x; }
    int y() const { return m_y; }
    int width() const { return m_width; }
    int height() const { return m_height; }
    std::string name() const { return m_name; }
    cairo_surface_t* surface() const { return m_surface; }
    std::vector<XtkWidgetX11*> widgets() const { return m_widgets; }

    void close();
    void show();
    void hide();
    void setSize(int width, int height);
    void resize(int width, int height);
    void draw();
    void setAlpha(double alpha);
    void swap(std::string color = "");

private:
    Display* m_display = nullptr;
    XtkTheme* m_theme = nullptr;
    XtkEventX11* m_event = nullptr;
    Window m_window = None;
    Visual* m_visual = nullptr;
    Atom m_wmDeleteMessage;
    int m_x = 0;
    int m_y = 0;
    int m_width = 0;
    int m_height = 0;
    std::string m_name;
    cairo_surface_t* m_surface = nullptr;
    cairo_t* context = nullptr;
    std::vector<XtkWidgetX11*> m_widgets;
    double m_alpha = 1.0;
};

};

#endif // XTK_WINDOW_X11_H
