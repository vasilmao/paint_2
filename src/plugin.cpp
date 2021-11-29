// #include "plugin.hpp"

// #include <stdint.h>

// #include "Window.h"

// #define plugincolor_to_color(plc) Color{static_cast<unsigned char>(plc >> 24), static_cast<unsigned char>(plc >> 16), static_cast<unsigned char>(plc >> 8), static_cast<unsigned char>(plc)}

// class PluginTexture : public plugin::ITexture {
// private:
//     Texture* tru_texture;
//     Renderer* bruh_renderer;
// public:
//     PluginTexture(Renderer* renderer, const char* filename) : tru_texture(new Texture(renderer, filename)), bruh_renderer(renderer) {

//     }

//     PluginTexture(Renderer* renderer, int w, int h) : tru_texture(new Texture(renderer, {w, h})), bruh_renderer(renderer) {

//     }

//     virtual int32_t GetWidth() {
//         return static_cast<int32_t> (tru_texture->getSize().getX());
//     }

//     virtual int32_t GetHieght() {
//         return static_cast<int32_t>(tru_texture->getSize().getY());
//     }

//     virtual void Present() {}

//     virtual void DrawRect(int32_t x, int32_t y, int32_t width, int32_t height, plugin::Color color) {
//         bruh_renderer->setTarget(tru_texture);
//         bruh_renderer->drawRect(Vector2(x, y), Vector2(width, height), plugincolor_to_color(color));
//     }

//     virtual ~PluginTexture() {
//         delete tru_texture;
//     }

//     virtual plugin::Buffer ReadBuffer() {
//         return plugin::Buffer{};
//     }

//     virtual void ReleaseBuffer(plugin::Buffer buffer) {

//     }

//     virtual void LoadBuffer(plugin::Buffer buffer) {

//     }

//     virtual void Clear(plugin::Color color) {

//     }

//     virtual void Present() {

//     }

//     virtual void DrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, plugin::Color color) {

//     }

//     virtual void DrawThickLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t thickness, plugin::Color color) {

//     }

//     virtual void DrawCircle(int32_t x, int32_t y, int32_t radius, plugin::Color color) {

//     }

//     virtual void CopyTexture(ITexture* texture, int32_t x, int32_t y, int32_t width, int32_t height) {

//     }
// };

// class Factory : public plugin::ITextureFactory {
// private:
//     Renderer* bruh_renderer;
// public:
//     virtual plugin::ITexture* CreateTexture(const char* filename) {
//         return new PluginTexture(bruh_renderer, filename);
//     }
//     virtual plugin::ITexture* CreateTexture(int32_t width, int32_t height) {
//         return new PluginTexture(bruh_renderer, width, height);
//     }
// };

// class ClickCallback : public plugin::IClickCallback {
//   public:
//     virtual ~ClickCallback() {}

//     virtual void RespondOnClick() {

//     }
// };

// class SliderCallback : public plugin::ISliderCallback {
// public:
//     virtual ~SliderCallback() {}

//     virtual void RespondOnSlide(float old_value, float current_value) {

//     }
// };

// class Widget {
//   public:
//     virtual ~IWidget() {}

//     virtual int32_t GetWidth() = 0;
//     virtual int32_t GetHieght() = 0;
// };

// class IButton : public IWidget {
//   public:
//     virtual ~IButton() {}

//     virtual void SetClickCallback(IClickCallback* callback) = 0;
// };