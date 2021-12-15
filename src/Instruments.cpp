#include "Instruments.h"
#include "Window.h"
#include "Handlers.h"

AbstractWindow* Brush::createPrefPanel(Renderer* renderer, const Vector2& pos, AbstractWindow* parent) {
    printf("creating panel!\n");
    SliderBody* r_slider = new SliderBody(renderer, pos + Vector2{10, 10}, Vector2{10, 100},
                                          new ChangeBrushRColorFunctor(this), 0, 255, color.r);
    SliderBody* g_slider = new SliderBody(renderer, pos + Vector2{30, 10}, Vector2{10, 100},
                                          new ChangeBrushGColorFunctor(this), 0, 255, color.g);
    SliderBody* b_slider = new SliderBody(renderer, pos + Vector2{50, 10}, Vector2{10, 100},
                                          new ChangeBrushBColorFunctor(this), 0, 255, color.b);
    SliderBody* radius_slider = new SliderBody(renderer, pos + Vector2{10, 120}, Vector2{100, 10},
                                               new ChangeBrushRFunctor(this), 1, 20, r);
    printf("sliders done\n");
    AbstractWindow* stupid_window = new AbstractWindow(
        pos,
        Vector2{120, 140},
        nullptr,
        parent,
        new Skin(new Texture(renderer, Vector2{120, 140}, Color{255, 255, 255, 255})));
    printf("window created\n");
    stupid_window->setHandler(new SensitiveHandler(stupid_window));
    printf("handler set\n");
    stupid_window->attachWindow(r_slider);
    stupid_window->attachWindow(g_slider);
    stupid_window->attachWindow(b_slider);
    stupid_window->attachWindow(radius_slider);
    printf("sliders attached\n");
    // InstrumentPanel::getInstance()->getParent()->attachWindow(stupid_window);
    // printf("window attached\n");
    printf("STUPID WINDOW IS %p\n", stupid_window);
    return stupid_window;
}

AbstractWindow* Eraser::createPrefPanel(Renderer* renderer, const Vector2& pos, AbstractWindow* parent) {
    return nullptr;
}