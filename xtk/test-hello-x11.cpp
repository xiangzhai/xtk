/* Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn> */

#include <xtk.h>
#include <iostream>
#include <vector>

class HelloWindowX11;

static Xtk::XtkDisplayX11* display = nullptr;
static Xtk::XtkEventX11* event = nullptr;
static Xtk::XtkWindowX11* window = nullptr;
static HelloWindowX11* parentWindow = nullptr;

class HelloWindowX11 : public Xtk::XtkWindowX11 
{
public:
    HelloWindowX11(Display* display, int x, int y, int width, int height) 
      : Xtk::XtkWindowX11(display, 
                          new Xtk::XtkTheme, 
                          x, 
                          y, 
                          width, 
                          height, 
                          "hello-world") 
    {
        std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
        text = new Xtk::XtkText(this->surface(), "Hello world 你好世界", 
                0, 0, 400, 80, 25);
        text->setFamily(this->theme()->string("font", "family", "Serif"));
        button = new Xtk::XtkButtonX11(this, "Clicke Me 点我", 0, 80, 100, 40);
        button->setButtonPressCallback(buttonPress);
        m_widgets.push_back(button);
    }
    ~HelloWindowX11() 
    {
        if (text) {
            delete text;
            text = nullptr;
        }

        if (button) {
            delete button;
            button = nullptr;
        }
    }

    std::vector<Xtk::XtkWidgetX11*> widgets() const { return m_widgets; }

    void draw() 
    {
        this->swap();
        text->draw();
        button->draw();
    }

private:
    static void* buttonPress(Xtk::XtkButtonX11* button, void* arg) 
    {
        std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " " << button << " " 
                  << arg << " " << button->text() << std::endl;
   
        return nullptr;
    }

private:
    Xtk::XtkText* text = nullptr;
    Xtk::XtkButtonX11* button = nullptr;
    std::vector<Xtk::XtkWidgetX11*> m_widgets;
};

static void cleanup() 
{
    if (parentWindow) {
        delete parentWindow;
        parentWindow = nullptr;
    }

    if (window) {
        delete window;
        window = nullptr;
    }

    if (event) {
        delete event;
        event = nullptr;
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

    // parentWindow
    parentWindow = new HelloWindowX11(display->display(), 0, 0, 800, 600);
    if (parentWindow == nullptr) {
        cleanup();
        return 1;
    }

    window = new Xtk::XtkWindowX11(display->display(), new Xtk::XtkTheme, 
                                   800, 0, 400, 300);
    if (window == nullptr) {
        cleanup();
        return 1;
    }

    // event loop
    event = new Xtk::XtkEventX11(display->display(), parentWindow);
    if (event == nullptr) {
        cleanup();
        return 1;
    }

    for (unsigned int i = 0; i < parentWindow->widgets().size(); i++) 
        event->connect(parentWindow->widgets()[i]);
    
    event->connect(window);

    event->run();

    /* Mr. cleanup */
    cleanup();

    return 0;
}
