#ifndef DREAMCAST

#pragma once
#include "Window.h"

class PCWindow: public Window {
public:
    virtual void createWindow(std::string windowName, int width, int height) override;
    virtual void closeWindow() override;
    virtual void swapBuffers() override;
    virtual int windowShouldClose() override;
};

#endif