#include "Window.h"
#include "Handlers.h"

const Color titlebar_color = {185, 226, 235, 255};

CanvasWindow::CanvasWindow(Renderer* renderer, const Vector2& pos, const Vector2& size) : AbstractWindow(pos, size, new EventHandler(this), nullptr, nullptr) {
    // BRUH, divide into some functions
    absolute_pos = pos;
    this->size = size; 
    // create titlebar, close button, canvas
    skin = new Skin(new Texture(renderer, size + Vector2(2, 2), {20, 20, 20, 255}), size + Vector2(2, 2));
    Vector2 titlebar_size(size.getX(), titlebar_height);
    createTitlebar(renderer, absolute_pos, titlebar_size);
    Vector2 canvas_pos(absolute_pos.getX(), absolute_pos.getY() + titlebar_size.getY());
    Vector2 canvas_size(size.getX(), size.getY() - titlebar_size.getY());
    createCanvas(renderer, canvas_pos, canvas_size);

    absolute_pos -= Vector2(1, 1);
    this->size += Vector2(2, 2);
}

void CanvasWindow::createTitlebar(Renderer* renderer, const Vector2& tb_pos, const Vector2& tb_size) {
    Texture* titlebar_texture = new Texture(renderer, tb_size, titlebar_color);
    renderer->setTarget(titlebar_texture);
    // Vector2 text_size(tb_size.getY() * (sizeof("Canvas") - 1) / 3, tb_size.getY() / 1.5);
    // renderer->drawText({0, 0}, text_size, "Canvas", {255, 255, 255, 255});
    // renderer->drawText({tb_size.getX() / 2, 0}, "Canvas", {255, 255, 255, 255});
    renderer->drawTextCentered({0, 0}, tb_size, "Canvas", {0, 0, 0, 255});
    renderer->setTarget(NULL);
    Skin* titlebar_skin = new Skin(titlebar_texture, tb_size);
    WindowMoverFunctor* move_f = new WindowMoverFunctor(this);
    EventHandler* titlebar_handler = new MovingHandler(nullptr, move_f);
    TitleBar* titlebar = new TitleBar(tb_pos, tb_size, titlebar_handler, nullptr, titlebar_skin);
    // printf("canvas titlebar: %p\n", titlebar);
    titlebar_handler->setWindow(titlebar);
    attachWindow(titlebar);
    createCloseButton(renderer, {tb_pos.getX() + tb_size.getX() - close_button_size.getX(), tb_pos.getY()}, close_button_size, titlebar);
}

void CanvasWindow::createCloseButton(Renderer* renderer, const Vector2& btn_pos, const Vector2& btn_size, AbstractWindow* titlebar) {
    Texture* close_button_texture = new Texture(renderer, "skins/close.bmp");
    Texture* close_button_texture_pressed = new Texture(renderer, "skins/close2.bmp");
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

void CanvasWindow::createCanvas(Renderer* renderer, const Vector2& canvas_pos, const Vector2& canvas_size) {
    Skin* canvas_skin = new Skin(new Texture(renderer, canvas_size, {255, 255, 255, 255}), canvas_size);
    CanvasDrawerFunctor* canvas_drawer = new CanvasDrawerFunctor();
    CanvasHandler* canvas_handler = new CanvasHandler(nullptr, renderer, canvas_drawer);

    Canvas* canvas = new Canvas(canvas_pos, canvas_size, canvas_handler, this, canvas_skin);
    canvas_handler->setWindow(canvas);
    attachWindow(canvas);
}