#ifndef INCLUDE_SKINS
#define INCLUDE_SKINS

#include "GraphicLib.h"

class Skin {
protected:
    Texture* texture;
    Vector2 real_size;
public:
    Skin();
    Skin(Texture* texture, const Vector2& size);
    virtual void draw(Renderer* renderer, const Vector2& pos);
    virtual bool hitTest(const Vector2& pos);
    virtual ~Skin();
    void resize(const Vector2& new_size);
    Texture* getTexture();
};

// class RepeatingSkin : public Skin {
// public:
//     RepeatingSkin(Texture* texture, const Vector2& size);
//     virtual void draw(Renderer* renderer, const Vector2& pos);
// };

class ButtonSkin : public Skin {
protected:
    Texture* usual_texture;
    Texture* hovered_texture;
    Texture* pressed_texture;

    Texture* current_texture; // one of those 3
public:
    ButtonSkin(Texture* texture, Texture* h_t, Texture* p_t);
    virtual ~ButtonSkin();

    void setUsual();
    void setHover();
    void setPressed();
    virtual void draw(Renderer* renderer, const Vector2& pos);
};

// class ColoredRectSkin : public Skin {
//     ColoredRectSkin(const Vector2& size, Color color);
// }

class LeakyCircleSkin : public ButtonSkin {
private:
    float big_r = 0;
    float small_r = 0;
public:
    LeakyCircleSkin(Texture* u_t, Texture* h_t, Texture* p_t, float big_r, float small_r);
    virtual bool hitTest(const Vector2& pos);
};

#endif