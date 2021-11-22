#ifndef INCLUDE_WINDOWS
#define INCLUDE_WINDOWS

#include "Vector.h"
#include "GraphicLib.h"
#include "List.h"
#include "GUIEvents.h"
#include "Skins.h"
#include "Instruments.h"
// #include "Functors.h"

class EventHandler;
template<class ...Args>
class Functor;

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
    virtual void soloRender(Renderer* renderer);
    void renderChildren(Renderer* renderer);

public:

    AbstractWindow(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin);
    virtual ~AbstractWindow();
    virtual bool onEvent(GUIEvent* event);
    void render(Renderer* renderer);
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
    void setHandler(EventHandler* new_handler) {
        handler = new_handler;
    }
};

class MainWindow : public AbstractWindow { // has titlebar, and global close button, not much functionality, just creates them, pos is always 0, 0
private:
    const float titlebar_height = 25;
    const Vector2 close_button_size = {titlebar_height, titlebar_height};
    void createTitlebar(Renderer* renderer, const Vector2& tb_pos, const Vector2& tb_size);
    void createCloseButton(Renderer* renderer, const Vector2& btn_pos, const Vector2& btn_size, AbstractWindow* titlebar);
    void createSignedButton(Renderer* renderer, const Vector2& btn_pos, const Vector2& btn_size, AbstractWindow* titlebar, const char* text, Functor<>* butn_functor);
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
    const float titlebar_height = 25;
    const Vector2 close_button_size = {titlebar_height, titlebar_height};
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

class InstrumentPanel : public AbstractWindow { // singleton
private:
    const float px_offset = 5.0;
    const float icons_size = 24.0;
    static bool is_created;
    static InstrumentPanel* panel;
    // List<AbstractInstrument*> instruments;
    AbstractInstrument* current_instrument = nullptr;
    InstrumentPanel();
    InstrumentPanel(const Vector2& pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin);
public:
    static void create(Renderer* renderer, AbstractWindow* parent);
    static void destroy();
    void addInstrument(AbstractInstrument* instr, ButtonSkin* skin);
    AbstractInstrument* getCurrentInstrument();
    void setInstrument(AbstractInstrument* instr);
    virtual ~InstrumentPanel();
    static InstrumentPanel* getInstance();
};

#endif