#include "Functors.h"

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

CreateCanvasWindowFunctor::CreateCanvasWindowFunctor(AbstractWindow* parent, Renderer* renderer) : parent(parent), renderer(renderer) {

}

bool CreateCanvasWindowFunctor::operator()(const Vector2& pos, const Vector2& size) {
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
    current_instrument->apply(renderer, texture, pos);
    return true;
}