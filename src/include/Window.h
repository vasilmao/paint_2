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
    AbstractWindow() : children(new List<AbstractWindow*>()) {}
    AbstractWindow(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin);
    virtual ~AbstractWindow();
    virtual bool onEvent(GUIEvent* event);
    void render(Renderer* renderer);
    void markToDelete();
    bool isToDelete();
    List<AbstractWindow*>* getChildren();
    AbstractWindow* getParent();
    const Vector2& getPos() const;
    void setPos(const Vector2& new_pos) {   
        Vector2 last_pos = absolute_pos;
        absolute_pos = new_pos;

        for (List<AbstractWindow*>::Iterator it = children->begin(); it.isValid(); ++it) {
            it.getData()->setPos(it.getData()->getPos() + new_pos - last_pos);
        }
    }
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
    void setResponce(Functor<>* functor);
    void setHover();
    void setPressed();
    void setUsual();
};

class CanvasWindow : public AbstractWindow {
protected:
    const float titlebar_height = 25;
    const Vector2 close_button_size = {titlebar_height, titlebar_height};
    virtual void createTitlebar(Renderer* renderer, const Vector2& tb_pos, const Vector2& tb_size);
    virtual void createCloseButton(Renderer* renderer, const Vector2& btn_pos, const Vector2& btn_size, AbstractWindow* titlebar);
    virtual void createCanvas(Renderer* renderer, const Vector2& pos, const Vector2& size); 
public:
    CanvasWindow() {}
    CanvasWindow(Renderer* renderer, const Vector2& pos, const Vector2& size);
};

class CanvasViewportWindow : public CanvasWindow {
private:
    // const float titlebar_height = 25;
    // const Vector2 close_button_size = {titlebar_height, titlebar_height};
    // void createTitlebar(Renderer* renderer, const Vector2& tb_pos, const Vector2& tb_size);
    // void createCloseButton(Renderer* renderer, const Vector2& btn_pos, const Vector2& btn_size, AbstractWindow* titlebar);
    void createCanvas(Renderer* renderer, const Vector2& pos, const Vector2& size);
public:
    CanvasViewportWindow(Renderer* renderer, const Vector2& pos, const Vector2& size);
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
    static constexpr Color bg_color = {138, 135, 134, 255};
    static constexpr float forced_icon_size = 32.0;
    static constexpr float up_offset = 20.0;
    static constexpr float px_interval = 5.0;
    static constexpr float icons_size = 32.0;
    static bool is_created;
    static InstrumentPanel* panel;
    static Renderer* my_renderer;
    // static char* skins_dir;
    // List<AbstractInstrument*> instruments;
    AbstractInstrument* current_instrument = nullptr;
    AbstractWindow* main_window;
    InstrumentPanel();
    InstrumentPanel(const Vector2& pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin);
public:
    static void create(Renderer* renderer, AbstractWindow* main_window);
    // static void setSkinsDir(char* path);
    // static const char* getSkinsDir();
    static void destroy();
    void addInstrument(AbstractInstrument* instr, ButtonSkin* skin);
    AbstractInstrument* getCurrentInstrument();
    void setInstrument(AbstractInstrument* instr);
    virtual ~InstrumentPanel();
    static InstrumentPanel* getInstance();
    AbstractWindow* getPrefPanel(const Vector2& pos, AbstractWindow* parent);
    Renderer* getRenderer() {
        return my_renderer;
    }
};

class SliderBody : public AbstractWindow {
public:
    SliderBody(Renderer* renderer, const Vector2& pos, const Vector2& size, Functor<float, float>* slider_reaction, float min_val = 0, float max_val = 1, float init_val = nanf("1")); // creates the slider
};

class Slider : public AbstractWindow {
private:
    // float min_value = 0;
    // float max_value = 1;
    // float current_value = 0;
    // Vector2 pos_at_min_value;
    // Vector2 pos_at_max_value;
    Vector2 move_dir;
public:
    void setPressed();
    void setUsual();
    float getValue();
    Slider(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin);
};

class RayCasterHolder : public AbstractWindow {
private:
    const float titlebar_height = 25;
    const Vector2 close_button_size = {titlebar_height, titlebar_height};
    virtual void createTitlebar(Renderer* renderer, const Vector2& tb_pos, const Vector2& tb_size);
    // virtual void createSphere(Renderer* renderer, const Vector2& pos, const Vector2& size);
    virtual void createCloseButton(Renderer* renderer, const Vector2& btn_pos, const Vector2& btn_size, AbstractWindow* titlebar);
public:
    RayCasterHolder(Renderer* renderer, const Vector2& abs_pos, const Vector2& size, AbstractWindow* parent);
};

class RayCaster : public AbstractWindow {
public:
    RayCaster(const Vector2& abs_pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin);
    // virtual void soloRender(Renderer* renderer);
};

class TextLabel : public AbstractWindow {
public:
    TextLabel(Renderer* renderer, const Vector2& pos, const Vector2& size, AbstractWindow* parent, const char* text);
};

#endif