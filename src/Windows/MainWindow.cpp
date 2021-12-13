#include "Window.h"
#include "Handlers.h"

const Color titlebar_color = {185, 226, 235, 255};

const Vector2 default_canvas_pos  = {40, 40};
const Vector2 default_canvas_size = {500, 300};

void MainWindow::createTitlebar(Renderer* renderer, const Vector2& tb_pos, const Vector2& tb_size) {
    Texture* titlebar_texture = new Texture(renderer, tb_size, titlebar_color);
    Skin* titlebar_skin = new Skin(titlebar_texture, tb_size);
    // WindowMoverFunctor* move_f = new WindowMoverFunctor(this);
    // EventHandler* titlebar_handler = new MovingHandler(nullptr, move_f);
    EventHandler* titlebar_handler = new EventHandler(nullptr);
    TitleBar* titlebar = new TitleBar(tb_pos, tb_size, titlebar_handler, nullptr, titlebar_skin);
    // printf("canvas titlebar: %p\n", titlebar);
    titlebar_handler->setWindow(titlebar);
    attachWindow(titlebar);
    createCloseButton(renderer, {tb_pos.getX() + tb_size.getX() - close_button_size.getX(), tb_pos.getY()}, close_button_size, titlebar);
    Vector2 buttons_size(300, titlebar_height);
    createSignedButton(renderer, {0, 0},                       buttons_size, titlebar, "Canvas", new CreateCanvasWindowFunctor(this, renderer, default_canvas_pos, default_canvas_size));
    createSignedButton(renderer, {buttons_size.getX(), 0},     buttons_size, titlebar, "Sphere", new CreateRayCasterFunctor   (this, renderer, default_canvas_pos, {430, 430}));
    createSignedButton(renderer, {buttons_size.getX() * 2, 0}, buttons_size, titlebar, "Viewport Canvas", new CreateViewportCanvasWindowFunctor(this, renderer, default_canvas_pos, default_canvas_size));
}

void MainWindow::createCloseButton(Renderer* renderer, const Vector2& btn_pos, const Vector2& btn_size, AbstractWindow* titlebar) {
    Texture* close_button_texture = new Texture(renderer, "skins/close.bmp");
    Texture* close_button_texture_pressed = new Texture(renderer, "skins/close2.bmp");
    ButtonSkin* close_button_skin = new ButtonSkin(
        close_button_texture,
        nullptr,
        close_button_texture_pressed
    );
    close_button_skin->resize(close_button_size);
    ButtonHandler* close_button_handler = new ButtonHandler(nullptr, new CloseButtonFunctor(this));
    Button* close_button = new Button(
        btn_pos,
        btn_size,
        close_button_handler,
        this,
        close_button_skin
    );
    // printf("canvas button: %p\n", close_button);
    close_button_handler->setWindow(close_button);
    titlebar->attachWindow(close_button);
}

void MainWindow::createSignedButton(Renderer* renderer, const Vector2& btn_pos, const Vector2& btn_size, AbstractWindow* titlebar, const char* text, Functor<>* butn_functor) {
    Texture* button_texture = new Texture(renderer, btn_size, {0, 0, 0, 0});
    renderer->setTarget(button_texture);
    // renderer->drawText(btn_pos, text, {0, 0, 0, 255});
    renderer->drawTextCentered(btn_pos, btn_size, text, {0, 0, 0, 255});
    Texture* button_texture_pressed = new Texture(renderer, btn_size, {237, 104, 64, 255});
    renderer->setTarget(button_texture_pressed);
    renderer->drawTextCentered(btn_pos, btn_size, text, {0, 0, 0, 255});
    ButtonSkin* button_skin = new ButtonSkin(
        button_texture,
        nullptr,
        button_texture_pressed
    );
    ButtonHandler* button_handler = new ButtonHandler(nullptr, butn_functor);
    Button* btn = new Button(
        btn_pos,
        btn_size,
        button_handler,
        this,
        button_skin
    );
    button_handler->setWindow(btn);
    titlebar->attachWindow(btn);
}

MainWindow::MainWindow(Renderer* renderer, const Vector2& size) : AbstractWindow({0, 0}, size, new EventHandler(this), nullptr, nullptr) {
    // printf("main window %p\n", this);
    skin = new Skin(new Texture(renderer, "skins/wallpaper.bmp"), size);
    absolute_pos = {0, 0};
    Vector2 titlebar_size(size.getX(), close_button_size.getY());
    createTitlebar(renderer, absolute_pos, titlebar_size);
}