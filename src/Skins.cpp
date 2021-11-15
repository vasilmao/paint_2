#include "Skins.h"

Skin::Skin() : texture(nullptr) {}

Skin::Skin(Texture* texture, const Vector2& size) : texture(texture), real_size(size) {

}


Skin::~Skin() {
    delete texture;
}

void Skin::draw(Renderer* renderer, const Vector2& pos) {
    // printf("default skin %p, pos is (%f, %f), drawing\n", this, pos.getX(), pos.getY());
    // printf("real size is (%f, %f)\n", real_size.getX(), real_size.getY());
    renderer->copyTexture(texture, pos);
}

bool Skin::hitTest(const Vector2& pos) {
    return 0 <= pos.getX() &&
           pos.getX() < real_size.getX() &&
           0 <= pos.getY() &&
           pos.getY() < real_size.getY();
}

Texture* Skin::getTexture() {
    return texture;
}

void RepeatingSkin::draw(Renderer* renderer, const Vector2& pos) {
    const Vector2& texture_size = texture->getSize();
    for (float x = pos.getX(); x <= pos.getX() + real_size.getX(); x += 1) {
        for (float y = pos.getY(); y <= pos.getY() + real_size.getY(); y += 1) {
            if (x + texture_size.getX() > pos.getX() + real_size.getX() || y + texture_size.getY() > pos.getY() + real_size.getY()) {
                float size_x = Min(x + texture_size.getX(), pos.getX() + real_size.getX());
                float size_y = Min(y + texture_size.getY(), pos.getY() + real_size.getY());
                Rect2f texture_rect = {0, 0, size_x, size_y};
                renderer->copyTextureRect(texture, {x, y}, texture_rect);
            } else {
                renderer->copyTexture(texture, {x, y});
            }
        }
    }
}

ButtonSkin::ButtonSkin(Texture* texture, Texture* h_t, Texture* p_t) : usual_texture(texture), hovered_texture(h_t), pressed_texture(p_t) {
    current_texture = usual_texture;
    real_size = current_texture->getSize();
}

void ButtonSkin::setUsual() {
    current_texture = usual_texture;
}

void ButtonSkin::setHover() {
    if (hovered_texture != nullptr) {
        current_texture = hovered_texture;
    }
}

void ButtonSkin::setPressed() {
    if (pressed_texture != nullptr) {
        current_texture = pressed_texture;
    }
}

void ButtonSkin::draw(Renderer* renderer, const Vector2& pos) {
    // printf("button skin %p, pos is (%f, %f), drawing\n", this, pos.getX(), pos.getY());
    // printf("real size is (%f, %f)\n", real_size.getX(), real_size.getY());
    renderer->copyTexture(current_texture, pos);
}

ButtonSkin::~ButtonSkin() {
    delete usual_texture;
    if (pressed_texture != nullptr) {
        delete pressed_texture;
    }
    if (hovered_texture != nullptr) {
        delete hovered_texture;
    }
}