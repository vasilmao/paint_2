//----------Event Handlers----------
#include "GUIEvents.h"
#include "Window.h"
#include "Functors.h"

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
    bool is_pressed;
    bool is_hovered;
    Functor<>* click_event_responce;
public:
    ButtonHandler(AbstractWindow* window, Functor<>* click_functor);
    virtual bool onEvent(GUIEvent* event);
    virtual ~ButtonHandler();
    // void setHoverResponce(ButtonHoverFunctor* hover_start, ButtonHoverFunctor* hover_end);
};

class TitlebarHandler : public EventHandler {
private:
    bool is_holding = false;
    Functor<const Vector2&>* window_mover;
public:
    TitlebarHandler(AbstractWindow* window, Functor<const Vector2&>* move_functor);
    virtual bool onEvent(GUIEvent* event);
    virtual ~TitlebarHandler();
};