#include "Window.h"
#include "Handlers.h"

const char MAIN_WINDOW_NAME[] = "main window";
const char MAIN_WINDOW_TITLE_NAME[] = "main window TITLE";
const char BUTTON_NAME[] = "button";

const Color titlebar_color = {185, 226, 235, 255};

const Vector2 default_canvas_pos  = {40, 40};
const Vector2 default_canvas_size = {500, 300};

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

void AbstractWindow::soloRender(Renderer* renderer) {
    renderer->setTarget(NULL);
    skin->draw(renderer, absolute_pos);
    // printf("drawed %p\n", this);
}

void AbstractWindow::renderChildren(Renderer* renderer) {
    for (List<AbstractWindow*>::Iterator it = children->end(); it.isValid(); --it) {
        it.getData()->render(renderer);
    }
}

void AbstractWindow::render(Renderer* renderer) {

    soloRender(renderer);
    renderChildren(renderer);
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
    EventHandler* titlebar_handler = new MovingHandler(nullptr, move_f);
    TitleBar* titlebar = new TitleBar(tb_pos, tb_size, titlebar_handler, nullptr, titlebar_skin);
    printf("canvas titlebar: %p\n", titlebar);
    titlebar_handler->setWindow(titlebar);
    attachWindow(titlebar);
    createCloseButton(renderer, {tb_pos.getX() + tb_size.getX() - close_button_size.getX(), tb_pos.getY()}, close_button_size, titlebar);
    Vector2 buttons_size(72, titlebar_height);
    createSignedButton(renderer, {0, 0}, buttons_size, titlebar, "Canvas", new CreateCanvasWindowFunctor(this, renderer, default_canvas_pos, default_canvas_size));
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
    printf("canvas button: %p\n", close_button);
    close_button_handler->setWindow(close_button);
    titlebar->attachWindow(close_button);
}

void MainWindow::createSignedButton(Renderer* renderer, const Vector2& btn_pos, const Vector2& btn_size, AbstractWindow* titlebar, const char* text, Functor<>* butn_functor) {
    Texture* button_texture = new Texture(renderer, btn_size, {0, 0, 0, 0});
    renderer->setTarget(button_texture);
    renderer->drawTextCentered(btn_pos, btn_size, text, {0, 0, 0, 255});
    Texture* button_texture_pressed = new Texture(renderer, btn_size, {237, 104, 64, 255});
    renderer->setTarget(button_texture);
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
    printf("main window %p\n", this);
    skin = new Skin(new Texture(renderer, "skins/wallpaper.bmp"), size);
    absolute_pos = {0, 0};
    Vector2 titlebar_size(size.getX(), close_button_size.getY());
    createTitlebar(renderer, absolute_pos, titlebar_size);
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
    close_button_skin->resize(close_button_size);
    printf("canvas button: %p\n", close_button);
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

Canvas::Canvas(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin) :
    AbstractWindow(abs_pos, size, handler, parent, skin) {}


//----------InstrumentPicker----------

// InstrumentPicker::InstrumentPicker(Renderer* renderer, const Vector2& abs_pos, const Vector2& size, AbstractWindow* parent) :
//     AbstractWindow(abs_pos, size, new EventHandler(this), parent, new Skin(new Texture(renderer, size, titlebar_color), size)) {
//     // create buttons for instruments
//     // npx
//     // npx - instr - npx - instr - npx
//     float n_px = (size.getX() - 2 * button_size.getX()) / 3;
//     // createInstrumentButton(renderer, abs_pos + Vector2(n_px, n_px), button_size, new Texture(renderer, "skins/Brush.bmp"));
//     // createInstrumentButton(renderer, abs_pos + Vector2(2 * n_px + button_size.getX(), n_px), button_size, new Texture(renderer, "skins/Eraser.bmp"));
// }

InstrumentPanel* InstrumentPanel::panel{nullptr};

InstrumentPanel::InstrumentPanel(const Vector2& pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin) : AbstractWindow(pos, size, handler, parent, skin) {

}

void InstrumentPanel::create(Renderer* renderer, AbstractWindow* parent) {
    // assert(!is_created && (panel == nullptr));
    // panel->instrument_buttons = new List<Button*>();
    // EventHandler* panel_handler = dynamic_cast<EventHandler*>(new MovingHandler(nullptr, ))
    Texture* texture = new Texture(renderer, {63, 92}, {138, 135, 134, 255});
    renderer->setTarget(texture);
    renderer->drawRect({0, 0}, Vector2(63, 92) - Vector2(1, 1), {0, 0, 0, 255});
    renderer->setTarget(NULL);
    panel = new InstrumentPanel({0, 0}, {63, 92}, nullptr, parent, new Skin(texture, {63, 92}));
    panel->absolute_pos = {0, 0};
    panel->size = {63, 92};
    // panel->is_created = true;
    EventHandler* panel_handler = new MovingHandler(panel, new WindowMoverFunctor(panel));
    panel->setHandler(panel_handler);
}

void InstrumentPanel::addInstrument(AbstractInstrument* instr, ButtonSkin* skin) {
    EventHandler* btn_handler = new ListElementHandler(nullptr, new InstrumentPickerFunctor(instr));
    int instr_cnt = panel->children->getSize();
    Vector2 btn_pos(0, 0);
    btn_pos.setY(panel->absolute_pos.getY() + instr_cnt / 2 * (icons_size + px_offset) + px_offset);
    btn_pos.setX(panel->absolute_pos.getX() + (instr_cnt % 2) * (icons_size + px_offset) + px_offset);
    Vector2 btn_sz(icons_size, icons_size);
    skin->resize(btn_sz);
    AbstractWindow* btn = dynamic_cast<AbstractWindow*>(new Button(btn_pos, btn_sz, btn_handler, panel, skin));
    btn_handler->setWindow(btn);
    printf("created button %p\n", btn);
    attachWindow(btn);
}

AbstractInstrument* InstrumentPanel::getCurrentInstrument() {
    return current_instrument;
}

InstrumentPanel* InstrumentPanel::getInstance() {
    return panel;
}

void InstrumentPanel::setInstrument(AbstractInstrument* instrument) {
    printf("setting\n");
    current_instrument = instrument;
    printf("setted\n");
}

void InstrumentPanel::destroy() {
    // assert(is_created && (panel != nullptr));
    // delete panel;
}

InstrumentPanel::~InstrumentPanel() {
    // eh
};