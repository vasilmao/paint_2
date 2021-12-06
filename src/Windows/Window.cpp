#include "Window.h"
#include "Handlers.h"

// const char MAIN_WINDOW_NAME[] = "main window";
// const char MAIN_WINDOW_TITLE_NAME[] = "main window TITLE";
// const char BUTTON_NAME[] = "button";

const Color titlebar_color = {185, 226, 235, 255};

TitleBar::TitleBar(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin) : 
    AbstractWindow(abs_pos, size, handler, parent, skin) {
    // printf("main window titlebar %p\n", this);
}

Button::Button(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin) : 
    AbstractWindow(abs_pos, size, handler, parent, skin) {
    // printf("button %p\n", this);
}

void Button::setHover() {
    dynamic_cast<ButtonSkin*>(skin)->setHover();
}

void Button::setPressed() {
    dynamic_cast<ButtonSkin*>(skin)->setPressed();
}

void Button::setUsual() {
    dynamic_cast<ButtonSkin*>(skin)->setUsual();
}


Canvas::Canvas(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin) :
    AbstractWindow(abs_pos, size, handler, parent, skin) {}


SliderBody::SliderBody(Renderer* renderer, const Vector2& pos, const Vector2& size, Functor<float, float>* slider_reaction) :
    AbstractWindow(pos, size, new EventHandler(this), nullptr, nullptr) {
    // create texture for self, for slider, handler for slider
    // 1
    Texture* my_texture = new Texture(renderer, size, {0, 0, 0, 100});
    skin = new Skin(my_texture, size);
    // 2
    Vector2 slider_axis(0, 0);
    Texture* slider_texture = new Texture(renderer, {20, 20}, {255, 168, 242, 255});
    Texture* slider_texture_pressed = new Texture(renderer, {20, 20}, {176, 247, 234, 255});
    float slider_len = 0;
    if (size.getX() > size.getY()) {
        slider_len = size.getY();
        slider_axis = {size.getX() - slider_len, 0};
    } else {
        slider_len = size.getX();
        slider_axis = {0, size.getY() - slider_len};
    }
    Vector2 slider_size(slider_len, slider_len);
    Skin* slider_skin = new ButtonSkin(slider_texture, nullptr, slider_texture_pressed);
    // 3
    EventHandler* slider_handler = new SliderHandler(slider_reaction, slider_axis);
    // the slider
    Slider* slider = new Slider(pos, slider_size, slider_handler, this, slider_skin);
    slider_handler->setWindow(slider);
    attachWindow(slider);
}

Slider::Slider(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin) :
    AbstractWindow(abs_pos, size, handler, parent, skin) {}