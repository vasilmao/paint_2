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
        case static_cast<int>(GUIEvent::GUIEventTypes::LEFT_MOUSE_BUTTON): {
            if (passEvent(event)) {
                return true;
            }
            GUILeftMouseButton* lmb_event = dynamic_cast<GUILeftMouseButton*>(event);
            return MBLResponce(lmb_event);
            // printf("passing mousebutton\n");
            // return passEvent(event);
            break;
        }

        case static_cast<int>(GUIEvent::GUIEventTypes::RIGHT_MOUSE_BUTTON): {
            if (passEvent(event)) {
                return true;
            }
            GUIRightMouseButton* rmb_event = dynamic_cast<GUIRightMouseButton*>(event);
            return MBRResponce(rmb_event);
            // printf("passing mousebutton\n");
            // return passEvent(event);
            break;
        }

        case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_MOVE): {
            if (passEvent(event)) {
                return true;
            }
            GUIMouseMove* mm_event = dynamic_cast<GUIMouseMove*>(event);
            return MMResponce(mm_event);
            // printf("passing mousebutton\n");
            // return passEvent(event);
            break;
        }

        case static_cast<int>(GUIEvent::GUIEventTypes::LIST_ELEMENT_CHANGED): {
            if (spreadEvent(event)) {
                return true;
            }
            GUIListElementChanged* list_event = dynamic_cast<GUIListElementChanged*>(event);
            return LECResponce(list_event);
            // printf("passing mousebutton\n");
            // return passEvent(event);
            break;
        }
    
        default: {
            return spreadEvent(event);
        }
    }
}

bool EventHandler::MBLResponce(GUILeftMouseButton* event) {
    return false;
}

bool EventHandler::MBRResponce(GUIRightMouseButton* event) {
    return false;
}

bool EventHandler::MMResponce(GUIMouseMove* event) {
    return false;
}

bool EventHandler::LECResponce(GUIListElementChanged* event) {
    return false;
}

void EventHandler::setWindow(AbstractWindow* window) {
    my_window = window;
}

ButtonHandler::ButtonHandler(AbstractWindow* window, Functor<>* click_functor) : EventHandler(window), click_event_responce(click_functor){}

bool ButtonHandler::MBLResponce(GUILeftMouseButton* mouse_click) {
    Button* button = dynamic_cast<Button*>(my_window);
    // printf("chekin\n");
    if (my_window->hitTest(mouse_click->getPos())) {
        // printf("btn hittest\n");
        printf("%p\n", mouse_click);
        if (!is_pressed && mouse_click->isButtonDown()) {
            is_pressed = true;
            button->setPressed();
            // printf("setted pressed\n");
            return true;
        } else if (is_pressed && !(mouse_click->isButtonDown())) {
            // printf("unpressed!\n");
            is_pressed = false;
            button->setUsual();
            // printf("ugh\n");
            // printf("%p\n", click_event_responce);
            return (*click_event_responce)();
        }
    } else if (is_pressed && !(mouse_click->isButtonDown())) {
        is_pressed = false;
        button->setUsual();
        return true;
    }
    return false;
}

// bool ButtonHandler::mhResponce(GUIMouseMove* mouse_move) {
//     Button* button = dynamic_cast<Button*>(my_window);
//     bool first_result = my_window->hitTest(mouse_move->getPrevPos());
//     bool second_result = my_window->hitTest(mouse_move->getNewPos());
//     if ((first_result || second_result)) {
//         if (!first_result && second_result) {
//             is_hovered = true;
//             button->setHover();
//             return true;
//             // return (*start_hover)(mouse_move->getNewPos());
//         } else if (first_result && !second_result) {
//             is_hovered = false;
//             if (!is_pressed) {
//                 button->setUsual();
//             }
//             return true;
//             // return (*end_hover)(mouse_move->getNewPos());
//         }
//     }
//     return false;
// }

// bool ButtonHandler::onEvent(GUIEvent* event) {
//     // Button* button = dynamic_cast<Button*>(my_window);
//     // printf("button got event\n");
//     switch (event->getType())
//     {
//         case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_BUTTON): {
//             GUILeftMouseButton* mouse_click = dynamic_cast<GUILeftMouseButton*> (event);
//             // printf("btn!\n");
//             return mbResponce(mouse_click);
//         }

