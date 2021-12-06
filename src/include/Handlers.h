//----------Event Handlers----------
#ifndef INCLUDE_HASNDLERS
#define INCLUDE_HASNDLERS

#include "GUIEvents.h"
#include "Window.h"
#include "Functors.h"
#include "Instruments.h"

class EventHandler {
protected:
    AbstractWindow* my_window;
    // FunctorEventSpreader* spreader;
    // FunctorEventPasser* passer;
    bool passEvent(GUIEvent* event);

    bool spreadEvent(GUIEvent* event);

public:
    EventHandler(AbstractWindow* window) : my_window(window){};
    virtual ~EventHandler();
    virtual bool onEvent(GUIEvent* event);
    virtual void setWindow(AbstractWindow* window);
};

class ButtonHandler : public EventHandler {
private:
    bool is_pressed = false;
    bool is_hovered = false;
    Functor<>* click_event_responce;
    bool mbResponce(GUIMouseClickEvent* mouse_click);
    bool mhResponce(GUIMouseMove* mouse_move);
public:
    ButtonHandler(AbstractWindow* window, Functor<>* click_functor);
    virtual bool onEvent(GUIEvent* event);
    virtual ~ButtonHandler();
    // void setHoverResponce(ButtonHoverFunctor* hover_start, ButtonHoverFunctor* hover_end);
};

class MouseCapturingHandler : public EventHandler {
protected:
    bool is_pressed = false;
public:
    MouseCapturingHandler() : EventHandler(nullptr) {};
    MouseCapturingHandler(AbstractWindow* window);
    virtual bool onEvent(GUIEvent* event);
    virtual bool mbResponce(GUIMouseClickEvent* mouse_click) = 0;
    virtual bool mmResponce(GUIMouseMove* mouse_move) = 0;
    virtual ~MouseCapturingHandler() = 0;
};

class MovingHandler : public MouseCapturingHandler {
private:
    bool is_holding = false;
    Functor<const Vector2&>* window_mover;
    virtual bool mbResponce(GUIMouseClickEvent* mouse_click);
    virtual bool mmResponce(GUIMouseMove* mouse_move);
public:
    MovingHandler(AbstractWindow* window, Functor<const Vector2&>* move_functor);
    // virtual bool onEvent(GUIEvent* event);
    virtual ~MovingHandler();
};

class CanvasHandler : public MouseCapturingHandler {
protected:
    // bool is_pressed = false;
    virtual bool mbResponce(GUIMouseClickEvent* mouse_click);
    virtual bool mmResponce(GUIMouseMove* mouse_move);
    Renderer* renderer;
    Functor<Renderer*, Texture*, const Vector2&>* canvas_drawer;
    AbstractInstrument* current_instrument;
public:
    virtual ~CanvasHandler();
    CanvasHandler(AbstractWindow* window, Renderer* renderer, Functor<Renderer*, Texture*, const Vector2&>* canvas_drawer);
    // virtual bool onEvent(GUIEvent* event);
};

// class CanvasViewportHandler : public CanvasHandler {
//     virtual bool mbResponce(GUIMouseClickEvent* mouse_click);
//     virtual bool mmResponce(GUIMouseMove* mouse_move);
// public:
//     virtual ~CanvasViewportHandler();
//     CanvasViewportHandler(AbstractWindow* window, Renderer* renderer, Functor<Renderer*, Texture*, const Vector2&>* canvas_drawer);
// }

class ListElementHandler : public EventHandler {
private:
    bool is_pressed = false;
    bool is_chosen = false;
    Functor<>* click_event_responce;
    bool mbResponce(GUIMouseClickEvent* mouse_click);
    bool mhResponce(GUIMouseMove* mouse_move);
public:
    ListElementHandler(AbstractWindow* window, Functor<>* click_functor);
    virtual bool onEvent(GUIEvent* event);
    virtual ~ListElementHandler();
};

class SliderHandler : public MouseCapturingHandler {
private:
    Functor<float, float>* move_functor;
    float min_value = 0;
    float max_value = 1;
    Vector2 axis;
    float current_value = 0;
    // bool is_pressed = false;
public:
    virtual bool mbResponce(GUIMouseClickEvent* mouse_click);
    virtual bool mmResponce(GUIMouseMove* mouse_move);
    SliderHandler(Functor<float, float>* functor, const Vector2& axis, float min_val = 0, float max_val = 1);
    // virtual bool onEvent(GUIEvent* event);
    virtual ~SliderHandler();
};
#endif