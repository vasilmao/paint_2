#include "Window.h"
#include "Handlers.h"

const char MAIN_WINDOW_NAME[] = "main window";
const char MAIN_WINDOW_TITLE_NAME[] = "main window TITLE";
const char BUTTON_NAME[] = "button";

const Color titlebar_color = {185, 226, 235, 255};

AbstractWindow::AbstractWindow(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin) : absolute_pos(abs_pos), size(size), handler(handler), parent(parent), skin(skin) {
    children = new List<AbstractWindow*>();
    // printf("window created %p\n", this);
}

List<AbstractWindow*>* AbstractWindow::getChildren() {
    return children;
}

AbstractWindow* AbstractWindow::getParent(){
    return parent;
}

void AbstractWindow::attachWindow(AbstractWindow* window) {
    children->pushFront(window);
}

bool AbstractWindow::hitTest(const Vector2& pos) const {
    return skin->hitTest(pos - absolute_pos);
}

AbstractWindow::~AbstractWindow() {
    for (List<AbstractWindow*>::Iterator it = children->begin(); it.isValid(); ++it) {
        delete it.getData();
        // children.erase(it);
    }
    delete children;
    delete skin;
    delete handler;
    free(title);
}

bool AbstractWindow::onEvent(GUIEvent* event) {
    // printf("yaaa yoy\n");
    if (handler) {
        // printf("eeee\n");
        return handler->onEvent(event);
    }
    return false;
}

void AbstractWindow::render(Renderer* renderer) {
    // printf("rendering %s (%p)\n", title, this);
    renderer->setTarget(NULL);
    // if (strlen(title) == sizeof(BUTTON_NAME) - 1)
    skin->draw(renderer, absolute_pos);
    // printf("drawing children\n");
    for (List<AbstractWindow*>::Iterator it = children->end(); it.isValid(); --it) {
        it.getData()->render(renderer);
    }
    // printf("my skin is %p\n", skin);
}

Skin* AbstractWindow::getSkin() {
    return skin;
}

void AbstractWindow::markToDelete() {
    on_delete = true;
}

bool AbstractWindow::isToDelete() {
    return on_delete;
}

void AbstractWindow::clean() {
    for (List<AbstractWindow*>::Iterator it = children->begin(); it != children->end(); ++it) {
        if (it.getData()->isToDelete()) {
            delete it.getData();
            children->erase(it);
        } else {
            it.getData()->clean();
        }
    }
    children->clear();
}

void AbstractWindow::move(const Vector2& dv) {
    absolute_pos += dv;
    // printf("moving: %p\n", this);
    for (List<AbstractWindow*>::Iterator it = children->begin(); it != children->cycleEnd(); ++it) {
        it.getData()->move(dv);
    }
}

const Vector2& AbstractWindow::getPos() const {
    return absolute_pos;
}

//----------MAIN WINDOW----------

void MainWindow::createTitlebar(Renderer* renderer, const Vector2& tb_pos, const Vector2& tb_size) {
    Texture* titlebar_texture = new Texture(renderer, tb_size, titlebar_color);
    Skin* titlebar_skin = new Skin(titlebar_texture, tb_size);
    WindowMoverFunctor* move_f = new WindowMoverFunctor(this);
    EventHandler* titlebar_handler = new TitlebarHandler(nullptr, move_f);
    TitleBar* titlebar = new TitleBar(tb_pos, tb_size, titlebar_handler, nullptr, titlebar_skin);
    printf("canvas titlebar: %p\n", titlebar);
    titlebar_handler->setWindow(titlebar);
    attachWindow(titlebar);
    createCloseButton(renderer, {tb_pos.getX() + tb_size.getX() - close_button_size.getX(), tb_pos.getY()}, close_button_size, titlebar);
}

void MainWindow::createCloseButton(Renderer* renderer, const Vector2& btn_pos, const Vector2& btn_size, AbstractWindow* titlebar) {
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
    printf("canvas button: %p\n", close_button);
    close_button_handler->setWindow(close_button);
    titlebar->attachWindow(close_button);
}

