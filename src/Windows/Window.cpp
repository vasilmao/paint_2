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


