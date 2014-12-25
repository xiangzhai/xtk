/* Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn> */

#include <xtk.h>
#include <iostream>

class HelloWindowX11;

static Xtk::XtkDisplayX11* display = nullptr;
static Xtk::XtkEventX11* event = nullptr;
static HelloWindowX11* window = nullptr;

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
        m_button = new Xtk::XtkButtonX11(this, "Clicke Me 点我", 0, 80, 400, 80);
        m_button->setButtonPressCallback(buttonPress);
    }
    ~HelloWindowX11() 
    {
        if (text) {
            delete text;
            text = nullptr;
        }

        if (m_button) {
            delete m_button;
            m_button = nullptr;
        }
    }

    Xtk::XtkButtonX11* button() const { return m_button; }

    void draw() 
    {
        this->swap();
        text->draw();
        m_button->draw();
    }

private:
    static void* buttonPress(Xtk::XtkButtonX11* button, void* arg) 
    {
        std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " " << button << " " 
                  << arg << " " << button->text() << std::endl;
   
        button->setSize(100, 40);
        return nullptr;
    }

private:
    Xtk::XtkText* text = nullptr;
    Xtk::XtkButtonX11* m_button = nullptr;
};

static void cleanup() 
{
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

    // window
    window = new HelloWindowX11(display->display(), 0, 0, 400, 300);
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
    event->connect(window->button());
    event->run();

    /* Mr. cleanup */
    cleanup();

    return 0;
}
