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
    printf("attaching %p to %p\n", window, this);
    children->pushFront(window);
    // printf("attached\n");
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
    // printf("window %p got event\n", this);
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
    printf("%p is now to delete!\n", this);
    on_delete = true;
}

bool AbstractWindow::isToDelete() {
    return on_delete;
}

void AbstractWindow::clean() {
    // bool deletedd = false;
    // if ((dynamic_cast<MainWindow*>(InstrumentPanel::getInstance()->getParent()))->kek) {
    //     printf("clearing %p\n", this);
    //     printf("%d children\n", children->getSize());
    //     if (children->getSize() > 0) {
    //         List<AbstractWindow*>::Iterator it = children->begin();
    //         printf("%p, %p, %d\n", it.getNode(), it.getData(), it == children->cycleEnd());
    //     }
    // }
    // int cycle_cnt = 0;
    for (List<AbstractWindow*>::Iterator it = children->begin(); it != children->cycleEnd(); ++it) {
        // ++cycle_cnt;
        // if ((dynamic_cast<MainWindow*>(InstrumentPanel::getInstance()->getParent()))->kek) {
        //     printf("%p's child is %p\n", this, it.getData());
        // }
        if (it.getData()->isToDelete()) {
            // printf("%p is onto delete by %p\n", it.getData(), this);
            // deletedd = true;
            delete it.getData();
            children->erase(it);
        } else {
            it.getData()->clean();
        }
    }
    // if (deletedd) {
    //     printf("%p children were %d\n", this, children->getSize());
    // }
    children->clear();
    // if (deletedd) {
    //     printf("children now are %d\n", children->getSize());
    // }
    // if ((dynamic_cast<MainWindow*>(InstrumentPanel::getInstance()->getParent()))->kek) {
    //     printf("%p cleared, %d\n", this, cycle_cnt);
    // }
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

const Vector2& AbstractWindow::getSize() const {
    return size;
}