//         case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_MOVE) : {
//             // GUIMouseMove* mouse_move = dynamic_cast<GUIMouseMove*>(event);
//             // return mhResponce(mouse_move);
//             return spreadEvent(event);
//         }
//         default: {
//             return spreadEvent(event);
//         }
//     }
// }

ButtonHandler::~ButtonHandler() {
    delete click_event_responce;
}

ListElementHandler::ListElementHandler(AbstractWindow* window, Functor<>* click_functor) : EventHandler(window), click_event_responce(click_functor) {
    
}

bool ListElementHandler::MBLResponce(GUILeftMouseButton* mouse_click) {
    Button* button = dynamic_cast<Button*>(my_window);
    if (my_window->hitTest(mouse_click->getPos())) {
        // printf("btn hittest\n");
        if (!is_pressed && mouse_click->isButtonDown()) {
            is_pressed = true;
            // printf("setted pressed\n");
            return true;
        } else if (is_pressed && !(mouse_click->isButtonDown())) {
            // printf("unpressed\n");
            GUIListElementChanged list_event;
            button->getParent()->onEvent(&list_event);
            is_pressed = false;
            is_chosen = true;
            button->setPressed();
            // button->setUsual();
            return (*click_event_responce)();
        }
    } else if (is_pressed && !(mouse_click->isButtonDown())) {
        is_pressed = false;
        button->setUsual();
        return true;
    }
    return false;
}

bool ListElementHandler::LECResponce(GUIListElementChanged* list_event) {
    is_pressed = false;
    is_chosen = false;
    dynamic_cast<Button*>(my_window)->setUsual();
    return true;
}


// bool ListElementHandler::onEvent(GUIEvent* event) {
//     switch (event->getType())
//     {
//         case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_BUTTON): {
//             GUILeftMouseButton* mouse_click = dynamic_cast<GUILeftMouseButton*> (event);
//             return mbResponce(mouse_click);
//         }

//         case static_cast<int>(GUIEvent::GUIEventTypes::LIST_ELEMENT_CHANGED) : {
//             is_pressed = false;
//             is_chosen = false;
//             dynamic_cast<Button*>(my_window)->setUsual();
//             return true;
//         }
//         default: {
//             return spreadEvent(event);
//         }
//     }
// }

ListElementHandler::~ListElementHandler() {
    delete click_event_responce;
}

// bool MouseCapturingHandler::onEvent(GUIEvent* event) {
//     switch (event->getType())
//     {
//         case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_BUTTON): {
//             // printf("mosue capturer got event, pressed is %d\n", is_pressed);
//             if (!is_pressed) {
//                 if (!passEvent(event)) {
//                     // printf("passed but withous success\n");
//                     GUILeftMouseButton* mouse_click = dynamic_cast<GUILeftMouseButton*> (event);
//                     bool res = mbResponce(mouse_click);
//                     // printf("pressed is %d, window %p, handler %p\n", is_pressed, my_window, this);
//                     // printf("%d %d\n", is_pressed, this->is_pressed);
//                     return res;
//                 } else {
//                     // printf("passed okay!\n");
//                 }
//                 return true;
//             } else {
//                 GUILeftMouseButton* mouse_click = dynamic_cast<GUILeftMouseButton*> (event);
//                 if (mbResponce(mouse_click)) {
//                     return true;
//                 }
//                 return passEvent(event);
//             }
//         }
        
//         case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_MOVE): {
//             // printf("mosue capturer move, pressed is %d, window %p, handler %p\n", is_pressed, my_window, this);
//             if (is_pressed) {
//                 GUIMouseMove* mouse_move = dynamic_cast<GUIMouseMove*>(event);
//                 return mmResponce(mouse_move);
//             }
//             return spreadEvent(event);
//         }
    
//         default:
//             return spreadEvent(event);
//             break;
//     }
// }

