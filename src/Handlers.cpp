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
    // printf("passing... %p\n", my_window);
    for (List<AbstractWindow*>::Iterator it = my_window->getChildren()->begin(); it.isValid(); ++it) {
        if (it.getData()->onEvent(event)) {
            // printf("found!\n");
            // printf("%p\n", my_window->getChildren());
            // printf("yaa, he got it!\n");
            my_window->getChildren()->makeFirst(it);
            // printf("eh...\n");
            return true;
        }
    }
    // printf("nobody :(\n");
    return false;
}

bool EventHandler::onEvent(GUIEvent* event) {
    switch (event->getType())
    {
        case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_BUTTON):
            // printf("passing mousebutton\n");
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

bool ButtonHandler::mbResponce(GUIMouseClickEvent* mouse_click) {
    Button* button = dynamic_cast<Button*>(my_window);
    if (my_window->hitTest(mouse_click->getPos())) {
        printf("btn hittest\n");
        if (!is_pressed && mouse_click->isButtonDown()) {
            is_pressed = true;
            button->setPressed();
            return true;
        } else if (is_pressed && !(mouse_click->isButtonDown())) {
            is_pressed = false;
            button->setUsual();
            return (*click_event_responce)();
        }
    } else if (is_pressed && !(mouse_click->isButtonDown())) {
        is_pressed = false;
        button->setUsual();
        return true;
    }
    return false;
}

bool ButtonHandler::mhResponce(GUIMouseMove* mouse_move) {
    Button* button = dynamic_cast<Button*>(my_window);
    bool first_result = my_window->hitTest(mouse_move->getPrevPos());
    bool second_result = my_window->hitTest(mouse_move->getNewPos());
    if ((first_result || second_result)) {
        if (!first_result && second_result) {
            is_hovered = true;
            button->setHover();
            return true;
            // return (*start_hover)(mouse_move->getNewPos());
        } else if (first_result && !second_result) {
            is_hovered = false;
            if (!is_pressed) {
                button->setUsual();
            }
            return true;
            // return (*end_hover)(mouse_move->getNewPos());
        }
    }
    return false;
}

bool ButtonHandler::onEvent(GUIEvent* event) {
    // Button* button = dynamic_cast<Button*>(my_window);
    // printf("button got event\n");
    switch (event->getType())
    {
        case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_BUTTON): {
            GUIMouseClickEvent* mouse_click = dynamic_cast<GUIMouseClickEvent*> (event);
            return mbResponce(mouse_click);
        }

        case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_MOVE) : {
            // GUIMouseMove* mouse_move = dynamic_cast<GUIMouseMove*>(event);
            // return mhResponce(mouse_move);
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

bool TitlebarHandler::mbResponce(GUIMouseClickEvent* mouse_click) {
    // printf("titlebar got mouseclick\n");
    if (my_window->hitTest(mouse_click->getPos())) {
        if (!is_holding && mouse_click->isButtonDown()) {
            is_holding = true;
            printf("titlebar captured!\n");
            return true;
        } else if (is_holding && !(mouse_click->isButtonDown())) {
            is_holding = false;
            printf("titlebar released!\n");
            return true;
        }
    }
    return false;
}

bool TitlebarHandler::onEvent(GUIEvent* event) {
    // printf("titlebar got event!\n");
    switch (event->getType())
    {
        case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_BUTTON): {
            if (!passEvent(event)) {
                GUIMouseClickEvent* mouse_click = dynamic_cast<GUIMouseClickEvent*> (event);
                return mbResponce(mouse_click);
            }
            return true;
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

CanvasHandler::CanvasHandler(AbstractWindow* window, Renderer* renderer, Functor<Renderer*, Texture*, const Vector2&>* canvas_drawer) :
    EventHandler(window), renderer(renderer), canvas_drawer(canvas_drawer), current_instrument(nullptr) {

}

bool CanvasHandler::mbResponce(GUIMouseClickEvent* mouse_click) {
    if (my_window->hitTest(mouse_click->getPos())) {
        // printf("ye, canvas hittest!\n");
        if (!is_pressed && (mouse_click->isButtonDown())) {
            is_pressed = true;
            Vector2 mouseclick_pos = mouse_click->getPos();
            (*canvas_drawer)(renderer, my_window->getSkin()->getTexture(), mouseclick_pos - my_window->getPos());
            return true;
        } else if (is_pressed && !(mouse_click->isButtonDown())) {
            is_pressed = false;
        }
    }
    return false;
}

bool CanvasHandler::mmResponce(GUIMouseMove* mouse_move) {
    if (is_pressed) {
        bool first_result = my_window->hitTest(mouse_move->getPrevPos());
        bool second_result = my_window->hitTest(mouse_move->getNewPos());
        if (first_result && second_result) {
            Vector2 mouse_pos = mouse_move->getNewPos();
            (*canvas_drawer)(renderer, my_window->getSkin()->getTexture(), mouse_pos - my_window->getPos());
            return true;
        } else {
            is_pressed = false;
        }
    }
    return false;
}

bool CanvasHandler::onEvent(GUIEvent* event) {
    // printf("canvas handler got an event!\n");
    switch (event->getType())
    {
        case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_BUTTON): {
            // no pass event
            GUIMouseClickEvent* mouse_click = dynamic_cast<GUIMouseClickEvent*> (event);
            return mbResponce(mouse_click);
        }
        
        case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_MOVE): {
            if (is_pressed) {
                GUIMouseMove* mouse_move = dynamic_cast<GUIMouseMove*>(event);
                return mmResponce(mouse_move);
            }
            return spreadEvent(event);
        }

        case static_cast<int>(GUIEvent::GUIEventTypes::INSTRUMENT_CHANGED): {
            // printf("das a new instrument!\n");
            GUIInstrumentChanged* instr_event = dynamic_cast<GUIInstrumentChanged*>(event);
            // Canvas* canvas = dynamic_cast<Canvas*>(my_window);
            current_instrument = instr_event->getInstrument();
            CanvasDrawerFunctor* canv_drawer = dynamic_cast<CanvasDrawerFunctor*>(canvas_drawer);
            // printf("ugh setting...\n");
            // printf("%p\n", canv_drawer);
            canv_drawer->setInstrument(current_instrument);
            // printf("ya, new instrument!\n");
            return true;
        }
    
        default:
            return spreadEvent(event);
            break;
    }
}

CanvasHandler::~CanvasHandler() {
    delete canvas_drawer;
}