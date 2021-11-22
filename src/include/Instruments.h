#ifndef INCLUDE_INSTRUMENTS
#define INCLUDE_INSTRUMENTS

#include "GraphicLib.h"

class AbstractInstrument {
public:
    virtual void apply(Renderer* renderer, Texture* texture, const Vector2& pos) = 0;
    virtual ~AbstractInstrument(){};
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
    void setR(float new_r) {
        r = new_r;
    }
    virtual void apply(Renderer* renderer, Texture* texture, const Vector2& pos) {
        renderer->setTarget(texture);
        renderer->drawFilledCircle(pos, r, color);
        renderer->setTarget(NULL);
    }
    virtual ~Brush(){};
};

class Eraser : public Brush {
public:
    Eraser(){
        color = {0, 0, 0, 255};
    }
    void setR(float new_r) {
        r = new_r;
    }
};

#endif