#include "PluginRealization.h"
#include "Handlers.h"


APITexture::~APITexture() {
    // delete texture;
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

Texture* APITexture::getGlibTexture() {
    return texture;
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
    functor_callback->setCallback(callback);
}

void APISlider::SetSliderCallback(ISliderCallback* callback) {
    functor_callback->setCallback(callback);
}

float APISlider::GetValue() {
    Slider* slider = dynamic_cast<Slider*>(window);
    assert(slider);
    return slider->getValue();
}

void APISlider::SetValue(float new_value) {
    assert(nullptr);
}

void APIPalette::SetPaletteCallback(IPaletteCallback* callback) {
    functor_callback->setCallback(callback);
}

void APILabel::SetText(const char* text) {
    Texture* texture = window->getSkin()->getTexture();
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    renderer->setTarget(texture);
    renderer->drawFilledRect({0, 0}, texture->getSize(), {0, 0, 0, 0});
    renderer->drawTextCentered({0, 0}, texture->getSize(), text, {0, 0, 0, 255});
}

void APIIcon::SetIcon(const ITexture* icon) { // bruh const
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    Skin* skin = window->getSkin();
    // if (skin->getTexture() != nullptr) {
    //     delete skin->getTexture();
    // }
    APITexture* api_texture = dynamic_cast<APITexture*>(const_cast<ITexture*>(icon));
    assert(api_texture);
    Texture* texture = api_texture->getGlibTexture();
    Texture* new_texture = new Texture(renderer, texture->getSize());
    renderer->setTarget(new_texture);
    renderer->copyTexture(texture, {0, 0});
    skin->setTexture(new_texture);
}

void APIPreferencesPanel::Attach(IButton* button,  int32_t x, int32_t y) {
    APIButton* btn = dynamic_cast<APIButton*>(button);
    btn->getGlibWindow()->setPos(Vector2{static_cast<float>(x), static_cast<float>(y)});
    window->attachWindow(btn->getGlibWindow());
}

void APIPreferencesPanel::Attach(ILabel* label,   int32_t x, int32_t y) {
    APILabel* api_label = dynamic_cast<APILabel*>(label);
    api_label->getGlibWindow()->setPos(Vector2{static_cast<float>(x), static_cast<float>(y)});
    window->attachWindow(api_label->getGlibWindow());
}

void APIPreferencesPanel::Attach(ISlider* slider,  int32_t x, int32_t y) {
    APISlider* api_slider = dynamic_cast<APISlider*>(slider);
    api_slider->getGlibWindow()->setPos(Vector2{static_cast<float>(x), static_cast<float>(y)});
    window->attachWindow(api_slider->getGlibWindow());
}

void APIPreferencesPanel::Attach(IIcon* icon,    int32_t x, int32_t y) {
    APIIcon* api_icon = dynamic_cast<APIIcon*>(icon);
    api_icon->getGlibWindow()->setPos(Vector2{static_cast<float>(x), static_cast<float>(y)});
    window->attachWindow(api_icon->getGlibWindow());
}

void APIPreferencesPanel::Attach(IPalette* palette, int32_t x, int32_t y) {
    APIPalette* api_palette = dynamic_cast<APIPalette*>(palette);
    api_palette->getGlibWindow()->setPos(Vector2{static_cast<float>(x), static_cast<float>(y)});
    window->attachWindow(api_palette->getGlibWindow());
}

APIButton* APIWidgetFactory::CreateDefaultButtonWithIcon(const char* icon_file_name) {
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    Texture* texture = new Texture(renderer, icon_file_name);
    APIClickCallback* callback = new APIClickCallback();
    ButtonHandler* handler = new ButtonHandler(nullptr, callback);
    Button* btn = new Button(
        {0, 0},
        texture->getSize(),
        handler,
        nullptr,
        new ButtonSkin(texture, nullptr, texture)
    );
    handler->setWindow(btn);
    return new APIButton(
        btn,
        callback
    );
}

APIButton* APIWidgetFactory::CreateDefaultButtonWithText(const char* text) {
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    Vector2 btn_size = {100, 30};
    Texture* button_texture = new Texture(renderer, btn_size, {0, 0, 0, 0});
    renderer->setTarget(button_texture);
    // renderer->drawText(btn_pos, text, {0, 0, 0, 255});
    renderer->drawTextCentered({0, 0}, btn_size, text, {0, 0, 0, 255});
    Texture* button_texture_pressed = new Texture(renderer, btn_size, {237, 104, 64, 255});
    renderer->setTarget(button_texture_pressed);
    renderer->drawTextCentered({0, 0}, btn_size, text, {0, 0, 0, 255});
    ButtonSkin* button_skin = new ButtonSkin(
        button_texture,
        nullptr,
        button_texture_pressed
    );
    APIClickCallback* callback = new APIClickCallback();
    ButtonHandler* button_handler = new ButtonHandler(nullptr, callback);
    Button* btn = new Button(
        {0, 0},
        btn_size,
        button_handler,
        nullptr,
        button_skin
    );
    printf("signed button: %p (%s)\n", btn, text);
    button_handler->setWindow(btn);
    // titlebar->attachWindow(btn);
    return new APIButton(btn, callback);
}

APIButton* APIWidgetFactory::CreateButtonWithIcon(int32_t size_x, int32_t size_y, const char* icon_file_name) {
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    Texture* texture = new Texture(renderer, icon_file_name);
    ButtonSkin* skin = new ButtonSkin(texture, nullptr, texture);
    skin->resize(Vector2{static_cast<float>(size_x), static_cast<float>(size_y)});
    APIClickCallback* callback = new APIClickCallback();
    ButtonHandler* handler = new ButtonHandler(nullptr, callback);
    Button* btn = new Button(
        {0, 0},
        texture->getSize(),
        handler,
        nullptr,
        new ButtonSkin(texture, nullptr, texture)
    );
    handler->setWindow(btn);
    return new APIButton(
        btn,
        callback
    );
}

APIButton* APIWidgetFactory::CreateButtonWithText(int32_t size_x, int32_t size_y, const char* text, int32_t char_size) {
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    Vector2 btn_size = {static_cast<float>(strlen(text) * char_size), static_cast<float>(char_size)};
    Texture* button_texture = new Texture(renderer, btn_size, {0, 0, 0, 0});
    renderer->setTarget(button_texture);
    // renderer->drawText(btn_pos, text, {0, 0, 0, 255});
    renderer->drawTextCentered({0, 0}, btn_size, text, {0, 0, 0, 255});
    Texture* button_texture_pressed = new Texture(renderer, btn_size, {237, 104, 64, 255});
    renderer->setTarget(button_texture_pressed);
    renderer->drawTextCentered({0, 0}, btn_size, text, {0, 0, 0, 255});
    ButtonSkin* button_skin = new ButtonSkin(
        button_texture,
        nullptr,
        button_texture_pressed
    );
    button_skin->resize(Vector2{static_cast<float>(size_x), static_cast<float>(size_y)});
    APIClickCallback* callback = new APIClickCallback();
    ButtonHandler* button_handler = new ButtonHandler(nullptr, callback);
    Button* btn = new Button(
        {0, 0},
        btn_size,
        button_handler,
        nullptr,
        button_skin
    );
    printf("signed button: %p (%s)\n", btn, text);
    button_handler->setWindow(btn);
    // titlebar->attachWindow(btn);
    return new APIButton(btn, callback);
}

APISlider* APIWidgetFactory::CreateDefaultSlider(float range_min, float range_max) {
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    APISliderCallback* callback = new APISliderCallback();
    SliderBody* body = new SliderBody(renderer, {0, 0}, {200, 20}, callback, range_min, range_max, range_min);
    return new APISlider(body, callback);
}

APISlider* APIWidgetFactory::CreateSlider(int32_t size_x, int32_t size_y, float range_min, float range_max) {
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    APISliderCallback* callback = new APISliderCallback();
    SliderBody* body = new SliderBody(renderer, {0, 0}, {static_cast<float>(size_x), static_cast<float>(size_y)}, callback, range_min, range_max, range_min);
    return new APISlider(body, callback);
}

APILabel*  APIWidgetFactory::CreateDefaultLabel(const char* text) {
    return CreateLabel(100, 30, text, 30);
}

APILabel*  APIWidgetFactory::CreateLabel(int32_t size_x, int32_t size_y, const char* text, int32_t char_size) {
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    TextLabel* label = new TextLabel(renderer, {0, 0}, Vector2{static_cast<float>(size_x), static_cast<float>(size_x)}, nullptr, text);
    return new APILabel(label);
}

APIIcon* APIWidgetFactory::CreateIcon(int32_t size_x, int32_t size_y) {
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    EventHandler* handler = new EventHandler(nullptr);
    Vector2 size(static_cast<float>(size_x),static_cast<float>(size_y));
    Texture* texture = new Texture(renderer, size, {0, 0, 0, 255});
    AbstractWindow* icon = new AbstractWindow({0, 0}, size, handler, nullptr, new Skin(texture));
    handler->setWindow(icon);
    return new APIIcon(icon);
}

APIPalette* APIWidgetFactory::CreatePalette() {
    return nullptr;
}

APIPreferencesPanel* APIWidgetFactory::CreatePreferencesPanel() {
    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer();
    Vector2 size(300, 350);
    EventHandler* handler = new EventHandler(nullptr);
    Texture* texture = new Texture(renderer, size, {255, 255, 255, 255});
    renderer->setTarget(texture);
    renderer->drawRect({0, 0}, size - Vector2{1, 1}, {0, 0, 0, 255});
    AbstractWindow* pref_panel = new AbstractWindow(
        {0, 0},
        size,
        handler,
        nullptr,
        new Skin(texture)
    );
    handler->setWindow(pref_panel);
    return new APIPreferencesPanel(pref_panel);
}

IWidgetFactory*  API::GetWidgetFactory() {
    return new APIWidgetFactory();
}

ITextureFactory* API::GetTextureFactory() {
    return new APITextureFactory();
}
