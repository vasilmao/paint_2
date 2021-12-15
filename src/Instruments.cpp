#include "Instruments.h"
#include "Window.h"
#include "Handlers.h"

AbstractWindow* Brush::createPrefPanel(Renderer* renderer, const Vector2& pos, AbstractWindow* parent) {
    
    SliderBody* r_slider = new SliderBody(renderer, pos + Vector2{10, 10}, Vector2{10, 100}, nullptr);
    return nullptr;
}

AbstractWindow* Eraser::createPrefPanel(Renderer* renderer, const Vector2& pos, AbstractWindow* parent) {
    return nullptr;
}