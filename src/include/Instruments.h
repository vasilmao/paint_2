#ifndef INCLUDE_INSTRUMENTS
#define INCLUDE_INSTRUMENTS

#include "GraphicLib.h"

class AbstractWindow;

class AbstractInstrument {
public:
    virtual void apply(Renderer* renderer, Texture* texture, const Vector2& pos) = 0;
    virtual ~AbstractInstrument(){};
    virtual AbstractWindow* createPrefPanel(Renderer* renderer, const Vector2& pos, AbstractWindow* parent) = 0;
};

class Brush : public AbstractInstrument {
protected:
    Color color = {255, 0, 0, 255};
    float r = 6;
public:
    Brush(){}
    Brush(Color color) : color(color) {}
    void setColor(Color new_color) {
        color = new_color;
    }
    Color getColor() const {
        return color;
    }
    void setR(float new_r) {
        r = new_r;
    }
    virtual void apply(Renderer* renderer, Texture* texture, const Vector2& pos) {
        renderer->setTarget(texture);
        renderer->drawFilledCircle(pos, r, color);
        renderer->setTarget(NULL);
        printf("applied!\n");
    }
    virtual ~Brush(){};
    virtual AbstractWindow* createPrefPanel(Renderer* renderer, const Vector2& pos, AbstractWindow* parent);
};

class Eraser : public Brush {
public:
    Eraser(){
        color = {255, 255, 255, 255};
    }
    void setR(float new_r) {
        r = new_r;
    }
    virtual AbstractWindow* createPrefPanel(Renderer* renderer, const Vector2& pos, AbstractWindow* parent);
};

#endif