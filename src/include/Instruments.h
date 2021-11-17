#ifndef INCLUDE_INSTRUMENTS
#define INCLUDE_INSTRUMENTS

#include "GraphicLib.h"

class AbstractInstrument {
public:
    virtual void apply(Renderer* renderer, Texture* texture, const Vector2& pos) = 0;
    virtual ~AbstractInstrument(){};
};

class Brush : public AbstractInstrument {
private:
    Color color = {255, 0, 0, 255};
    float r = 6;
public:
    Brush(){}
    Brush(Color color) : color(color) {}
    void setColor(Color new_color) {
        color = new_color;
    }
    virtual void apply(Renderer* renderer, Texture* texture, const Vector2& pos) {
        renderer->setTarget(texture);
        // printf("texuture: %p\n", texture);
        // printf("drawing filled circle at (%f, %f) with color (%d, %d, %d, %d)\n", pos.getX(), pos.getY(), (int)color.r, (int)color.g, (int)color.b, (int)color.a);
        // renderer->drawFilledCircle(pos, r, color);
        renderer->drawFilledCircle(pos, r, color);
        renderer->setTarget(NULL);
        // renderer->copyTexture(texture, {0, 0});
        // renderer->render();
        // SDL_Delay(2000);
    }
    virtual ~Brush(){};
};

#endif