//----------Event Handlers----------
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

class MovingHandler : public EventHandler {
private:
    bool is_holding = false;
    Functor<const Vector2&>* window_mover;
    bool mbResponce(GUIMouseClickEvent* mouse_click);
    // bool mhResponce(GUIMouseMove* mouse_move);
public:
    MovingHandler(AbstractWindow* window, Functor<const Vector2&>* move_functor);
    virtual bool onEvent(GUIEvent* event);
    virtual ~MovingHandler();
};

class CanvasHandler : public EventHandler {
private:
    bool is_pressed = false;
    bool mbResponce(GUIMouseClickEvent* mouse_click);
    bool mmResponce(GUIMouseMove* mouse_move);
    Renderer* renderer;
    Functor<Renderer*, Texture*, const Vector2&>* canvas_drawer;
    AbstractInstrument* current_instrument;
public:
    virtual ~CanvasHandler();
    CanvasHandler(AbstractWindow* window, Renderer* renderer, Functor<Renderer*, Texture*, const Vector2&>* canvas_drawer);
    virtual bool onEvent(GUIEvent* event);
};