#include "Skins.h"

Skin::Skin() : texture(nullptr) {}

Skin::Skin(Texture* texture, const Vector2& size) : texture(texture), real_size(size) {

}

Skin::Skin(Texture* texture) : texture(texture), real_size(texture->getSize()) {

}

Skin::~Skin() {
    delete texture;
}

void Skin::draw(Renderer* renderer, const Vector2& pos) {
    // printf("default skin %p, pos is (%f, %f), drawing\n", this, pos.getX(), pos.getY());
    // printf("real size is (%f, %f)\n", real_size.getX(), real_size.getY());
    renderer->copyTexture(texture, pos, real_size);
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

void Skin::resize(const Vector2& new_size) {
    real_size = new_size;
}

RepeatingSkin::RepeatingSkin(Texture* texture) : Skin(texture) {}
RepeatingSkin::RepeatingSkin(Texture* texture, const Vector2& size) : Skin(texture, size) {}

void RepeatingSkin::draw(Renderer* renderer, const Vector2& pos) {
    // renderer->setTarget()
    const Vector2& texture_size = texture->getSize();
    for (float x = pos.getX(); x <= pos.getX() + real_size.getX(); x += texture_size.getX()) {
        for (float y = pos.getY(); y <= pos.getY() + real_size.getY(); y += texture_size.getY()) {
            if (x + texture_size.getX() > pos.getX() + real_size.getX() || y + texture_size.getY() > pos.getY() + real_size.getY()) {
                float p_x = Min(x + texture_size.getX(), pos.getX() + real_size.getX());
                float p_y = Min(y + texture_size.getY(), pos.getY() + real_size.getY());
                Rect2f texture_rect = {0, 0, p_x - x, p_y - y};
                Rect2f dst_rect = {x, y, p_x - x, p_y - y};
                renderer->copyTexture(texture, texture_rect, dst_rect);
            } else {
                renderer->copyTexture(texture, {x, y});
            }
        }
    }
}

// void RepeatingSkin::draw(Renderer* renderer, const Vector2& pos) {
//     const Vector2& texture_size = texture->getSize();
//     for (float x = pos.getX(); x <= pos.getX() + real_size.getX(); x += 1) {
//         for (float y = pos.getY(); y <= pos.getY() + real_size.getY(); y += 1) {
//             if (x + texture_size.getX() > pos.getX() + real_size.getX() || y + texture_size.getY() > pos.getY() + real_size.getY()) {
//                 float size_x = Min(x + texture_size.getX(), pos.getX() + real_size.getX());
//                 float size_y = Min(y + texture_size.getY(), pos.getY() + real_size.getY());
//                 Rect2f texture_rect = {0, 0, size_x, size_y};
//                 renderer->copyTextureRect(texture, {x, y}, texture_rect);
//             } else {
//                 renderer->copyTexture(texture, {x, y});
//             }
//         }
//     }
// }

ViewportSkin::ViewportSkin(Texture* texture, const Vector2& viewport_size) : Skin(texture, {viewport_size.getX(), viewport_size.getY()}),
    full_texture_size(texture->getSize()) {
    viewport = {0, 0, viewport_size.getX(), viewport_size.getY()};
}

const Rect2f& ViewportSkin::getViewport() {
    return viewport;
}

void ViewportSkin::draw(Renderer* renderer, const Vector2& pos) {
    renderer->copyTexture(texture, viewport, Rect2f{pos.getX(), pos.getY(), viewport.width, viewport.height});
}

void ViewportSkin::moveViewPort(const Vector2& delta) {
    viewport.x += delta.getX();
    viewport.y += delta.getY();
}

void ViewportSkin::setViewPort(const Rect2f& new_viewport) {
    viewport = new_viewport;
}

const Vector2& ViewportSkin::getFullSize() {
    return texture->getSize();
}

ButtonSkin::ButtonSkin(Texture* texture, Texture* h_t, Texture* p_t) : usual_texture(texture), hovered_texture(h_t), pressed_texture(p_t) {
    current_texture = usual_texture;
    // printf("usual vs pressed: %p, %p\n", usual_texture, pressed_texture);
    real_size = current_texture->getSize();
}

void ButtonSkin::setUsual() {
    printf("usual!\n");
    current_texture = usual_texture;
}

void ButtonSkin::setHover() {
    if (hovered_texture != nullptr) {
        current_texture = hovered_texture;
    }
}

void ButtonSkin::setPressed() {
    printf("pressed!\n");
    if (pressed_texture != nullptr) {
        current_texture = pressed_texture;
    }
}

// int cnt = 0;

void ButtonSkin::draw(Renderer* renderer, const Vector2& pos) {
    // printf("button skin %p, pos is (%f, %f), drawing\n", this, pos.getX(), pos.getY());
    // printf("real size is (%f, %f)\n", real_size.getX(), real_size.getY());
    // printf("button: drawing texture %p\n", current_texture);
    renderer->setTarget(NULL);
    renderer->copyTexture(current_texture, pos, real_size);
    // cnt++;
    // if (cnt == 5) {
    //     renderer->setTarget(pressed_texture);
    //     // renderer->drawFilledRect({0, 0}, {10, 10}, {255,255,255,255});
    //     renderer->setTarget(NULL);
    //     renderer->copyTexture(pressed_texture, {0, 0}, real_size);
    //     renderer->render();
    //     SDL_Delay(3000);
    //     exit(0);
    // }
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

LeakyCircleSkin::LeakyCircleSkin(Texture* u_t, Texture* h_t, Texture* p_t, float big_r, float small_r) :
    ButtonSkin(u_t, h_t, p_t), big_r(big_r), small_r(small_r) {
    
}

bool LeakyCircleSkin::hitTest(const Vector2& pos) {
    Vector2 center = real_size * 0.5;
    float dx = pos.getX() - center.getX();
    float dy = pos.getY() - center.getY();
    bool inner_circle = (dx * dx + dy * dy <= small_r * small_r);
    bool outer_circle = (dx * dx + dy * dy <= big_r * big_r);
    return outer_circle && (!inner_circle);
}