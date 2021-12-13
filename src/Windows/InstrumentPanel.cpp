#include "Window.h"
#include "Handlers.h"

//----------InstrumentPicker----------

// InstrumentPicker::InstrumentPicker(Renderer* renderer, const Vector2& abs_pos, const Vector2& size, AbstractWindow* parent) :
//     AbstractWindow(abs_pos, size, new EventHandler(this), parent, new Skin(new Texture(renderer, size, titlebar_color), size)) {
//     // create buttons for instruments
//     // npx
//     // npx - instr - npx - instr - npx
//     float n_px = (size.getX() - 2 * button_size.getX()) / 3;
//     // createInstrumentButton(renderer, abs_pos + Vector2(n_px, n_px), button_size, new Texture(renderer, "skins/Brush.bmp"));
//     // createInstrumentButton(renderer, abs_pos + Vector2(2 * n_px + button_size.getX(), n_px), button_size, new Texture(renderer, "skins/Eraser.bmp"));
// }

// constexpr Color InstrumentPanel::bg_color;
// constexpr float InstrumentPanel::forced_icon_size;
// constexpr float InstrumentPanel::up_offset;
// constexpr float InstrumentPanel::px_interval;
// constexpr float InstrumentPanel::icons_size;
// bool InstrumentPanel::is_created;
// // InstrumentPanel* InstrumentPanel::panel;
// Renderer* InstrumentPanel::my_renderer;

InstrumentPanel* InstrumentPanel::panel{nullptr};
Renderer* InstrumentPanel::my_renderer{nullptr};

InstrumentPanel::InstrumentPanel(const Vector2& pos, const Vector2& size, EventHandler* handler, AbstractWindow* parent, Skin* skin) : AbstractWindow(pos, size, handler, parent, skin) {

}

void InstrumentPanel::create(Renderer* renderer, AbstractWindow* parent) {
    // assert(!is_created && (panel == nullptr));
    // panel->instrument_buttons = new List<Button*>();
    // EventHandler* panel_handler = dynamic_cast<EventHandler*>(new MovingHandler(nullptr, ))
    Vector2 real_size(forced_icon_size * 2 + px_interval * 3, up_offset);
    Texture* texture = new Texture(renderer, real_size, bg_color);
    renderer->setTarget(texture);
    renderer->drawRect({0, 0}, real_size - Vector2(1, 1), {0, 0, 0, 255});
    // renderer->setTarget(NULL);
    panel = new InstrumentPanel({0, 0}, real_size, nullptr, parent, new Skin(texture, real_size));
    panel->absolute_pos = {10, 30};
    panel->size = real_size;
    // panel->is_created = true;
    EventHandler* panel_handler = new MovingHandler(panel, new WindowMoverFunctor(panel));
    panel->setHandler(panel_handler);
    my_renderer = renderer;
}

void InstrumentPanel::addInstrument(AbstractInstrument* instr, ButtonSkin* skin) {
    EventHandler* btn_handler = new ListElementHandler(nullptr, new InstrumentPickerFunctor(instr));
    int instr_cnt = panel->children->getSize();
    Vector2 btn_pos(0, 0);
    if (instr_cnt % 2 == 1) {
        // we need to resize texture
        delete panel->skin;
        Vector2 new_size(forced_icon_size * 2 + px_interval * 3, up_offset + forced_icon_size * ((instr_cnt / 2) + 1) + px_interval * ((instr_cnt / 2) + 2));
        Texture* texture = new Texture(my_renderer, new_size, bg_color);
        my_renderer->setTarget(texture);
        my_renderer->drawRect({0, 0}, new_size - Vector2(1, 1), {0, 0, 0, 255});
        my_renderer->setTarget(NULL);
        panel->skin = new Skin(texture, new_size);
        // other buttons coords remain
    }
    btn_pos.setY(panel->absolute_pos.getY() + instr_cnt / 2 * (icons_size + px_interval) + px_interval + up_offset);
    btn_pos.setX(panel->absolute_pos.getX() + (instr_cnt % 2) * (icons_size + px_interval) + px_interval);
    Vector2 btn_sz(icons_size, icons_size);
    skin->resize(btn_sz);
    AbstractWindow* btn = dynamic_cast<AbstractWindow*>(new Button(btn_pos, btn_sz, btn_handler, panel, skin));
    btn_handler->setWindow(btn);
    // printf("created button %p\n", btn);
    attachWindow(btn);
}

AbstractInstrument* InstrumentPanel::getCurrentInstrument() {
    return current_instrument;
}

InstrumentPanel* InstrumentPanel::getInstance() {
    return panel;
}

void InstrumentPanel::setInstrument(AbstractInstrument* instrument) {
    printf("setting\n");
    current_instrument = instrument;
    printf("setted\n");
}

void InstrumentPanel::destroy() {
    // assert(is_created && (panel != nullptr));
    // delete panel;
}

InstrumentPanel::~InstrumentPanel() {
    // eh
};