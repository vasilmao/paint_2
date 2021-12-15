#include "Instruments.h"
#include "Window.h"
#include "Handlers.h"

AbstractWindow* Brush::createPrefPanel(Renderer* renderer, const Vector2& pos, AbstractWindow* parent) {
    
    SliderBody* r_slider = new SliderBody(renderer, pos + Vector2{10, 10}, Vector2{10, 100},
                                          new ChangeBrushRColorFunctor(this), 0, 255);
    SliderBody* g_slider = new SliderBody(renderer, pos + Vector2{30, 10}, Vector2{10, 100},
                                          new ChangeBrushGColorFunctor(this), 0, 255);
    SliderBody* b_slider = new SliderBody(renderer, pos + Vector2{50, 10}, Vector2{10, 100},
                                          new ChangeBrushBColorFunctor(this), 0, 255);
    SliderBody* radius_slider = new SliderBody(renderer, pos + Vector2{10, 120}, Vector2{100, 10},
                                               new ChangeBrushRFunctor(this), 1, 20);
    AbstractWindow* stupid_window = new AbstractWindow(
        pos,
        Vector2{120, 140},
        nullptr,
        parent,
        new Skin(new Texture(renderer, Vector2{120, 140}, Color{0, 0, 0, 255})));
    stupid_window->setHandler(new SensitiveHandler(stupid_window));

    stupid_window->attachWindow(r_slider);
    stupid_window->attachWindow(g_slider);
    stupid_window->attachWindow(b_slider);
    stupid_window->attachWindow(radius_slider);
    InstrumentPanel::getInstance()->getParent()->attachWindow(stupid_window);
    return nullptr;
}

AbstractWindow* Eraser::createPrefPanel(Renderer* renderer, const Vector2& pos, AbstractWindow* parent) {
    return nullptr;
}