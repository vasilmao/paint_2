#ifndef INCLUDE_GUI_EVENTS
#define INCLUDE_GUI_EVENTS

#include "Vector.h"
#include "Tools.h"
#include "GraphicLib.h"
#include "Instruments.h"


class GUIEvent {
public:
    enum class GUIEventTypes {
        NO_EVENT,
        OTHER_EVENTS,
        MOUSE_BUTTON,
        MOUSE_MOVE,
        CLOSE,
        INSTRUMENT_CHANGED,
        LIST_ELEMENT_CHANGED,
        EVENTS_CNT
    };
protected:
    GUIEventTypes type_id;
    GUIEvent(GUIEventTypes type_id) : type_id(type_id) {}
public: 
    int getType() {
        return static_cast<int> (type_id);
    }
    virtual ~GUIEvent() {}

};

class GUIMouseClickEvent : public GUIEvent {
private:
    Vector2 pos;
    uint8_t button_type;
    bool button_down;
public:
    GUIMouseClickEvent(Vector2 pos, uint8_t button_type, bool button_down) : GUIEvent(GUIEventTypes::MOUSE_BUTTON), pos(pos), button_type(button_type), button_down(button_down) {}

    GUIMouseClickEvent(SystemEvent event) : GUIEvent(GUIEventTypes::MOUSE_BUTTON), pos(event.mb_press_info.pos), button_type(event.mb_press_info.button_type), button_down(event.mb_press_info.button_down) {
        assert(event.event_type == SystemEventTypes::MOUSE_CLICK);
    }

    const Vector2& getPos() const {
        return pos;
    }

    uint8_t getButtonType() {
        return button_type;
    }

    bool isButtonDown() {
        return button_down;
    }
};

class GUIMouseMove : public GUIEvent {
private:
    Vector2 prev_pos;
    Vector2 new_pos;
public:
    GUIMouseMove(Vector2 pos1, Vector2 pos2) : GUIEvent(GUIEventTypes::MOUSE_MOVE), prev_pos(pos1), new_pos(pos2) {}
    GUIMouseMove(SystemEvent event) : GUIEvent(GUIEventTypes::MOUSE_MOVE), prev_pos(event.mouse_move.prev_pos), new_pos(event.mouse_move.new_pos) {
        assert(event.event_type == SystemEventTypes::MOUSE_MOVE);
    }

    const Vector2& getPrevPos() const {
        return prev_pos;
    }

    const Vector2& getNewPos() const {
        return new_pos;
    }

};

class GUIClose : public GUIEvent {
public:
    GUIClose() : GUIEvent(GUIEventTypes::CLOSE) {}
};


class GUIInstrumentChanged : public GUIEvent {
private:
    AbstractInstrument* new_instrument;
public:
    GUIInstrumentChanged(AbstractInstrument* new_instr) : GUIEvent(GUIEventTypes::INSTRUMENT_CHANGED), new_instrument(new_instr) {}
    AbstractInstrument* getInstrument() {
        return new_instrument;
    }
};

class GUIListElementChanged : public GUIEvent {
public:
    GUIListElementChanged() : GUIEvent(GUIEventTypes::LIST_ELEMENT_CHANGED) {}
};

#endif