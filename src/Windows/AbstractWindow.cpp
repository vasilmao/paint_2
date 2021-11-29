#include "Window.h"
#include "Handlers.h"

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