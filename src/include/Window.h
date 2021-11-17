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
    Vector2 absolute_pos;
    Vector2 size;
    EventHandler* handler;
    AbstractWindow* parent;
    Skin* skin;
    bool on_delete = false;

public:

    AbstractWindow(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin);
    virtual ~AbstractWindow();
    virtual bool onEvent(GUIEvent* event);
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
    const Vector2 close_button_size = {32, 32};
    void createTitlebar(Renderer* renderer, const Vector2& tb_pos, const Vector2& tb_size);
    void createCloseButton(Renderer* renderer, const Vector2& btn_pos, const Vector2& btn_size, AbstractWindow* titlebar);
    // AbstractInstrument* instrument;
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
private:
    const Vector2 close_button_size = {32, 32};
    void createTitlebar(Renderer* renderer, const Vector2& tb_pos, const Vector2& tb_size);
    void createCloseButton(Renderer* renderer, const Vector2& btn_pos, const Vector2& btn_size, AbstractWindow* titlebar);
    void createCanvas(Renderer* renderer, const Vector2& pos, const Vector2& size); 
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