// MouseCapturingHandler::MouseCapturingHandler(AbstractWindow* window) : EventHandler(window) {}

// MouseCapturingHandler::~MouseCapturingHandler() {

// }

MovingHandler::MovingHandler(AbstractWindow* window, Functor<const Vector2&>* move_functor) : EventHandler(window), window_mover(move_functor) {

}

bool MovingHandler::MBLResponce(GUILeftMouseButton* mouse_click) {
    // printf("moving handler got mouseclick\n");
    if (my_window->hitTest(mouse_click->getPos())) {
        if (!is_pressed && mouse_click->isButtonDown()) {
            is_pressed = true;
            // printf("titlebar captured!\n");
            return true;
        } else if (is_pressed && !(mouse_click->isButtonDown())) {
            is_pressed = false;
            // printf("titlebar released!\n");
            return true;
        }
    } else if (is_pressed && !(mouse_click->isButtonDown())) {
        is_pressed = false;
        return true;
    }
    return false;
}

bool MovingHandler::MMResponce(GUIMouseMove* mouse_move) {
    if (is_pressed) {
        Vector2 delta = mouse_move->getNewPos() - mouse_move->getPrevPos();
        return (*window_mover)(delta);
    }
    return false;
}


// bool MovingHandler::onEvent(GUIEvent* event) {
//     // printf("titlebar got event!\n");
//     switch (event->getType())
//     {
//         case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_BUTTON): {
//             if (!passEvent(event)) {
//                 GUIMouseClickEvent* mouse_click = dynamic_cast<GUIMouseClickEvent*> (event);
//                 return mbResponce(mouse_click);
//             }
//             return true;
//         }

//         case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_MOVE): {
//             if (is_holding) {
//                 GUIMouseMove* mouse_move = dynamic_cast<GUIMouseMove*>(event);
//                 Vector2 delta = mouse_move->getNewPos() - mouse_move->getPrevPos();
//                 return (*window_mover)(delta);
//             }
//             return spreadEvent(event);
//         }

//         default:
//             return spreadEvent(event);
//             break;
//     }
// }

MovingHandler::~MovingHandler() {
    delete window_mover;
}

CanvasHandler::CanvasHandler(AbstractWindow* window, Renderer* renderer, Functor<Renderer*, Texture*, const Vector2&>* canvas_drawer) :
    EventHandler(window), renderer(renderer), canvas_drawer(canvas_drawer), current_instrument(nullptr) {

}

