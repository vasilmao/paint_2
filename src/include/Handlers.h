//----------Event Handlers----------
#ifndef INCLUDE_HASNDLERS
#define INCLUDE_HASNDLERS

#include "GUIEvents.h"
#include "Window.h"
#include "Functors.h"
#include "Instruments.h"

#include "RayCaster.h"

class EventHandler {
protected:
    AbstractWindow* my_window;
    bool passEvent(GUIEvent* event);
    bool spreadEvent(GUIEvent* event);

public:
    virtual bool MMResponce(GUIMouseMove* mm_event);
    virtual bool MBLResponce(GUILeftMouseButton* mbl_event);
    virtual bool MBRResponce(GUIRightMouseButton* mbr_event);
    virtual bool LECResponce(GUIListElementChanged* list_event);
    virtual bool TIMEResponce(GUITimePassed* time_event);
    EventHandler(AbstractWindow* window) : my_window(window){};
    EventHandler() : my_window(nullptr) {}
    virtual ~EventHandler();
    bool onEvent(GUIEvent* event);
    virtual void setWindow(AbstractWindow* window);
};

class ButtonHandler : public EventHandler {
private:
    bool is_pressed = false;
    bool is_hovered = false;
    Functor<>* click_event_responce;
    virtual bool MBLResponce(GUILeftMouseButton* mbl_event);
public:
    ButtonHandler(AbstractWindow* window, Functor<>* click_functor);
    // virtual bool onEvent(GUIEvent* event);
    virtual ~ButtonHandler();
    // void setHoverResponce(ButtonHoverFunctor* hover_start, ButtonHoverFunctor* hover_end);
};

// class MouseCapturingHandler : public EventHandler {
// protected:
//     bool is_pressed = false;
// public:
//     MouseCapturingHandler() : EventHandler(nullptr) {};
//     MouseCapturingHandler(AbstractWindow* window);
//     virtual bool onEvent(GUIEvent* event);
//     virtual bool mbResponce(GUILeftMouseButton* mouse_click) = 0;
//     virtual bool mmResponce(GUIMouseMove* mouse_move) = 0;
//     virtual ~MouseCapturingHandler() = 0;
// };

class MovingHandler : public EventHandler {
private:
    bool is_pressed = false;
    Functor<const Vector2&>* window_mover;
    virtual bool MMResponce(GUIMouseMove* mm_event);
    virtual bool MBLResponce(GUILeftMouseButton* mbl_event);
public:
    MovingHandler(AbstractWindow* window, Functor<const Vector2&>* move_functor);
    // virtual bool onEvent(GUIEvent* event);
    virtual ~MovingHandler();
};

class CanvasHandler : public EventHandler {
protected:
    bool is_pressed = false;
        virtual bool MMResponce(GUIMouseMove* mm_event);
    virtual bool MBLResponce(GUILeftMouseButton* mbl_event);
    Renderer* renderer;
    Functor<Renderer*, Texture*, const Vector2&>* canvas_drawer;
    AbstractInstrument* current_instrument;
public:
    virtual ~CanvasHandler();
    CanvasHandler(AbstractWindow* window, Renderer* renderer, Functor<Renderer*, Texture*, const Vector2&>* canvas_drawer);
    // virtual bool onEvent(GUIEvent* event);
};

// class CanvasViewportHandler : public CanvasHandler {
//     virtual bool mbResponce(GUIMouseClickEvent* mouse_click);
//     virtual bool mmResponce(GUIMouseMove* mouse_move);
// public:
//     virtual ~CanvasViewportHandler();
//     CanvasViewportHandler(AbstractWindow* window, Renderer* renderer, Functor<Renderer*, Texture*, const Vector2&>* canvas_drawer);
// }

class ListElementHandler : public EventHandler {
private:
    bool is_pressed = false;
    bool is_chosen = false;
    Functor<>* click_event_responce;
    virtual bool MBLResponce(GUILeftMouseButton* mbl_event);
    virtual bool LECResponce(GUIListElementChanged* list_event);
public:
    ListElementHandler(AbstractWindow* window, Functor<>* click_functor);
    // virtual bool onEvent(GUIEvent* event);
    virtual ~ListElementHandler();
};

class SliderHandler : public EventHandler {
private:
    Functor<float, float>* move_functor;
    Vector2 axis;
    float min_value = 0;
    float max_value = 1;
    float current_value = 0;
    bool is_pressed = false;
public:
    virtual bool MMResponce(GUIMouseMove* mm_event);
    virtual bool MBLResponce(GUILeftMouseButton* mbl_event);
    SliderHandler(Functor<float, float>* functor, const Vector2& axis, float min_val = 0, float max_val = 1);
    // virtual bool onEvent(GUIEvent* event);
    virtual ~SliderHandler();
};

class RayCasterHandler : public EventHandler {
private:
    Renderer* renderer;
    Camera* camera;
    Light* light_source;
    Sphere* sphere;
    Texture* texture = nullptr;
    virtual bool TIMEResponce(GUITimePassed* time_event);
public:
    RayCasterHandler(Renderer* renderer, Camera* camera, Light* light_source, Sphere* sphere);
    void setTexture(Texture* new_texture) {
        texture = new_texture;
    }
    void rotateLight(float angle) {
        Vector3 light_pos = light_source->getPos();
        float s = sin(angle);
        float c = cos(angle);
        float new_x = light_pos.getX() * c - light_pos.getZ() * s;
        // float new_y = light_pos.getX() * s + light_pos.getY() * c;
        float new_z = light_pos.getX() * s + light_pos.getZ() * c;
        light_pos.setX(new_x);
        // light_pos.setY(new_y);
        light_pos.setZ(new_z);
        light_source->setPos(light_pos);
        renderer->setTarget(texture);
        renderSphere(renderer, sphere, camera, light_source);
        renderer->setTarget(nullptr);
    }
};

#endif