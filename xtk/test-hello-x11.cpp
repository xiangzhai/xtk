/* Copyright (C) 2014 Leslie Zhai <xiang.zhai@i-soft.com.cn> */

#include <xtk.h>
#include <iostream>

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
                10, 0, 400, 80, 25);
        text->setFamily(this->theme()->string("font", "family", "Serif"));
        
        button = new Xtk::XtkButtonX11(this, "Clicke Me 点我", 10, 80, 100, 40);
        button->setButtonPressCallback(buttonPress);
        
        eventButton = new Xtk::XtkButtonX11(this, 
                "Click Me then disconnect 点我就断开信号", 10, 130, 400, 40);
        eventButton->setButtonPressCallback(eventButtonPress, this);
    
        imageButton = new Xtk::XtkButtonX11(this, "START", 10, 500, 40, 40);
        imageButton->setButtonPressCallback(imageButtonPress, this);
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

        if (eventButton) {
            delete eventButton;
            eventButton = nullptr;
        }

        if (imageButton) {
            delete imageButton;
            imageButton = nullptr;
        }
    }

    void eventButtonPress() 
    {
        std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << std::endl;
    }

    void setEvent(Xtk::XtkEventX11* event) 
    {
        if (m_event == event) 
            return;

        m_event = event;
        
        // TODO: dynamic event connect
        if (button) 
            m_event->connect(button);
        
        if (eventButton) 
            m_event->connect(eventButton);
    
        if (imageButton)
            m_event->connect(imageButton);
    }

    void draw() 
    {
        this->swap();

        text->draw();

        if (button) 
            button->draw();

        if (eventButton) 
            eventButton->draw();

        if (imageButton) 
            imageButton->draw();
    }

private:
    static void* buttonPress(Xtk::XtkButtonX11* button, void* arg) 
    {
        std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " " << button << " " 
                  << arg << " " << button->text() << std::endl;
   
        return nullptr;
    }

    static void* eventButtonPress(Xtk::XtkButtonX11* button, void* arg) 
    {
        HelloWindowX11* thisPtr = reinterpret_cast<HelloWindowX11*>(arg);
        std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " " << button << " " 
                  << arg << " " << button->text() << std::endl;
        // TODO: dynamic event disconnect
        if (thisPtr->m_event) 
            thisPtr->m_event->disconnect(button);

        button->setText("disconnected 断开信号了");
        return nullptr;
    }

    static void* imageButtonPress(Xtk::XtkButtonX11* button, void* arg) 
    {
        HelloWindowX11* thisPtr = reinterpret_cast<HelloWindowX11*>(arg);
        std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " " << button << " " 
                  << arg << " " << button->text() << std::endl;
        // TODO: menu stuff
        if (thisPtr->menu == nullptr) { 
            thisPtr->menu = new Xtk::XtkMenuX11(thisPtr, 10, 500, 100, 100);
            thisPtr->m_event->connect(thisPtr->menu);
            thisPtr->menu->draw();
        } else 
            thisPtr->menu->show();
        return nullptr;
    }

private:
    Xtk::XtkText* text = nullptr;
    Xtk::XtkButtonX11* button = nullptr;
    Xtk::XtkButtonX11* eventButton = nullptr;
    Xtk::XtkButtonX11* imageButton = nullptr;
    Xtk::XtkMenuX11* menu = nullptr;
    Xtk::XtkEventX11* m_event = nullptr;
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
    // TODO: dynamic event connect
    parentWindow->setEvent(event);
    // TODO: static event connect
    event->connect(window);
    event->run();

    cleanup();

    return 0;
}
