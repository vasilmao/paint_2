#include "App.h"

const int WIDTH  = 1000;
const int HEIGHT = 750;

const Color bg_color = {185, 226, 235, 255};

App::App() {
    running = false;

    width = WIDTH;
    height = HEIGHT;
    renderer = new Renderer(width, height, bg_color);
    // printf("render created\n");
    main_window = new MainWindow(renderer, {width, height});
    // printf("constructed\n");
}

void App::run() {
    running = true;
    // printf("running\n");
    while (running) {
        SystemEvent sys_event = getSystemEvent();
        while (sys_event.event_type != SystemEventTypes::NO_EVENT) {
            // printf("event got\n");
            if (sys_event.event_type == SystemEventTypes::MOUSE_CLICK) {
                GUIMouseClickEvent event(sys_event);
                printf("uh oh mouse button society\n");
                main_window->onEvent(&event);
            } else if (sys_event.event_type == SystemEventTypes::MOUSE_MOVE) {
                GUIMouseMove event(sys_event);
                main_window->onEvent(&event);
            } else if (sys_event.event_type == SystemEventTypes::QUIT_EVENT) {
                running = false;
            }
            if (!running) {
                break;
            }
            printf("yaa\n");
            sys_event = getSystemEvent();
        }
        if (main_window->isToDelete()) {
            delete main_window;
            running = false;
        }
        if (!running) {
            break;
        }
        // printf("before render\n");
        main_window->render(renderer);
        // printf("present\n");
        renderer->render();
        SDL_Delay(500);
    }
    
}

void App::stop() {
    running = false;
}

App::~App() {}