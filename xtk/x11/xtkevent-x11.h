// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#ifndef XTK_EVENT_X11_H
#define XTK_EVENT_X11_H

#include <X11/Xlib.h>
#include <X11/extensions/Xdamage.h>
#include <x11/xtkwidget-x11.h>
#include <vector>

namespace Xtk 
{

class XtkEventX11;

typedef void* (*CLIENT_MESSAGE_CALLBACK)(XtkEventX11*, void*);
typedef void* (*PROPERTY_NOTIFY_CALLBACK)(XtkEventX11*, void*);
typedef void* (*XDAMAGE_NOTIFY_CALLBACK)(XtkEventX11*, void*);
typedef void* (*DESTROY_NOTIFY_CALLBACK)(XtkEventX11*, void*);
typedef void* (*CONFIGURE_NOTIFY_CALLBACK)(XtkEventX11*, void*);

class XtkEventX11 
{
public:
    XtkEventX11(Display* display, XtkWidgetX11* parent);
    ~XtkEventX11();

    Display* display() const { return m_display; }
    XEvent event() const { return m_event; }

    void setClientMessageCallback(CLIENT_MESSAGE_CALLBACK clientMessageCallback, 
                                  void* arg = nullptr);
    void setPropertyNotifyCallback(PROPERTY_NOTIFY_CALLBACK propertyNotifyCallback, 
                                   void* arg = nullptr);
    void setXDamageNotifyCallback(XDAMAGE_NOTIFY_CALLBACK xDamageNotifyCallback, 
                                  void* arg = nullptr);
    void setDestroyNotifyCallback(DESTROY_NOTIFY_CALLBACK destroyNotifyCallback, 
                                  void* arg = nullptr);
    void setConfigureNotifyCallback(
            CONFIGURE_NOTIFY_CALLBACK configureNotifyCallback, 
            void* arg = nullptr);
    void connect(XtkWidgetX11* widget);
    void disconnect(XtkWidgetX11* widget);
    void run();

private:
    Display* m_display = nullptr;
    XtkWidgetX11* m_parent = nullptr;
    XEvent m_event;
    bool quit = false;
    std::vector<XtkWidgetX11*> widgets;
    CLIENT_MESSAGE_CALLBACK m_clientMessageCallback;
    PROPERTY_NOTIFY_CALLBACK m_propertyNotifyCallback;
    XDAMAGE_NOTIFY_CALLBACK m_xDamageNotifyCallback;
    DESTROY_NOTIFY_CALLBACK m_destroyNotifyCallback;
    CONFIGURE_NOTIFY_CALLBACK m_configureNotifyCallback;
    void* m_clientMessageArg = nullptr;
    void* m_propertyNotifyArg = nullptr;
    void* m_xDamageNotifyArg = nullptr;
    void* m_destroyNotifyArg = nullptr;
    void* m_configureNotifyArg = nullptr;
};

};

#endif // XTK_EVENT_X11_H
