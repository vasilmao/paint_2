#include "PluginRealization.h"
#include "Handlers.h"


APITexture::~APITexture() {
    delete texture;
}

int32_t APITexture::GetSizeX() {
    return texture->getSize().getX();
}

int32_t APITexture::GetSizeY() {
    return texture->getSize().getY();
}

Buffer APITexture::ReadBuffer() {
    return Buffer{texture->copyIntBuffer(), this};
}

void APITexture::ReleaseBuffer(Buffer buffer) {
    delete[] buffer.pixels;
}

void APITexture::LoadBuffer(Buffer buffer) {
    texture->updatePixels(buffer.pixels);
}


void APITexture::Clear(color_t color) {
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    renderer->setTarget(texture);
    renderer->drawFilledRect({0, 0}, texture->getSize(), color); // non-explicit costructor works
}

void APITexture::Present() {}

void APITexture::DrawLine(const Line& line) {
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    renderer->setTarget(texture);
    renderer->setTarget(texture);
    renderer->drawLine({line.x0, line.y0}, {line.x1, line.y1}, line.color); // non-explicit
}

void APITexture::DrawCircle(const Circle& circle) {
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    renderer->setTarget(texture);
    renderer->drawCircle({circle.x, circle.y}, circle.radius, circle.fill_color); // non-explicit
}

void APITexture::DrawRect(const Rect& rect) {
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    renderer->setTarget(texture);
    renderer->drawFilledRect({rect.x, rect.y}, {rect.size_x, rect.size_y}, rect.fill_color); // non-explicit
}

void APITexture::CopyTexture(ITexture* source, int32_t x, int32_t y, int32_t size_x, int32_t size_y) {
    APITexture* tru_texture = dynamic_cast<APITexture*>(source);
    assert(tru_texture);
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    renderer->setTarget(texture);
    renderer->copyTexture(tru_texture->texture, Rect2f{0, 0, static_cast<float>(size_x), static_cast<float>(size_y)}, Rect2f{static_cast<float>(x), static_cast<float>(y), static_cast<float>(size_x), static_cast<float>(size_y)});
}

void APITexture::CopyTexture(ITexture* source, int32_t x, int32_t y) {
    APITexture* tru_texture = dynamic_cast<APITexture*>(source);
    assert(tru_texture);
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    renderer->setTarget(texture);
    renderer->copyTexture(tru_texture->texture, Vector2{static_cast<float>(x), static_cast<float>(y)});
}

APITexture* APITextureFactory::CreateTexture(const char* filename) {
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    return new APITexture(new Texture(renderer, filename, false));
}

APITexture* APITextureFactory::CreateTexture(int32_t size_x, int32_t size_y) {
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    return new APITexture(new Texture(renderer, {static_cast<float>(size_x), static_cast<float>(size_y)}));
}

// plugin does callback code

int32_t APIWidget::GetSizeX() {
    return window->getSize().getX();
}

int32_t APIWidget::GetSizeY() {
    return window->getSize().getY();
}

void APIButton::SetClickCallback(IClickCallback* callback) {
    Button* btn = dynamic_cast<Button*>(window)
}