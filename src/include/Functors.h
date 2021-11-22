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

class EmptyFunctor : public Functor<> {
public:
    EmptyFunctor();
    virtual bool operator()();
};

class CloseFunctor : public Functor<> {
public:
    virtual bool operator()();
};

class CloseButtonFunctor : public Functor<> {
private:
    // Button* button;
    AbstractWindow* to_close_window;
public:
    CloseButtonFunctor(AbstractWindow* to_close_window);
    virtual bool operator()();
};

class PrintFunctor : public Functor<> {
public:
    virtual bool operator()();
};

class CreateCanvasWindowFunctor : public Functor<> {
private:
    AbstractWindow* parent;
    Renderer* renderer;
    Vector2 pos;
    Vector2 size;
public:
    CreateCanvasWindowFunctor(AbstractWindow* parent, Renderer* renderer, const Vector2& pos, const Vector2& size);
    virtual bool operator()();
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
    // AbstractInstrument* current_instrument;
public:
    CanvasDrawerFunctor() {
        // current_instrument = nullptr;
    }
    // CanvasDrawerFunctor(AbstractInstrument* instr) {
    //     current_instrument = instr;
    // }
    // void setInstrument(AbstractInstrument* new_instr) {
    //     current_instrument = new_instr;
    // }
    virtual bool operator()(Renderer* renderer, Texture* texture, const Vector2& pos);
};

class InstrumentPickerFunctor : public Functor<> {
private:
    AbstractInstrument* current_instrument;
public:
    InstrumentPickerFunctor(AbstractInstrument* its_instrument);
    virtual bool operator()();
};

#endif