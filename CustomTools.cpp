#include <cstdlib>
#include <cstring>
#include "CustomTools.h"

namespace plugin {

ISlider* MakeSliderWithTitle(int size_x, int size_y, int coord_x, int coord_y, int max_val, int min_val, char* title, 
                         IPreferencesPanel* panel, ISliderCallback* callback,
                         int title_size_x, int title_size_y, int ch_size) {

    ILabel* label = api->GetWidgetFactory()->CreateLabel(title_size_x, title_size_y, title, ch_size);
    panel->Attach(label, coord_x + size_x / 2 - title_size_x / 2, coord_y - title_size_y);
    
    ISlider* slider = api->GetWidgetFactory()->CreateSlider(size_x, size_y, max_val, min_val);
    slider->SetSliderCallback(callback);
    panel->Attach(slider, coord_x, coord_y);

    return slider;
};

ISlider* MakeSliderWithTitleMeasure(int size_x, int size_y, int coord_x, int coord_y, int max_val, int min_val, char* title, 
                                IPreferencesPanel* panel, ILabel* num_label, ISliderCallback* callback,
                                int title_size_x, int title_size_y, int ch_size) {
                             
    //ILabel* label = api->GetWidgetFactory()->CreateDefaultLabel(title);
    //panel->Attach(label, coord_x + size_x / 2 - title_size_x / 2, coord_y - label->GetSizeY());
    //panel->Attach(num_label, coord_x + size_x / 2 + title_size_x / 4, coord_y - title_size_y);

    ISlider* slider = api->GetWidgetFactory()->CreateSlider(size_x, size_y, max_val, min_val);
    slider->SetSliderCallback(callback);
    panel->Attach(slider, coord_x, coord_y);

    return slider;
};

ChangeShapeeCallback::ChangeShapeeCallback(ShapesDrawer* drawer, ITexture* color_texture, IIcon* icon, ShapeType type) : drawer(drawer), type(type), color_texture(color_texture), icon(icon) {};
void ChangeShapeeCallback::RespondOnClick() {
    drawer->SetShape(type);

    color_texture->Clear(0xFF'FF'FF'FF);
    drawer->ActionBegin(color_texture, color_texture->GetSizeX() / 2, color_texture->GetSizeY() / 2);

    icon->SetIcon(color_texture);
};

ChangeColorShapesPaletteCallback::ChangeColorShapesPaletteCallback(ShapesDrawer* tool, IIcon* icon, ITexture* color_texture, ISlider* red, ISlider* green, ISlider* blue) : 
    tool(tool), icon(icon), color_texture(color_texture), slider_red(red), slider_green(green), slider_blue(blue) {};
void ChangeColorShapesPaletteCallback::RespondOnChangeColor(color_t color) {
    tool->SetColor(color);

    //slider_red->SetValue  (color & 0x00'00'00'FF >> 0);
    //slider_green->SetValue(color & 0x00'00'FF'00 >> 8);
    //slider_blue->SetValue (color & 0x00'FF'00'00 >> 16);

    color_texture->Clear(0xFF'FF'FF'FF);
    tool->ActionBegin(color_texture, color_texture->GetSizeX() / 2, color_texture->GetSizeY() / 2);

    icon->SetIcon(color_texture);
};  

ChangeColorSlideCallback::ChangeColorSlideCallback(ShapesDrawer* drawer, ILabel* num_label, ITexture* color_texture, IIcon* icon, ColorComponent component) : drawer(drawer), num_label(num_label), component(component), color_texture(color_texture), icon(icon) {};
void ChangeColorSlideCallback::RespondOnSlide(float old_value, float current_value) {
    color_t color = drawer->GetColor();

    static char buf[5] = {};

    switch(component) {
        case COMPONENT_RED:
            color &= 0xFF'FF'FF'00;
            color |= static_cast<uint32_t>(current_value) << 0;
        break;

        case COMPONENT_GREEN:
            color &= 0xFF'FF'00'FF;
            color |= static_cast<uint32_t>(current_value) << 8;
        break;

        case COMPONENT_BLUE:
            color &= 0xFF'00'FF'FF;
            color |= static_cast<uint32_t>(current_value) << 16;
        break;

        default:
        break;
    }

    memset(buf, 0, 5);
    // itoa(static_cast<uint32_t>(current_value), buf, 10);
    snprintf(buf, 4, "%d", static_cast<uint32_t>(current_value));
    num_label->SetText(buf);

    drawer->SetColor(color);
    
    //color_texture->Clear(color);
    
    color_texture->Clear(0xFF'FF'FF'FF);
    drawer->ActionBegin(color_texture, color_texture->GetSizeX() / 2, color_texture->GetSizeY() / 2);

    icon->SetIcon(color_texture);
};

ChangeSizeSlideCallback::ChangeSizeSlideCallback(ShapesDrawer* drawer, ILabel* num_label, ITexture* color_texture, IIcon* icon) : drawer(drawer), num_label(num_label), icon(icon), color_texture(color_texture) {};
void ChangeSizeSlideCallback::RespondOnSlide(float old_value, float current_value) {
    drawer->SetSize(static_cast<int>(current_value));
    static char buf[4] = {};
    memset(buf, 0, 4);
    // itoa(static_cast<uint32_t>(current_value), buf, 10);
    snprintf(buf, 3, "%d", static_cast<uint32_t>(current_value));
    num_label->SetText(buf);

    color_texture->Clear(0xFF'FF'FF'FF);
    drawer->ActionBegin(color_texture, color_texture->GetSizeX() / 2, color_texture->GetSizeY() / 2);

    icon->SetIcon(color_texture);
};

TestCallback::TestCallback() {};
void TestCallback::RespondOnClick() { 
    std::cout << "click callback!\n";
};

TestSliderCallback::TestSliderCallback() {};
void TestSliderCallback::RespondOnSlide(float old_value, float current_value) {
    std::cout << "new value: " << current_value <<"\n";
};

Negative::Negative() {
    this->ConstructPreferencePanel();
};
void Negative::Apply(ITexture* canvas) {
    Buffer buf = canvas->ReadBuffer();
    
    //std::cout << "ApplyFilter\n";
    
    for (int i = 0; i < canvas->GetSizeY() * canvas->GetSizeX(); ++i) {
        color_t pixel = buf.pixels[i];
        pixel = 0xFF'FF'FF'FF - pixel;
        pixel |= 0xFF'00'00'00;
        buf.pixels[i] = pixel;
    }

    canvas->LoadBuffer(buf);
    canvas->ReleaseBuffer(buf);
};
const char* Negative::GetName() const {
    static const char* name = "Negative";
    return name;
};
IPreferencesPanel* Negative::GetPreferencesPanel() const {
    return nullptr;
};
void Negative::ConstructPreferencePanel() {
    panel = api->GetWidgetFactory()->CreatePreferencesPanel();

    IButton* text_button = api->GetWidgetFactory()->CreateButtonWithText(120, 30, "Press me", 10);
    text_button->SetClickCallback(new TestCallback());

    panel->Attach(text_button, panel->GetSizeX() / 2, panel->GetSizeY() / 2);
};

ShapesDrawer::ShapesDrawer() : type(TYPE_CIRCLE), size(1), color(0xFF'00'00'00) {
    this->ConstructPreferencePanel();
};
void ShapesDrawer::ActionBegin(ITexture* canvas, int x, int y) {
    //std::cout << "started drawing shapes!\n";
    switch (type) {
        case TYPE_CIRCLE:
            //std::cout << "Circle!\n";
            canvas->DrawCircle({x, y, size, 1, color, color});
        break;
    
        case TYPE_SQUARE:
            //std::cout << "Rect!\n";
            canvas->DrawRect({x - size / 2, y - size / 2, size, size, 1, color, color});
        break;

        default:
            std::cout << "unknown type: " << type << "\n";
        break;
    }    
};
void ShapesDrawer::SetSize(int new_size) { 
    size = new_size;
};
const char* ShapesDrawer::GetIconFileName() const {
    static const char* name = "skins/Untitled.bmp";
    return name;
};
const char* ShapesDrawer::GetName() const {
    static const char* name = "Shapes Drawer";
    return name;
};
IPreferencesPanel* ShapesDrawer::GetPreferencesPanel() const {
    return panel;
};
void ShapesDrawer::ConstructPreferencePanel() {
    IWidgetFactory* factory = api->GetWidgetFactory();
    
    panel = api->GetWidgetFactory()->CreatePreferencesPanel();

    ILabel* tool_name = api->GetWidgetFactory()->CreateLabel(170, 30, "Shapes drawer", 12);
    panel->Attach(tool_name, panel->GetSizeX() / 2 - 170 / 2, 0);

    Point coord();
    int button_x = 5 * panel->GetSizeX() / 8;
    int button_y = panel->GetSizeY() / 8;
    int dy = 10;
    int button_size_x = 80;
    int button_size_y = 30;
    
    IIcon* icon = api->GetWidgetFactory()->CreateIcon(100, 100);
    //panel->Attach(icon, panel->GetSizeX() / 12, panel->GetSizeY() / 5 + 210);

    ITexture* icon_texture = api->GetTextureFactory()->CreateTexture(100, 100);
    icon_texture->Clear(0xFF'FF'FF'FF);

    icon->SetIcon(icon_texture);

    IButton* circle_button = api->GetWidgetFactory()->CreateDefaultButtonWithText("Circle");
    circle_button->SetClickCallback(new ChangeShapeeCallback(this, icon_texture, icon, TYPE_CIRCLE));
    panel->Attach(circle_button, button_x, button_y);

    IButton* square_button = api->GetWidgetFactory()->CreateDefaultButtonWithText("Square");
    square_button->SetClickCallback(new ChangeShapeeCallback(this, icon_texture, icon, TYPE_SQUARE));
    panel->Attach(square_button, button_x, button_y + 1 * (button_size_y + dy));

    ILabel* red_count   = factory->CreateLabel(50, 30, "0", 10);
    ILabel* green_count = factory->CreateLabel(50, 30, "0", 10);
    ILabel* blue_count  = factory->CreateLabel(50, 30, "0", 10);
    ILabel* size_count  = factory->CreateLabel(50, 30, "1", 10);

    MakeSliderWithTitleMeasure(150, 15, panel->GetSizeX() / 12, panel->GetSizeY() / 5 + 0  , 1, 100, "size:",  panel, size_count,  new ChangeSizeSlideCallback (this, size_count,  icon_texture, icon));
    ISlider* slider_red = MakeSliderWithTitleMeasure(150, 15, panel->GetSizeX() / 12, panel->GetSizeY() / 5 + 60 , 0, 255, "red:",   panel, red_count,   new ChangeColorSlideCallback(this, red_count,   icon_texture, icon, COMPONENT_RED));
    ISlider* slider_green = MakeSliderWithTitleMeasure(150, 15, panel->GetSizeX() / 12, panel->GetSizeY() / 5 + 120, 0, 255, "green:", panel, green_count, new ChangeColorSlideCallback(this, green_count, icon_texture, icon, COMPONENT_GREEN));
    ISlider* slider_blue = MakeSliderWithTitleMeasure(150, 15, panel->GetSizeX() / 12, panel->GetSizeY() / 5 + 180, 0, 255, "blue:",  panel, blue_count,  new ChangeColorSlideCallback(this, blue_count,  icon_texture, icon, COMPONENT_BLUE));

    // IPalette* palette = api->GetWidgetFactory()->CreatePalette();
    // palette->SetPaletteCallback(new ChangeColorShapesPaletteCallback(this, icon, icon_texture, slider_red, slider_green, slider_blue));
    // panel->Attach(palette, button_x, button_y + 2 * (button_size_y + dy));

    // IIcon* icon_flower = api->GetWidgetFactory()->CreateIcon(30, 30);
    // panel->Attach(icon_flower, button_x, panel->GetSizeY() / 5 + 230);

    // ITexture* flower = api->GetTextureFactory()->CreateTexture("skins/titlebar2.bmp");
    // icon_flower->SetIcon(flower);
};

Plugin::Plugin(Negative* filter, ShapesDrawer* drawer) : filter(filter), drawer(drawer) {}
Filters Plugin::GetFilters() const {
    Filters filter_list;
    filter_list.count = 1;
    filter_list.filters = new IFilter*[filter_list.count];
    filter_list.filters[0] = filter;
    return filter_list;
};
Tools Plugin::GetTools () const {
    Tools tool_list;
    tool_list.count = 1;
    tool_list.tools = new ITool*[tool_list.count];
    tool_list.tools[0] = drawer;
    return tool_list;
};

}

extern "C" plugin::IPlugin* Create(plugin::IAPI* gui_api) {
  plugin::api = gui_api;
  return new plugin::Plugin(new plugin::Negative(), new plugin::ShapesDrawer());
}
extern "C" void Destroy(plugin::IPlugin* plugin) {
  delete plugin;
}
extern "C" uint32_t Version() {
  return plugin::kVersion;
}
