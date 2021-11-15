#ifndef INCLUDE_FUNCTORS
#define INCLUDE_FUNCTORS

#include "Window.h"

template<class ...Args>
class Functor {
public:
    Functor() = default;
    virtual bool operator()(Args...) = 0;
    virtual ~Functor() {}
};

class CloseFunctor : public Functor<> {
    virtual bool operator()();
};

// class FunctorEventSpreader : public Functor<> {
// public:
//     virtual bool operator()(GUIEvent* event) {
//         bool responce = false;
//         for (List<AbstractWindow*>::Iterator it = caller_window->getChildren()->begin(); it.isValid(); ++it) {
//             responce |= it.getData()->onEvent(event);
//         }
//         return responce;
//     }
// };

// class FunctorEventPasser : public Functor<> {
// public:
//     virtual bool operator()(GUIEvent* event) {
//         for (List<AbstractWindow*>::Iterator it = caller_window->getChildren()->begin(); it.isValid(); ++it) {
//             if (it.getData()->onEvent(event)) {
//                 return true;
//             }
//         }
//         return false;
//     }
// };

class CloseButtonFunctor : public Functor<> {
private:
    // Button* button;
    AbstractWindow* to_close_window;
public:
    CloseButtonFunctor(AbstractWindow* to_close_window);
    virtual bool operator()();
};

// class ButtonHoverFunctor : public Functor<const Vector2&> {
// public:
//     virtual bool operator()(const Vector2& mouse_pos);
// };

class PrintFunctor : public Functor<> {
public:
    virtual bool operator()();
};

class CreateCanvasWindowFunctor : public Functor<const Vector2&, const Vector2&> {
private:
    Renderer* renderer;
    AbstractWindow* parent;
public:
    CreateCanvasWindowFunctor(AbstractWindow* parent, Renderer* renderer);
    virtual bool operator()(const Vector2& pos, const Vector2& size);
};

class WindowMoverFunctor : public Functor<const Vector2&> {
private:
    AbstractWindow* window_to_move;
public:
    WindowMoverFunctor(AbstractWindow* to_move_window);
    virtual bool operator()(const Vector2& dv);
};

class CanvasDrawerFunctor : public Functor<Renderer*, Texture*, const Vector2&> {
private:
    AbstractInstrument* current_instrument;
public:
    CanvasDrawerFunctor() {
        current_instrument = nullptr;
    }
    CanvasDrawerFunctor(AbstractInstrument* instr) {
        current_instrument = instr;
    }
    void setInstrument(AbstractInstrument* new_instr) {
        current_instrument = new_instr;
    }
    virtual bool operator()(Renderer* renderer, Texture* texture, const Vector2& pos);
};

#endif