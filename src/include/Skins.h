#ifndef INCLUDE_SKINS
#define INCLUDE_SKINS

#include "GraphicLib.h"

class Skin {
protected:
    Texture* texture;
    Vector2 real_size;
public:
    Skin();
    Skin(Texture* texture);
    Skin(Texture* texture, const Vector2& size);
    virtual void draw(Renderer* renderer, const Vector2& pos);
    virtual bool hitTest(const Vector2& pos);
    virtual ~Skin();
    void resize(const Vector2& new_size);
    Texture* getTexture();
    void setTexture(Texture* new_texture);
};

class ViewportSkin : public Skin {
protected:
    // Texture* full_texture;
    Vector2 full_texture_size;
    Rect2f viewport;
public:
    ViewportSkin();
    ViewportSkin(Texture* texture, const Vector2& viewport_size);
    virtual void draw(Renderer* renderer, const Vector2& pos);
    // virtual bool hitTest(const Vector2& pos);
    // virtual ~ViewportSkin();
    void resizeViewport(const Vector2& new_size);
    void moveViewPort(const Vector2& delta);
    void setViewPort(const Rect2f& new_viewport);
    const Rect2f& getViewport();
    const Vector2& getFullSize();
};

class RepeatingSkin : public Skin {
public:
    RepeatingSkin(Texture* texture);
    RepeatingSkin(Texture* texture, const Vector2& size);
    virtual void draw(Renderer* renderer, const Vector2& pos);
};

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

class CircleButtonSkin : public ButtonSkin {
private:
    Vector2 rel_center;
    float r;
public:
    CircleButtonSkin(Texture* texture, Texture* h_t, Texture* p_t) : ButtonSkin(texture, h_t, p_t) {
        rel_center = texture->getSize() * 0.5;
        r = Min(rel_center.getX(), rel_center.getY());
    }
    CircleButtonSkin(Texture* texture, Texture* h_t, Texture* p_t, const Vector2& p, float r) : ButtonSkin(texture, h_t, p_t), rel_center(p), r(r) {}
    virtual bool hitTest(const Vector2& pos) {
        float dx = pos.getX() - rel_center.getX();
        float dy = pos.getY() - rel_center.getY();
        return (dx * dx + dy * dy <= r * r + EPS);
    }
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