#include "Window.h"
#include "Handlers.h"

#include "RayCaster.h"

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
    printf("\t button %p\n", this);
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

void Button::setResponce(Functor<>* functor) {
    ButtonHandler* btn_handler = dynamic_cast<ButtonHandler*>(handler);
    btn_handler->setResponce(functor);
}

Canvas::Canvas(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin) :
    AbstractWindow(abs_pos, size, handler, parent, skin) {}

void Slider::setPressed() {
    dynamic_cast<ButtonSkin*>(skin)->setPressed();
}

void Slider::setUsual() {
    dynamic_cast<ButtonSkin*>(skin)->setUsual();
}

float Slider::getValue() {
    return dynamic_cast<SliderHandler*>(handler)->getValue();
}

SliderBody::SliderBody(Renderer* renderer, const Vector2& pos, const Vector2& size, Functor<float, float>* slider_reaction, float min_val, float max_val, float init_val) :
    AbstractWindow(pos, size, new EventHandler(this), nullptr, nullptr) {
    if (isnanf(init_val)) {
        init_val = min_val;
    }
    // create texture for self, for slider, handler for slider
    // 1
    // Texture* my_texture = new Texture(renderer, size, {0, 0, 0, 100});
    Texture* my_texture = new Texture(renderer, size, {0, 0, 0, 0});
    renderer->setTarget(my_texture);
    Color line_color = {0, 0, 0, 100};
    float delta = 0;
    if (size.getX() > size.getY()) {
        delta = size.getY() / 2;
        renderer->drawLine(Vector2{delta, delta}, Vector2{size.getX() - delta, delta}, line_color);
        renderer->drawFilledCircle(Vector2{delta, delta}, delta * 2 / 3, line_color);
        renderer->drawFilledCircle(Vector2{size.getX() - delta, delta}, delta * 2 / 3, line_color);
    } else {
        delta = size.getX() / 2;
        renderer->drawLine(Vector2{delta, delta}, Vector2{delta, size.getY() - delta}, line_color);
        renderer->drawFilledCircle(Vector2{delta, delta}, delta * 2 / 3, line_color);
        renderer->drawFilledCircle(Vector2{delta, size.getY() - delta}, delta * 2 / 3, line_color);
    }
    renderer->setTarget(nullptr);
    skin = new Skin(my_texture, size);
    // 2
    Vector2 slider_axis(0, 0);
    float slider_wh = Min(size.getX(), size.getY());
    // Texture* slider_texture = new Texture(renderer, {slider_wh, slider_wh}, {255, 168, 242, 255});
    Texture* slider_texture = new Texture(renderer, {slider_wh, slider_wh}, {0, 0, 0, 0});
    renderer->setTarget(slider_texture);
    renderer->drawFilledCircle(slider_texture->getSize() * 0.5, delta * 2 / 3, {255, 168, 242, 255});
    // Texture* slider_texture_pressed = new Texture(renderer, {slider_wh, slider_wh}, {176, 247, 234, 255});
    Texture* slider_texture_pressed = new Texture(renderer, {slider_wh, slider_wh}, {0, 0, 0, 0});
    renderer->setTarget(slider_texture_pressed);
    renderer->drawFilledCircle(slider_texture_pressed->getSize() * 0.5, delta * 2 / 3, {176, 247, 234, 255});
    float slider_len = 0;
    if (size.getX() > size.getY()) {
        slider_len = size.getY();
        slider_axis = {size.getX() - slider_len, 0};
    } else {
        slider_len = size.getX();
        slider_axis = {0, size.getY() - slider_len};
    }
    Vector2 slider_size(slider_len, slider_len);
    Skin* slider_skin = new CircleButtonSkin(slider_texture, nullptr, slider_texture_pressed);
    // 3
    EventHandler* slider_handler = new SliderHandler(slider_reaction, slider_axis, min_val, max_val, init_val);
    // the slider
    Slider* slider = new Slider(pos + (init_val - min_val) / (max_val - min_val) * slider_axis, slider_size, slider_handler, this, slider_skin);
    slider_handler->setWindow(slider);
    attachWindow(slider);
}

Slider::Slider(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin) :
    AbstractWindow(abs_pos, size, handler, parent, skin) {}



RayCasterHolder::RayCasterHolder(Renderer* renderer, const Vector2& abs_pos, const Vector2& sz, AbstractWindow* parent) :
AbstractWindow(abs_pos - Vector2(1, 1), sz + Vector2(2, 2), nullptr, parent, nullptr) {
    handler = new EventHandler(this);
    skin = new Skin(new Texture(renderer, size, Color{0, 0, 0, 255}));
    Vector2 titlebar_size(size.getX(), titlebar_height);
    createTitlebar(renderer, absolute_pos, titlebar_size);
    // now creating sphere

    Vector2 rc_size(size.getX(), size.getY() - titlebar_size.getY());
    Vector2 rc_pos(abs_pos.getX(), abs_pos.getY() + titlebar_size.getY());
    const Rect2f range_rect = {0, 0, rc_size.getX(), rc_size.getY()};
    Camera* camera = new Camera({0, 0, 150}, {0, 1, 0}, {range_rect.width, range_rect.height});
    Sphere* sphere = new Sphere({0, 0, 0}, 120, {1, 0, 0});
    Light* light_source = new Light({-1300, -300, 0}, {1, 1, 1});
    
    Texture* rc_texture = new Texture(renderer, Vector2{range_rect.width, range_rect.height}, {0, 0, 0, 255});
    Skin* rc_skin = new Skin(rc_texture);

    RayCasterHandler* rc_handler = new RayCasterHandler(renderer, camera, light_source, sphere);

    rc_handler->setTexture(rc_texture);

    printf("updating...\n");
    rc_handler->rotateLight(0);
    printf("updated!\n");
    // renderer->setTarget(rc_texture);
    // renderer->drawRect({0, 0}, {100, 100}, {255, 255, 255, 255});
    renderer->setTarget(nullptr);
    // renderer->copyTexture(rc_texture, Vector2{0, 0});
    // renderer->render();
    // printf("%f %f\n", rc_texture->getSize().getX(), rc_texture->getSize().getY());
    // SDL_Delay(3000);
    // exit(0);

    RayCaster* rc = new RayCaster(rc_pos, rc_size, rc_handler, this, rc_skin);
    rc_handler->setWindow(rc);
    attachWindow(rc);
}

