// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef XTK_BUTTON_X11_H
#define XTK_BUTTON_X11_H

#include <xtkwindow-x11.h>
#include <xtktext.h>

#include <string>

namespace Xtk 
{

class XtkButtonX11;

typedef void* (*BUTTON_PRESS_CALLBACK)(XtkButtonX11*, void*);

class XtkButtonX11 : public XtkWindowX11 
{
public:
    XtkButtonX11(XtkWindowX11* parent, 
                 std::string textStr, 
                 int x, 
                 int y, 
                 int width, 
                 int height);
    ~XtkButtonX11();

    std::string text() const { return m_textStr; }

    void setButtonPressCallback(BUTTON_PRESS_CALLBACK buttonPressCallback, 
                                void* arg = nullptr);

    void buttonPress();
    void draw();

private:
    XtkWindowX11* m_parent = nullptr;
    std::string m_textStr;
    int m_width = 0;
    int m_height = 0;
    XtkText* m_text = nullptr;
    BUTTON_PRESS_CALLBACK m_buttonPressCallback = nullptr;
    void* m_arg = nullptr;
};

};

#endif // XTK_BUTTON_X11_H
