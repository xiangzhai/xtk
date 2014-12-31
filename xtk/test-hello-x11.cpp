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
        std::cout << "DEBUG: " << __PRETTY_FUNCTION__  << " " << this->window() 
                  << std::endl;
        text = new Xtk::XtkText(this->surface(), "Hello world 你好世界", 
                10, 0, 400, 80, 25);
        text->setFamily(this->theme()->string("font", "family", "Serif"));
        
        button = new Xtk::XtkButtonX11(this, "Clicke Me 点我", 10, 80, 100, 40);
        button->setButtonPressCallback(buttonPress);
        button->setRightButtonPressCallback(rightButtonPress, this);
        
        eventButton = new Xtk::XtkButtonX11(this, 
                "Click Me then disconnect 点我就断开信号", 10, 130, 400, 40);
        eventButton->setButtonPressCallback(eventButtonPress, this);
    
        imageButton = new Xtk::XtkButtonX11(this, "Menu", 10, 500, 40, 40);
        imageButton->setButtonPressCallback(imageButtonPress, this);
    
        menuButton = new Xtk::XtkButtonX11(this, "Menu2", 60, 500, 40, 40);
        menuButton->setButtonPressCallback(menuButtonPress, this);
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

        if (menuButton) {
            delete menuButton;
            menuButton = nullptr;
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

        if (menuButton)
            m_event->connect(menuButton);
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

        if (menuButton)
            menuButton->draw();
    }

private:
    static void* buttonPress(Xtk::XtkButtonX11* button, void* arg) 
    {
        std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " " << button << " " 
                  << arg << " " << button->text() << std::endl;
        return nullptr;
    }

    static void* rightButtonPress(Xtk::XtkButtonX11* button, void* arg) 
    {
        HelloWindowX11* thisPtr = reinterpret_cast<HelloWindowX11*>(arg);
        std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " " << button << " " 
                  << arg << " " << button->text() << std::endl;
        thisPtr->quit(thisPtr->window());
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

    static void* menuItemCallback(Xtk::XtkMenuX11* menu, void* arg) 
    {
        char* text = reinterpret_cast<char*>(arg);
        std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " " << text << std::endl;
        return nullptr;
    }

    static void* imageButtonPress(Xtk::XtkButtonX11* button, void* arg) 
    {
        HelloWindowX11* thisPtr = reinterpret_cast<HelloWindowX11*>(arg);
        std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " " << button << " " 
                  << arg << " " << button->text() << std::endl;
        thisPtr->menu1 = new Xtk::XtkMenuX11(thisPtr, 10, 500, 200);
        thisPtr->menu1->setEvent(thisPtr->m_event);
        for (int i = 0; i < 8; i++) { 
            std::string text = "MENU-ITEM-" + std::to_string(i + 1);
            Xtk::XtkMenuItem* item = new Xtk::XtkMenuItem(text, 
                    thisPtr->menuItemCallback, (void*)text.c_str());
            thisPtr->menu1->addItem(item);
            for (int j = 0; j < 6; j++) {
                std::string text = "MENU-SUB1-ITEM-" + 
                    std::to_string(i + 1) + "-" + std::to_string(j + 1);
                Xtk::XtkMenuItem* sub1item = new Xtk::XtkMenuItem(text, 
                        thisPtr->menuItemCallback, (void*)text.c_str(), item);
                thisPtr->menu1->addItem(sub1item);
                for (int k = 0; k < 4; k++) {
                    std::string text = "MENU-SUB2-ITEM-" + 
                        std::to_string(i + 1) + "-" + std::to_string(j + 1) + 
                        "-" + std::to_string(k + 1);
                    Xtk::XtkMenuItem* sub2item = new Xtk::XtkMenuItem(text, 
                            thisPtr->menuItemCallback, (void*)text.c_str(), 
                            sub1item);
                    thisPtr->menu1->addItem(sub2item);
                    for (int l = 0; l < 2; l++) {
                        std::string text = "MENU-SUB3-ITEM-" + 
                            std::to_string(i + 1) + "-" + 
                            std::to_string(j + 1) + "-" + 
                            std::to_string(k + 1) + "-" + 
                            std::to_string(l + 1);
                        Xtk::XtkMenuItem* sub3item = new Xtk::XtkMenuItem(
                                text, thisPtr->menuItemCallback, 
                                (void*)text.c_str(), sub2item);
                        thisPtr->menu1->addItem(sub3item);
                    }
                }
            }
        }
        thisPtr->m_event->connect(thisPtr->menu1);
        thisPtr->menu1->draw();
        return nullptr;
    }

    static void* menuButtonPress(Xtk::XtkButtonX11* button, void* arg) 
    {
        HelloWindowX11* thisPtr = reinterpret_cast<HelloWindowX11*>(arg);
        std::cout << "DEBUG: " << __PRETTY_FUNCTION__ << " " << button << " " 
                  << arg << " " << button->text() << std::endl;
        thisPtr->menu2 = new Xtk::XtkMenuX11(thisPtr, 60, 500, 100);
        std::string text = "Close";
        thisPtr->menu2->addItem(text, thisPtr->menuItemCallback, (void*)text.c_str());
        thisPtr->menu2->setEvent(thisPtr->m_event);
        thisPtr->m_event->connect(thisPtr->menu2);
        thisPtr->menu2->draw();
        return nullptr;
    }

private:
    Xtk::XtkText* text = nullptr;
    Xtk::XtkButtonX11* button = nullptr;
    Xtk::XtkButtonX11* eventButton = nullptr;
    Xtk::XtkButtonX11* imageButton = nullptr;
    Xtk::XtkButtonX11* menuButton = nullptr;
    Xtk::XtkMenuX11* menu1 = nullptr;
    Xtk::XtkMenuX11* menu2 = nullptr;
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
    window->setAlpha(0.1);

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