bool CanvasHandler::MBLResponce(GUILeftMouseButton* mouse_click) {
    // printf("canvas handler got mouseclick\n");
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

bool CanvasHandler::MMResponce(GUIMouseMove* mouse_move) {
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

// bool CanvasHandler::onEvent(GUIEvent* event) {
//     // printf("canvas handler got an event!\n");
//     switch (event->getType())
//     {
//         case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_BUTTON): {
//             // no pass event
//             GUIMouseClickEvent* mouse_click = dynamic_cast<GUIMouseClickEvent*> (event);
//             return mbResponce(mouse_click);
//         }
        
//         case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_MOVE): {
//             if (is_pressed) {
//                 GUIMouseMove* mouse_move = dynamic_cast<GUIMouseMove*>(event);
//                 return mmResponce(mouse_move);
//             }
//             return spreadEvent(event);
//         }
    
//         default:
//             return spreadEvent(event);
//             break;
//     }
// }

CanvasHandler::~CanvasHandler() {
    delete canvas_drawer;
}

// CanvasViewportHandler::CanvasViewportHandler(AbstractWindow* window, Renderer* renderer, Functor<Renderer*, Texture*, const Vector2&>* canvas_drawer) :
//     CanvasHandler(window, renderer, canvas_drawer) {}

// bool CanvasViewportHandler::mbResponce(GUIMouseClickEvent* mouse_click) {
//     if (my_window->hitTest(mouse_click->getPos())) {
//         // printf("ye, canvas hittest!\n");
//         if (!is_pressed && (mouse_click->isButtonDown())) {
//             is_pressed = true;
//             Vector2 mouseclick_pos = mouse_click->getPos();
//             (*canvas_drawer)(renderer, my_window->getSkin()->getTexture(), mouseclick_pos - my_window->getPos());
//             return true;
//         } else if (is_pressed && !(mouse_click->isButtonDown())) {
//             is_pressed = false;
//         }
//     }
//     return false;
// }

SliderHandler::SliderHandler(Functor<float, float>* functor, const Vector2& axis, float min_val, float max_val) :
    move_functor(functor), axis(axis), min_value(min_val), max_value(max_val), current_value(min_value) {
}

bool SliderHandler::MBLResponce(GUILeftMouseButton* mouse_click) {
    // printf("slider handler got mouseclick\n");
    if (my_window->hitTest(mouse_click->getPos())) {
        // printf("hittest is okay! %d %d\n", (!is_pressed), mouse_click->isButtonDown());
        if (!is_pressed && mouse_click->isButtonDown()) {
            is_pressed = true;
            // printf("mb captured! pressed of window %p is true %d, handler %p\n", my_window, is_pressed, this);
            // printf("%d %d\n", is_pressed, this->is_pressed);
            return true;
        } else if (is_pressed && !(mouse_click->isButtonDown())) {
            is_pressed = false;
            // printf("titlebar released!\n");
            return true;
        }
    }
    else if (is_pressed && !(mouse_click->isButtonDown())) {
        is_pressed = false;
        return true;
    }
    // printf("but hittest is  not okay\n");
    return false;
}

bool SliderHandler::MMResponce(GUIMouseMove* mouse_move) {
    if (is_pressed) {
        // printf("slider got mouse movement\n");
        bool first_result = my_window->hitTest(mouse_move->getPrevPos());
        bool second_result = my_window->hitTest(mouse_move->getNewPos());
        // printf("hittest results are %d %d\n", first_result, second_result);
        if (true) {
            Vector2 mouse_prev_pos = mouse_move->getPrevPos();
            Vector2 mouse_pos = mouse_move->getNewPos();
            Vector2 mouse_delta = mouse_pos - mouse_prev_pos;
            // printf("mouse delta: %f %f\n", mouse_delta.getX(), mouse_delta.getY());
            // printf("axis: %f %f\n", axis.getX(), axis.getY());
            // Vector2 slider_delta = axis.getProjection(mouse_delta);
            Vector2 slider_delta = mouse_delta.getProjection(axis);
            // printf("slider delta: %f %f\n", slider_delta.getX(), slider_delta.getY());
            if (fabs(slider_delta.getX()) < EPS && fabs(slider_delta.getY()) < EPS) {
                return false;
            }
            float value_delta = (slider_delta.getLength() / axis.getLength()) * (max_value - min_value);
            value_delta *= (slider_delta * axis) / slider_delta.getLength() / axis.getLength();
            if (value_delta == 0) {
                return false;
            }
            // printf("value delta:%f\n", value_delta);
            float new_value = Min(max_value, Max(min_value, current_value + value_delta));
            // printf("new value:%f\n", new_value);
            // printf("old value:%f\n", current_value);
            float true_delta = new_value - current_value;
            // printf("true delta:%f\n", true_delta);
            Vector2 true_slider_delta = axis * true_delta;
            my_window->move(true_slider_delta);
            (*move_functor)(current_value, new_value);
            current_value = new_value;
            return true;
        } else {
            is_pressed = false;
        }
    }
    return false;
}

// bool SliderHandler::onEvent(GUIEvent* event) {
//     switch (event->getType())
//     {
//         case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_BUTTON): {
//             GUIMouseClickEvent* mouse_click = dynamic_cast<GUIMouseClickEvent*> (event);
//             return mbResponce(mouse_click);
//         }

//         case static_cast<int>(GUIEvent::GUIEventTypes::MOUSE_MOVE): {
//             if (is_pressed) {
//                 GUIMouseMove* mouse_move = dynamic_cast<GUIMouseMove*>(event);
//                 return mmResponce(mouse_move);
//             }
//             return spreadEvent(event);
//         }
//         default:
//             break;
//     }
// }

SliderHandler::~SliderHandler() {
    delete move_functor;
}