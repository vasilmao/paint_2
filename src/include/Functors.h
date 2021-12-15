#ifndef INCLUDE_FUNCTORS
#define INCLUDE_FUNCTORS

#include "Window.h"
#include "Instruments.h"

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

class CreateRayCasterFunctor : public Functor<> {
private:
    AbstractWindow* parent;
    Renderer* renderer;
    Vector2 pos;
    Vector2 size;
public:
    CreateRayCasterFunctor(AbstractWindow* parent, Renderer* renderer, const Vector2& pos, const Vector2& size);
    virtual bool operator()();
};

class CreateViewportCanvasWindowFunctor : public Functor<> {
private:
    AbstractWindow* parent;
    Renderer* renderer;
    Vector2 pos;
    Vector2 size;
public:
    CreateViewportCanvasWindowFunctor(AbstractWindow* parent, Renderer* renderer, const Vector2& pos, const Vector2& size);
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

class CanvasViewportDrawerFunctor : public Functor<Renderer*, Texture*, const Vector2&> {
private:
    // AbstractInstrument* current_instrument;
    ViewportSkin* skin;
public:
    CanvasViewportDrawerFunctor(ViewportSkin* skin) : skin(skin) {
        // current_instrument = nullptr;
    }
    virtual bool operator()(Renderer* renderer, Texture* texture, const Vector2& pos);
};

class InstrumentPickerFunctor : public Functor<> {
private:
    AbstractInstrument* current_instrument;
public:
    InstrumentPickerFunctor(AbstractInstrument* its_instrument);
    virtual bool operator()();
};

class MoveViewportFunctor : public Functor<float, float> {
private:
    ViewportSkin* skin_to_change;
    Vector2 move_dir;
public:
    MoveViewportFunctor(ViewportSkin* skin, const Vector2& move_dir) : skin_to_change(skin), move_dir(move_dir) {}
    virtual bool operator()(float last_val, float new_val) {
        // printf("movements! %f %f\n", last_val, new_val);
        Rect2f viewport = skin_to_change->getViewport();
        // printf("x is %f\n", viewport.x);
        Vector2 real_size = skin_to_change->getFullSize();
        // new val is percent to move actually
        float new_x = (real_size.getX() - viewport.width) * (new_val - last_val) * move_dir.getX();
        float new_y = (real_size.getY() - viewport.height) * (new_val - last_val) * move_dir.getY();
        viewport.x += new_x;
        viewport.y += new_y;
        // printf("new x is %f\n", viewport.x);
        skin_to_change->setViewPort(viewport);
        return true;
    }
};

class ChangeBrushRColorFunctor : public Functor<> {
private:
    Brush* brush;
public:
    ChangeBrushRColorFunctor(Brush* brush);
    virtual bool operator()() {
        return true;
    }
};

#endif