MainWindow::MainWindow(Renderer* renderer, const Vector2& size) : AbstractWindow({0, 0}, size, new EventHandler(this), nullptr, nullptr) {
    // BRUH, divide into some functions
    printf("main window %p\n", this);
    skin = new Skin(new Texture(renderer, "skins/wallpaper.bmp"), size);
    absolute_pos = {0, 0};
    Vector2 titlebar_size(size.getX(), close_button_size.getY());
    createTitlebar(renderer, absolute_pos, titlebar_size);

    // printf("close_button attached!\n");
    // Texture* close_button_texture = new Texture(renderer, "skins/close.bmp");
    // Texture* close_button_texture_pressed = new Texture(renderer, "skins/close2.bmp");
}


TitleBar::TitleBar(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin) : 
    AbstractWindow(abs_pos, size, handler, parent, skin) {
    printf("main window titlebar %p\n", this);
}

//----------BUTTON----------

Button::Button(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin) : 
    AbstractWindow(abs_pos, size, handler, parent, skin) {
    title = (char*)calloc(sizeof(BUTTON_NAME), sizeof(char));
    strcpy(title, BUTTON_NAME);
    printf("button %p\n", this);
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

//----------CANVAS WINDOW----------

CanvasWindow::CanvasWindow(Renderer* renderer, const Vector2& pos, const Vector2& size) : AbstractWindow(pos, size, new EventHandler(this), nullptr, nullptr) {
    // BRUH, divide into some functions
    absolute_pos = pos;
    this->size = size; 
    // create titlebar, close button, canvas
    skin = new Skin(new Texture(renderer, size + Vector2(2, 2), {20, 20, 20, 255}), size + Vector2(2, 2));
    Vector2 titlebar_size(size.getX(), close_button_size.getY());
    createTitlebar(renderer, absolute_pos, titlebar_size);
    Vector2 canvas_pos(absolute_pos.getX(), absolute_pos.getY() + titlebar_size.getY());
    Vector2 canvas_size(size.getX(), size.getY() - titlebar_size.getY());
    createCanvas(renderer, canvas_pos, canvas_size);

    absolute_pos -= Vector2(1, 1);
    this->size += Vector2(2, 2);
}

void CanvasWindow::createTitlebar(Renderer* renderer, const Vector2& tb_pos, const Vector2& tb_size) {
    Texture* titlebar_texture = new Texture(renderer, tb_size, titlebar_color);
    Skin* titlebar_skin = new Skin(titlebar_texture, tb_size);
    WindowMoverFunctor* move_f = new WindowMoverFunctor(this);
    EventHandler* titlebar_handler = new TitlebarHandler(nullptr, move_f);
    TitleBar* titlebar = new TitleBar(tb_pos, tb_size, titlebar_handler, nullptr, titlebar_skin);
    printf("canvas titlebar: %p\n", titlebar);
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
    printf("canvas button: %p\n", close_button);
    close_button_handler->setWindow(close_button);
    titlebar->attachWindow(close_button);
}

void CanvasWindow::createCanvas(Renderer* renderer, const Vector2& canvas_pos, const Vector2& canvas_size) {
    Skin* canvas_skin = new Skin(new Texture(renderer, canvas_size, {125, 125, 125, 255}), canvas_size);
    CanvasDrawerFunctor* canvas_drawer = new CanvasDrawerFunctor();
    CanvasHandler* canvas_handler = new CanvasHandler(nullptr, renderer, canvas_drawer);

    Canvas* canvas = new Canvas(canvas_pos, canvas_size, canvas_handler, this, canvas_skin);
    canvas_handler->setWindow(canvas);
    attachWindow(canvas);
}

Canvas::Canvas(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin) :
    AbstractWindow(abs_pos, size, handler, parent, skin) {}

// Canvas::setInstrument(AbstractInstrument* new_instrument) {

// }