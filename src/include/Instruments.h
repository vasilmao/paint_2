#ifndef INCLUDE_INSTRUMENTS
#define INCLUDE_INSTRUMENTS

#include "GraphicLib.h"
#include "plugin.hpp"

class AbstractWindow;

class AbstractInstrument {
public:
    virtual void apply(Renderer* renderer, Texture* texture, const Vector2& pos) = 0;
    virtual void applyStart(Renderer* renderer, Texture* texture, const Vector2& pos) = 0;
    virtual void applyMove(Renderer* renderer, Texture* texture, const Vector2& pos1, const Vector2& pos2) = 0;
    virtual void applyEnd(Renderer* renderer, Texture* texture, const Vector2& pos) = 0;
    virtual ~AbstractInstrument(){};
    virtual AbstractWindow* createPrefPanel(Renderer* renderer, const Vector2& pos, AbstractWindow* parent) = 0;
};

class Brush : public AbstractInstrument {
protected:
    Color color = {255, 255, 255, 255};
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
    float getR() {
        return r;
    }
    virtual void apply(Renderer* renderer, Texture* texture, const Vector2& pos) {
        applyStart(renderer, texture, pos);
    }
    virtual void applyStart(Renderer* renderer, Texture* texture, const Vector2& pos) {
        renderer->setTarget(texture);
        renderer->drawFilledCircle(pos, r, color);
        renderer->setTarget(NULL);
        printf("applied!\n");
    }
    virtual void applyMove(Renderer* renderer, Texture* texture, const Vector2& pos1, const Vector2& pos2) {
        float steps = (pos2 - pos1).getLength() * 3;
        renderer->setTarget(texture);
        for (float cur_step = 0; cur_step < steps + EPS; cur_step += 1) {
            // Vector2 cur_pos = ((pos2 - pos1) * (cur_step / steps)) + pos1;
            Vector2 cur_pos = pos1 * ((steps - cur_step) / steps) + pos2 * (cur_step / steps);
            renderer->drawFilledCircle(cur_pos, r, color);
        }
        renderer->setTarget(NULL);
    }
    virtual void applyEnd(Renderer* renderer, Texture* texture, const Vector2& pos) {
        applyStart(renderer, texture, pos);
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

class Filler : public AbstractInstrument {
private:
    Color color = {255, 255, 255, 255};
public:
    Filler (){
    }
};

class ToolInstrument : public AbstractInstrument {
private:
    plugin::ITool* plugin_tool;
public:
    ToolInstrument(plugin::ITool* tool) : plugin_tool(tool) {}
    virtual void apply(Renderer* renderer, Texture* texture, const Vector2& pos);
    virtual void applyStart(Renderer* renderer, Texture* texture, const Vector2& pos);
    virtual void applyMove(Renderer* renderer, Texture* texture, const Vector2& pos1, const Vector2& pos2);
    virtual void applyEnd(Renderer* renderer, Texture* texture, const Vector2& pos);
    // virtual ~AbstractInstrument(){};
    virtual AbstractWindow* createPrefPanel(Renderer* renderer, const Vector2& pos, AbstractWindow* parent);
};



#endif