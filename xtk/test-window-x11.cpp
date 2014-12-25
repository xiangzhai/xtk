/* Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn> */

#include <xtk.h>
#include <iostream>

static Xtk::XtkDisplayX11* display = nullptr;
static Xtk::XtkTheme* theme = nullptr;
static Xtk::XtkWindowX11* window = nullptr;
static Xtk::XtkEventX11* event = nullptr;

static void cleanup() 
{
    if (event) {
        delete event;
        event = nullptr;
    }

    if (window) {
        delete window;
        window = nullptr;
    }

    if (theme) {
        delete theme;
        theme = nullptr;
    }

    if (display) {
        delete display;
        display = nullptr;
    }
}

int main(int argc, char* argv[]) 
{
    // display
    display = new Xtk::XtkDisplayX11;
    if (display == nullptr) {
        cleanup();
        return 1;
    }

    // theme
    theme = new Xtk::XtkTheme;
    if (theme == nullptr) {
        cleanup();
        return 1;
    }

    // window
    window = new Xtk::XtkWindowX11(display->display(), theme, 0, 0, 300, 200);
    if (window == nullptr) {
        cleanup();
        return 1;
    }

    // event loop
    event = new Xtk::XtkEventX11(display->display(), window);
    if (event == nullptr) {
        cleanup();
        return 1;
    }
    event->run();

    /* Mr. cleanup */
    cleanup();

    return 0;
}
