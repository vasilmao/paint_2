#include "Handlers.h"

EventHandler::~EventHandler() {

}

bool EventHandler::spreadEvent(GUIEvent* event) {
    bool result = false;
    for (List<AbstractWindow*>::Iterator it = my_window->getChildren()->begin(); it.isValid(); ++it) {
        result |= it.getData()->onEvent(event);
    }
    return result;
}

bool EventHandler::passEvent(GUIEvent* event) {
    // printf("passing...\n");
    for (List<AbstractWindow*>::Iterator it = my_window->getChildren()->begin(); it.isValid(); ++it) {
        if (it.getData()->onEvent(event)) {
            // printf("found!\n");
            // printf("%p\n", my_window->getChildren());
            my_window->getChildren()->makeFirst(it);
            // printf("eh...\n");
            return true;
        }
    }
    return false;
}

bool EventHandler::onEvent(GUIEvent* event) {
    switch (event->getType())
    {
        case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_BUTTON):
            return passEvent(event);
            break;
    
        default: {
            return spreadEvent(event);
        }
    }
}

void EventHandler::setWindow(AbstractWindow* window) {
    my_window = window;
}

ButtonHandler::ButtonHandler(AbstractWindow* window, Functor<>* click_functor) : EventHandler(window), click_event_responce(click_functor){}

bool ButtonHandler::onEvent(GUIEvent* event) {
    Button* button = dynamic_cast<Button*>(my_window);
    // printf("button got event\n");
    switch (event->getType())
    {
        case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_BUTTON): {
            printf("oh, mb, button\n");
            GUIMouseClickEvent* mouse_click = dynamic_cast<GUIMouseClickEvent*> (event);
            if (my_window->hitTest(mouse_click->getPos())) {
                printf("yes, inside!\n");
                printf("%d, %d\n", (int)is_pressed, (int)(mouse_click->isButtonDown()));
                if (!is_pressed && mouse_click->isButtonDown()) {
                    printf("down!\n");
                    is_pressed = true;
                    button->setPressed();
                    printf("ya!\n");
                    return true;
                } else if (is_pressed && !(mouse_click->isButtonDown())) {
                    printf("up!\n");
                    is_pressed = false;
                    button->setUsual();
                    return (*click_event_responce)();
                }
            } else if (is_pressed && !(mouse_click->isButtonDown())) {
                printf("up :(\n");
                is_pressed = false;
                button->setUsual();
                return true;
            }
            return false;
            break;
        }

        case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_MOVE) : {
            // GUIMouseMove* mouse_move = dynamic_cast<GUIMouseMove*>(event);
            // bool first_result = my_window->hitTest(mouse_move->getPrevPos());
            // bool second_result = my_window->hitTest(mouse_move->getNewPos());
            // if ((first_result || second_result)) {
            //     if (!first_result && second_result) {
            //         is_hovered = true;
            //         button->setHover();
            //         return true;
            //         // return (*start_hover)(mouse_move->getNewPos());
            //     } else if (first_result && !second_result) {
            //         is_hovered = false;
            //         if (!is_pressed) {
            //             button->setUsual();
            //         }
            //         return true;
            //         // return (*end_hover)(mouse_move->getNewPos());
            //     }
            // }
            return spreadEvent(event);
        }
        default: {
            return spreadEvent(event);
        }
    }
}

ButtonHandler::~ButtonHandler() {
    delete click_event_responce;
}

TitlebarHandler::TitlebarHandler(AbstractWindow* window, Functor<const Vector2&>* move_functor) : EventHandler(window), window_mover(move_functor) {

}

bool TitlebarHandler::onEvent(GUIEvent* event) {
    switch (event->getType())
    {
        case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_BUTTON): {
            GUIMouseClickEvent* mouse_click = dynamic_cast<GUIMouseClickEvent*> (event);
            if (my_window->hitTest(mouse_click->getPos())) {
                if (!passEvent(event)) {
                    if (!is_holding && mouse_click->isButtonDown()) {
                        is_holding = true;
                        return true;
                    } else if (is_holding && !(mouse_click->isButtonDown())) {
                        is_holding = true;
                        return true;
                    }
                }
            }
            return passEvent(event);
            break;
        }

        case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_MOVE): {
            if (is_holding) {
                GUIMouseMove* mouse_move = dynamic_cast<GUIMouseMove*>(event);
                Vector2 delta = mouse_move->getNewPos() - mouse_move->getPrevPos();
                return (*window_mover)(delta);
            }
            return spreadEvent(event);
        }

        default:
            return spreadEvent(event);
            break;
    }
}

TitlebarHandler::~TitlebarHandler() {
    delete window_mover;
}