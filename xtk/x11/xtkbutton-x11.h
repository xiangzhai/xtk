// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef XTK_BUTTON_X11_H
#define XTK_BUTTON_X11_H

#include <x11/xtkwindow-x11.h>
#include <cairo.h>
#include <xtktext.h>

#include <string>

namespace Xtk 
{

class XtkButtonX11;

typedef void* (*BUTTON_PRESS_CALLBACK)(XtkButtonX11*, void*);
typedef void* (*RIGHT_BUTTON_PRESS_CALLBACK)(XtkButtonX11*, void*);

class XtkButtonX11 : public XtkWindowX11 
{
public:
    XtkButtonX11(XtkWindowX11* parent, 
                 std::string text, 
                 int x, 
                 int y, 
                 int width, 
                 int height);
    ~XtkButtonX11();

    std::string text() const { return m_text; }
    void setText(const std::string & text);

    void setButtonPressCallback(BUTTON_PRESS_CALLBACK buttonPressCallback, 
                                void* arg = nullptr);
    void setRightButtonPressCallback(
            RIGHT_BUTTON_PRESS_CALLBACK rightButtonPressCallback, 
            void* arg = nullptr);

    void enterNotify();
    void leaveNotify();
    void buttonPress(XButtonEvent event);
    void setSize(int width, int height);
    void draw();

private:
    XtkWindowX11* m_parent = nullptr;
    std::string m_text;
    int m_width = 0;
    int m_height = 0;
    XtkText* m_textObj = nullptr;
    cairo_t* context = nullptr;
    BUTTON_PRESS_CALLBACK m_buttonPressCallback = nullptr;
    RIGHT_BUTTON_PRESS_CALLBACK m_rightButtonPressCallback = nullptr;
    void* m_buttonPressArg = nullptr;
    void* m_rightButtonPressArg = nullptr;
};

};

#endif // XTK_BUTTON_X11_H
