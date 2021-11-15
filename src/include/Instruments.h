#ifndef INCLUDE_INSTRUMENTS
#define INCLUDE_INSTRUMENTS

#include "GraphicLib.h"

class AbstractInstrument {
public:
    virtual void apply(Renderer* renderer, Texture* texture, const Vector2& pos);
};

class Brush : public AbstractInstrument {
private:
    Color color = {255, 0, 0, 255};
    float r = 1;
public:
    Brush(){}
    Brush(Color color) : color(color) {}
    void setColor(Color new_color) {
        color = new_color;
    }
    virtual void apply(Renderer* renderer, Texture* texture, const Vector2& pos) {
        renderer->drawFilledCircle(pos, r, color);
    }
};

#endif