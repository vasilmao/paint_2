#ifndef INCLUDE_WINDOWS
#define INCLUDE_WINDOWS

#include "Vector.h"
#include "GraphicLib.h"
#include "List.h"
#include "GUIEvents.h"
#include "Skins.h"
#include "Instruments.h"

class EventHandler;
// class Functor;

//----------Windows----------

class AbstractWindow {
protected:
    char* title = nullptr;
    List<AbstractWindow*>* children;
    // struct {
    //     bool is_changed:1;
    //     bool to_delete:1;
    // };
    Vector2 absolute_pos;
    Vector2 size;
    EventHandler* handler;
    AbstractWindow* parent;
    Skin* skin;
    bool on_delete = false;

public:

    AbstractWindow(){}; // defined
    AbstractWindow(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin); // defined
    virtual ~AbstractWindow();
    // virtual void bubbleEvent(GUIEvent* event);  // decides, bubble it or not
    // virtual bool captureEvent(GUIEvent* event) = 0; // decides, to do responce, to spread, to pass
    virtual bool onEvent(GUIEvent* event);      // event responce
    // virtual void updateTexture(Renderer* renderer);
    virtual void render(Renderer* renderer);
    void markToDelete();
    bool isToDelete();
    List<AbstractWindow*>* getChildren();
    AbstractWindow* getParent();
    const Vector2& getPos() const;
    const Vector2& getSize() const;
    void attachWindow(AbstractWindow* window);
    bool hitTest(const Vector2& pos) const ;
    Skin* getSkin();
    void clean();

    void move(const Vector2& delta);
};

class MainWindow : public AbstractWindow { // has titlebar, and global close button, not much functionality, just creates them, pos is always 0, 0
private:
    AbstractInstrument* instrument;
public:
    MainWindow(Renderer* renderer, const Vector2& size);
};

class Button : public AbstractWindow {
public:
    Button(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin);
    void setHover();
    void setPressed();
    void setUsual();
};

class CanvasWindow : public AbstractWindow {
public:
    CanvasWindow(Renderer* renderer, const Vector2& pos, const Vector2& size);
};

class TitleBar : public AbstractWindow {
public:
    TitleBar(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin);
};

class Canvas : public AbstractWindow {
public:
    Canvas(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin); // event handler and skin/texture will be created in constructor
    // void setInstrument(AbstractInstrument* instrument);
};

#endif