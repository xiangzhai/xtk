// Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "xtkevent-x11.h"

#include <iostream>
#include <algorithm>

namespace Xtk 
{

XtkEventX11::XtkEventX11(Display* display, XtkWidgetX11* parent)
  : m_display(display), 
    m_parent(parent) 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    widgets.push_back(m_parent);
}

XtkEventX11::~XtkEventX11() 
{
#if XTK_DEBUG
    std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
#endif
}

void XtkEventX11::setClientMessageCallback(
        CLIENT_MESSAGE_CALLBACK clientMessageCallback, 
        void* arg) 
{
    m_clientMessageCallback = clientMessageCallback;
    m_clientMessageArg = arg;
}

void XtkEventX11::setPropertyNotifyCallback(
        PROPERTY_NOTIFY_CALLBACK propertyNotifyCallback, 
        void* arg) 
{
    m_propertyNotifyCallback = propertyNotifyCallback;
    m_propertyNotifyArg = arg;
}

void XtkEventX11::setXDamageNotifyCallback(
        XDAMAGE_NOTIFY_CALLBACK xDamageNotifyCallback, 
        void* arg) 
{
    m_xDamageNotifyCallback = xDamageNotifyCallback;
    m_xDamageNotifyArg = arg;
}

void XtkEventX11::setDestroyNotifyCallback(
        DESTROY_NOTIFY_CALLBACK destroyNotifyCallback, 
        void* arg) 
{
    m_destroyNotifyCallback = destroyNotifyCallback;
    m_destroyNotifyArg = arg;
}

void XtkEventX11::setConfigureNotifyCallback(
        CONFIGURE_NOTIFY_CALLBACK configureNotifyCallback, 
        void* arg) 
{
    m_configureNotifyCallback = configureNotifyCallback;
    m_configureNotifyArg = arg;
}

void XtkEventX11::connect(XtkWidgetX11* widget) { widgets.push_back(widget); }

void XtkEventX11::disconnect(XtkWidgetX11* widget) 
{
    std::vector<XtkWidgetX11*>::iterator iter = std::find(widgets.begin(), 
            widgets.end(), widget);

    if (iter == widgets.end()) { 
        return;
    }

    widgets.erase(iter);
}

void XtkEventX11::run() 
{
    int damageEventBase = 0, damageErrorBase;

    XDamageQueryExtension(m_display, &damageEventBase, &damageErrorBase);

    while (quit == false && !XNextEvent(m_display, &m_event)) {
        // X.h Event names
        if (m_event.type == ClientMessage) {
            if ((Atom)m_event.xclient.data.l[0] == m_parent->wmDeleteMessage()) {
                if (m_event.xclient.window == m_parent->window()) {
                    quit = true;
                    return;
                } else {
                    for (unsigned int i = 0; i < widgets.size(); i++) {
                        if (widgets[i]->window() == m_event.xclient.window) {
                            widgets[i]->close();
                        }
                    }
                }
            }

            if (m_clientMessageCallback) 
                m_clientMessageCallback(this, m_clientMessageArg);
        }
        
        if (m_event.type == Expose) {
            for (unsigned int i = 0; i < widgets.size(); i++) {
                if (widgets[i]->window() == m_event.xexpose.window) {
                    widgets[i]->draw();
                }
            }
        }
        
        if (m_event.type == PropertyNotify) {
            if (m_propertyNotifyCallback) 
                m_propertyNotifyCallback(this, m_propertyNotifyArg);
        }

        if (m_event.type == damageEventBase + XDamageNotify) {
            if (m_xDamageNotifyCallback) 
                m_xDamageNotifyCallback(this, m_xDamageNotifyArg);
        }

        if (m_event.type == DestroyNotify) {
            if (m_destroyNotifyCallback) 
                m_destroyNotifyCallback(this, m_destroyNotifyArg);
        }

        if (m_event.type == ConfigureNotify) {
            for (unsigned int i = 0; i < widgets.size(); i++) {
                if (widgets[i]->window() == m_event.xconfigure.window) {
                    widgets[i]->resize(m_event.xconfigure.width, 
                                       m_event.xconfigure.height);
                    widgets[i]->draw();
                }
            }

            if (m_configureNotifyCallback) 
                m_configureNotifyCallback(this, m_configureNotifyArg);
        }

        if (m_event.type == EnterNotify) {
            for (unsigned int i = 0; i < widgets.size(); i++) {
                if (widgets[i]->window() == m_event.xbutton.window) {
                    widgets[i]->enterNotify();
                }
            }
        }

        if (m_event.type == LeaveNotify) {
            for (unsigned int i = 0; i < widgets.size(); i++) {
                if (widgets[i]->window() == m_event.xbutton.window) {
                    widgets[i]->leaveNotify();
                }
            }
        }

        if (m_event.type == ButtonPress) {
            for (unsigned int i = 0; i < widgets.size(); i++) {
                if (widgets[i]->window() == m_event.xbutton.window) {
                    widgets[i]->buttonPress();
                }
            }
        }
    }
}

};