void RayCasterHolder::createTitlebar(Renderer* renderer, const Vector2& tb_pos, const Vector2& tb_size) {
    // char str[50] = {};
    // strcpy(str, InstrumentPanel::getSkinsDir());
    // strcat(str, "titlebar.bmp");
    Texture* titlebar_texture = new Texture(renderer, "titlebar.bmp", true);
    // TextureManager::addTexture(titlebar_texture, "titlebar.bmp");
    // Texture* titlebar_texture = new Texture(renderer, tb_size, titlebar_color);
    // renderer->setTarget(titlebar_texture);
    // Vector2 text_size(tb_size.getY() * (sizeof("Canvas") - 1) / 3, tb_size.getY() / 1.5);
    // renderer->drawText({0, 0}, text_size, "Canvas", {255, 255, 255, 255});
    // renderer->drawText({tb_size.getX() / 2, 0}, "Canvas", {255, 255, 255, 255});
    // renderer->drawTextCentered({0, 0}, tb_size, "Sphere", {0, 0, 0, 255});
    renderer->setTarget(NULL);
    Skin* titlebar_skin = new Skin(titlebar_texture, tb_size);
    WindowMoverFunctor* move_f = new WindowMoverFunctor(this);
    EventHandler* titlebar_handler = new MovingHandler(nullptr, move_f);
    TitleBar* titlebar = new TitleBar(tb_pos, tb_size, titlebar_handler, nullptr, titlebar_skin);
    // printf("canvas titlebar: %p\n", titlebar);
    titlebar_handler->setWindow(titlebar);
    titlebar->attachWindow(new TextLabel(renderer, tb_pos, tb_size, titlebar, "Sphere"));
    attachWindow(titlebar);
    createCloseButton(renderer, {tb_pos.getX() + tb_size.getX() - close_button_size.getX(), tb_pos.getY()}, close_button_size, titlebar);
}

void RayCasterHolder::createCloseButton(Renderer* renderer, const Vector2& btn_pos, const Vector2& btn_size, AbstractWindow* titlebar) {
    // char str[50] = {};
    // strcpy(str, InstrumentPanel::getSkinsDir());
    // strcat(str, "close.bmp");
    Texture* close_button_texture = new Texture(renderer, "close.bmp", true);
    // TextureManager::addTexture(close_button_texture, "close.bmp");
    // memset(str, 0, 50);
    // strcpy(str, InstrumentPanel::getSkinsDir());
    // strcat(str, "close2.bmp");
    Texture* close_button_texture_pressed = new Texture(renderer, "close2.bmp", true);
    // TextureManager::addTexture(close_button_texture_pressed, "close2.bmp");
    // Texture* close_button_texture = new Texture(renderer, "skins/close.bmp");
    // Texture* close_button_texture_pressed = new Texture(renderer, "skins/close2.bmp");
    ButtonSkin* close_button_skin = new ButtonSkin(
        close_button_texture,
        nullptr,
        close_button_texture_pressed
    );
    ButtonHandler* close_button_handler = new ButtonHandler(nullptr, new CloseButtonFunctor(this));
    Button* close_button = new Button(
        btn_pos,
        btn_size,
        close_button_handler,
        this,
        close_button_skin
    );
    close_button_skin->resize(close_button_size);
    // printf("canvas button: %p\n", close_button);
    close_button_handler->setWindow(close_button);
    titlebar->attachWindow(close_button);
}

RayCaster::RayCaster(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin) :
AbstractWindow(abs_pos, size, handler, parent, skin) {}

// void RayCaster::soloRender(Renderer* renderer) {
//     dynamic_cast<RayCasterHandler*>(handler)->update();
//     renderer->setTarget(NULL);
//     skin->draw(renderer, absolute_pos);
// }

TextLabel::TextLabel(Renderer* renderer, const Vector2& pos, const Vector2& size, AbstractWindow* parent, const char* text) :
AbstractWindow(pos, size, new EventHandler(this), parent, nullptr) {
    Texture* texture = new Texture(renderer, size, {0, 0, 0, 0});
    renderer->setTarget(texture);
    renderer->drawTextCentered(pos, size, text, {0, 0, 0, 255});
    renderer->setTarget(nullptr);
    skin = new Skin(texture);
}