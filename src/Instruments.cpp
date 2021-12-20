#include "Instruments.h"
#include "Window.h"
#include "Handlers.h"
#include "PluginRealization.h"

AbstractWindow* Brush::createPrefPanel(Renderer* renderer, const Vector2& pos, AbstractWindow* parent) {
    printf("creating panel!\n");
    float maxr = 20;
    Texture* preview_texture = new Texture(renderer, Vector2{240, 280}, color);
    Texture* r_preview_texture = new Texture(renderer, Vector2{(maxr + 1) * 2, (maxr + 1) * 2}, {0, 0, 0, 0});
    Color r_preview_color;
    if (color.r + color.g + color.b <= 20) {
        r_preview_color = {255, 255, 255, 255};
    } else {
        r_preview_color = {0, 0, 0, 255};
    }
    renderer->setTarget(r_preview_texture);
    renderer->drawCircle(r_preview_texture->getSize() * 0.5, r, r_preview_color);
    // renderer->drawCircle(r_preview_texture->getSize() * 0.5, r - 1, r_preview_color);
    // renderer->setTarget(nullptr);
    // renderer->copyTexture(r_preview_texture, Vector2{0, 0});
    // renderer->render();
    // SDL_Delay(3000);
    // exit(0);
    SliderBody* r_slider = new SliderBody(renderer, pos + Vector2{20, 20}, Vector2{20, 200},
                                          new ChangeBrushRColorFunctor(renderer, this, preview_texture, r_preview_texture), 0, 255, color.r);
    SliderBody* g_slider = new SliderBody(renderer, pos + Vector2{60, 20}, Vector2{20, 200},
                                          new ChangeBrushGColorFunctor(renderer, this, preview_texture, r_preview_texture), 0, 255, color.g);
    SliderBody* b_slider = new SliderBody(renderer, pos + Vector2{100, 20}, Vector2{20, 200},
                                          new ChangeBrushBColorFunctor(renderer, this, preview_texture, r_preview_texture), 0, 255, color.b);
    SliderBody* radius_slider = new SliderBody(renderer, pos + Vector2{20, 240}, Vector2{200, 20},
                                               new ChangeBrushRFunctor(renderer, this, preview_texture, r_preview_texture), 1, maxr, r);
    
    printf("sliders done\n");
    AbstractWindow* pref_panel = new AbstractWindow(
        pos,
        Vector2{240, 280},
        nullptr,
        parent,
        new Skin(preview_texture)
    );
    printf("window created\n");
    pref_panel->setHandler(new SensitiveHandler(pref_panel));
    AbstractWindow* dumb_preview = new AbstractWindow(
        pos + Vector2{130, 20},
        r_preview_texture->getSize(),
        nullptr,
        pref_panel,
        new Skin(r_preview_texture)
    );
    dumb_preview->setHandler(new EventHandler(dumb_preview));
    printf("handler set\n");
    pref_panel->attachWindow(r_slider);
    pref_panel->attachWindow(g_slider);
    pref_panel->attachWindow(b_slider);
    pref_panel->attachWindow(radius_slider);
    pref_panel->attachWindow(dumb_preview);
    printf("sliders attached\n");
    // InstrumentPanel::getInstance()->getParent()->attachWindow(pref_panel);
    // printf("window attached\n");
    printf("STUPID WINDOW IS %p\n", pref_panel);
    printf("DUMB PREVIEW  IS %p\n", dumb_preview);
    return pref_panel;
}

AbstractWindow* Eraser::createPrefPanel(Renderer* renderer, const Vector2& pos, AbstractWindow* parent) {
    float maxr = 20;
    Texture* r_preview_texture = new Texture(renderer, Vector2{(maxr + 1) * 2, (maxr + 1) * 2}, {0, 0, 0, 0});
    Color r_preview_color = {0, 0, 0, 255};
    renderer->setTarget(r_preview_texture);
    renderer->drawCircle(r_preview_texture->getSize() * 0.5, r, r_preview_color);
    Texture* transp_texture = new Texture(renderer, "no-transparent_160.bmp", true);
    AbstractWindow* pref_panel = new AbstractWindow(
        pos,
        Vector2{200 + maxr * 2, maxr * 2 + 20},
        nullptr,
        parent,
        // new Skin(new Texture(renderer, Vector2{240 + maxr * 2, maxr * 2 + 20}, {255, 255, 255, 255}))
        new RepeatingSkin(transp_texture, Vector2{240 + maxr * 2, maxr * 2 + 20})
    );
    pref_panel->setHandler(new SensitiveHandler(pref_panel));
    SliderBody* radius_slider = new SliderBody(renderer, pos + Vector2{0, maxr}, Vector2{200, 20},
                                               new ChangeBrushRFunctor(renderer, this, nullptr, r_preview_texture), 1, maxr, r);
    AbstractWindow* dumb_preview = new AbstractWindow(
        pos + Vector2{210, 10},
        r_preview_texture->getSize(),
        nullptr,
        pref_panel,
        new Skin(r_preview_texture)
    );
    dumb_preview->setHandler(new EventHandler(dumb_preview));
    pref_panel->attachWindow(radius_slider);
    pref_panel->attachWindow(dumb_preview);
    return pref_panel;
}

void ToolInstrument::apply(Renderer* renderer, Texture* texture, const Vector2& pos) {
    assert(0);
}
void ToolInstrument::applyStart(Renderer* renderer, Texture* texture, const Vector2& pos) {
    APITexture pl_texture(texture);
    plugin_tool->ActionBegin(&pl_texture, static_cast<int>(pos.getX()), static_cast<int>(pos.getY()));
}
void ToolInstrument::applyMove(Renderer* renderer, Texture* texture, const Vector2& pos1, const Vector2& pos2) {
    APITexture pl_texture(texture);
    plugin_tool->Action(&pl_texture, static_cast<int>(pos1.getX()), static_cast<int>(pos1.getY()), static_cast<int>((pos2-pos1).getX()), static_cast<int>((pos2-pos1).getY()));
}
void ToolInstrument::applyEnd(Renderer* renderer, Texture* texture, const Vector2& pos) {
    APITexture pl_texture(texture);
    plugin_tool->ActionEnd(&pl_texture, static_cast<int>(pos.getX()), static_cast<int>(pos.getY()));
}
AbstractWindow* ToolInstrument::createPrefPanel(Renderer* renderer, const Vector2& pos, AbstractWindow* parent) {
    return dynamic_cast<APIPreferencesPanel*>(plugin_tool->GetPreferencesPanel())->getGlibWindow();
}
