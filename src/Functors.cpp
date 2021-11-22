#include "Functors.h"

EmptyFunctor::EmptyFunctor() {}

bool EmptyFunctor::operator()() {
    return false;
}

CloseButtonFunctor::CloseButtonFunctor(AbstractWindow* to_close_window) : to_close_window(to_close_window) {
    
}

bool CloseButtonFunctor::operator()() {
    to_close_window->markToDelete();
    return true;
}

bool PrintFunctor::operator()() {
    printf("printf!\n");
    return true;
}

CreateCanvasWindowFunctor::CreateCanvasWindowFunctor(AbstractWindow* parent, Renderer* renderer, const Vector2& pos, const Vector2& size) :
    parent(parent), renderer(renderer), pos(pos), size(size) {
}

bool CreateCanvasWindowFunctor::operator()() {
    CanvasWindow* c_w = new CanvasWindow(renderer, pos, size);
    parent->attachWindow(c_w);
    return true;
}

WindowMoverFunctor::WindowMoverFunctor(AbstractWindow* to_move_window) : window_to_move(to_move_window) {

}

bool WindowMoverFunctor::operator()(const Vector2& dv) {
    window_to_move->move(dv);
    return true;
}

bool CanvasDrawerFunctor::operator()(Renderer* renderer, Texture* texture, const Vector2& pos) {
    // printf("")
    AbstractInstrument* instrument = InstrumentPanel::getInstance()->getCurrentInstrument();
    if (instrument != nullptr) {
        instrument->apply(renderer, texture, pos);
        return true;
    }
    // current_instrument->apply(renderer, texture, pos);
    return false;
}

InstrumentPickerFunctor::InstrumentPickerFunctor(AbstractInstrument* its_instrument) : current_instrument(its_instrument) {}

bool InstrumentPickerFunctor::operator()() {
    // printf("%p\n", InstrumentPanel::getInstance());
    // printf("%p\n", current_instrument);
    (InstrumentPanel::getInstance())->setInstrument(current_instrument);
    return true;
}