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

MainWindow::MainWindow(Renderer* renderer, const Vector2& size) : AbstractWindow({0, 0}, size, new EventHandler(this), nullptr, nullptr) {
    printf("main window %p\n", this);
    // title is "main window";
    // title = (char*)calloc(sizeof(MAIN_WINDOW_NAME), sizeof(char));
    // strcpy(title, MAIN_WINDOW_NAME);
    skin = new Skin(new Texture(renderer, size, {0, 0, 0, 255}), size);
    // printf("yeah, main window skin is %p\n");
    // printf("lets just try to draw it\n");
    // skin->draw(renderer, absolute_pos);
    // we need to spawn childs
    // they are: main window title, file button, help button, close button
    absolute_pos = {0, 0};
    Texture* close_button_texture = new Texture(renderer, "skins/close.bmp");
    Vector2 button_size = close_button_texture->getSize();
    Vector2 titlebar_size(size.getX(), button_size.getY());
    printf("button   size: {%f, %f}\n", button_size.getX(), button_size.getY());
    printf("titlebar size: {%f, %f}\n", titlebar_size.getX(), titlebar_size.getY());
    // title
    Texture* titlebar_texture = new Texture(renderer, titlebar_size, titlebar_color);
    Skin* titlebar_skin = new Skin(titlebar_texture, titlebar_size);
    EventHandler* titlebar_handler = new EventHandler(nullptr);

    TitleBar* titlebar = new TitleBar(Vector2(0, 0), titlebar_size, titlebar_handler, nullptr, titlebar_skin);
    titlebar_handler->setWindow(titlebar);
    attachWindow(titlebar);
    printf("ya\n");
    printf("titlebar attached!\n");
    // close button
    Texture* close_button_texture_pressed = new Texture(renderer, "skins/close2.bmp");
    ButtonSkin* close_button_skin = new ButtonSkin(
        close_button_texture,
        nullptr,
        close_button_texture_pressed
    );
    // Vector2 button_size = close_button_texture->getSize();
    Vector2 button_pos(size.getX() - button_size.getX(), 0);
    ButtonHandler* close_button_handler = new ButtonHandler(nullptr, new CloseButtonFunctor(this));
    // ButtonHandler* close_button_handler = new ButtonHandler(nullptr, new PrintFunctor());
    Button* close_button = new Button(
        button_pos,
        button_size,
        close_button_handler,
        this,
        close_button_skin
    );
    close_button_handler->setWindow(close_button);
    titlebar->attachWindow(close_button);



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
    absolute_pos = pos;
    this->size = size; 
    // create titlebar, close button, canvas
    skin = new Skin(new Texture(renderer, size, {20, 20, 20, 255}), size);
    Texture* close_button_texture = new Texture(renderer, "skins/close.bmp");
    Texture* close_button_texture_pressed = new Texture(renderer, "skins/close2.bmp");
    Vector2 button_size = close_button_texture->getSize();
    Vector2 titlebar_size(size.getX(), button_size.getY());

    Texture* titlebar_texture = new Texture(renderer, titlebar_size, titlebar_color);
    Skin* titlebar_skin = new Skin(titlebar_texture, titlebar_size);
    WindowMoverFunctor* move_f = new WindowMoverFunctor(this);
    EventHandler* titlebar_handler = new TitlebarHandler(nullptr, move_f);

    TitleBar* titlebar = new TitleBar(absolute_pos, titlebar_size, titlebar_handler, nullptr, titlebar_skin);
    printf("canvas titlebar: %p\n", titlebar);
    titlebar_handler->setWindow(titlebar);
    attachWindow(titlebar);


    ButtonSkin* close_button_skin = new ButtonSkin(
        close_button_texture,
        nullptr,
        close_button_texture_pressed
    );
    // Vector2 button_size = close_button_texture->getSize();
    Vector2 button_pos(absolute_pos.getX() + size.getX() - button_size.getX(), absolute_pos.getY());
    ButtonHandler* close_button_handler = new ButtonHandler(nullptr, new CloseButtonFunctor(this));
    // ButtonHandler* close_button_handler = new ButtonHandler(nullptr, new PrintFunctor());
    Button* close_button = new Button(
        button_pos,
        button_size,
        close_button_handler,
        this,
        close_button_skin
    );
    printf("canvas button: %p\n", close_button);
    close_button_handler->setWindow(close_button);
    titlebar->attachWindow(close_button);

    Vector2 canvas_pos(absolute_pos.getX(), absolute_pos.getY() + titlebar_size.getY());
    Vector2 canvas_size(size.getX(), size.getY() - titlebar_size.getY());
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