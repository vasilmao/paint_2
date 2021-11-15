#ifndef INCLUDE_APP
#define INCLUDE_APP

#include <time.h>
#include <stdio.h>
#include "GraphicLib.h"
// #include "DynamicArray.h"
#include "Tools.h"
#include "List.h"
#include "Window.h"
#include "Instruments.h"

class App {
private:
    friend Renderer;
    Renderer* renderer;

    bool running;

    int width;
    int height;

    void parseEvent(const SystemEvent& event);

    MainWindow* main_window;

    AbstractInstrument* instrument;

public:
    App();

    void run();
    void stop();

    ~App();
};

#endif