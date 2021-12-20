#define EXPORT_TOOL
#include <iostream>
#include <stdlib.h>
#include "src/include/plugin.hpp"

struct Point {
    int x;
    int y;

    Point(int x, int y) : x(x), y(y) {};
};

namespace plugin {

class ShapesDrawer;
class Negative;

enum ShapeType {
    TYPE_SQUARE,
    TYPE_CIRCLE
};

enum ColorComponent {
    COMPONENT_RED,
    COMPONENT_GREEN,
    COMPONENT_BLUE
};

IAPI* api = nullptr;

ISlider* MakeSliderWithTitle(int size_x, int size_y, int coord_x, int coord_y, int max_val, int min_val, char* title, 
                         IPreferencesPanel* panel, ISliderCallback* callback,
                         int title_size_x = 80, int title_size_y = 30, int ch_size = 10);

ISlider* MakeSliderWithTitleMeasure(int size_x, int size_y, int coord_x, int coord_y, int max_val, int min_val, char* title, 
                         IPreferencesPanel* panel, ILabel* num_label, ISliderCallback* callback,
                         int title_size_x = 80, int title_size_y = 30, int ch_size = 10);

class ChangeShapeeCallback : public IClickCallback {
  public:
    ChangeShapeeCallback(ShapesDrawer* drawer, ITexture* color_texture, IIcon* icon, ShapeType type);
    virtual ~ChangeShapeeCallback() {};

    virtual void RespondOnClick() override;

  private:
    ShapesDrawer* drawer;
    ShapeType type;
    IIcon* icon;
    ITexture* color_texture;
};

class ChangeColorSlideCallback : public ISliderCallback {
  public:
    ChangeColorSlideCallback(ShapesDrawer* drawer, ILabel* num_label, ITexture* color_texture, IIcon* icon, ColorComponent component);
    virtual ~ChangeColorSlideCallback() {};

    virtual void RespondOnSlide(float old_value, float current_value) override;

  private:
    ShapesDrawer* drawer;
    ILabel* num_label;
    ITexture* color_texture;
    IIcon* icon;
    ColorComponent component;
};

class ChangeColorShapesPaletteCallback : public IPaletteCallback {
  public:
    ChangeColorShapesPaletteCallback(ShapesDrawer* tool, IIcon* icon, ITexture* color_texture, ISlider* red, ISlider* green, ISlider* blue);
    virtual ~ChangeColorShapesPaletteCallback() {}

    virtual void RespondOnChangeColor(color_t color) override;  

  private:
    ISlider* slider_red;
    ISlider* slider_green;
    ISlider* slider_blue;
    ShapesDrawer* tool;
    IIcon* icon;
    ITexture* color_texture;
};

class ChangeSizeSlideCallback : public ISliderCallback {
  public:
    ChangeSizeSlideCallback(ShapesDrawer* drawer, ILabel* num_label, ITexture* color_texture, IIcon* icon);
    virtual ~ChangeSizeSlideCallback() {};

    virtual void RespondOnSlide(float old_value, float current_value) override;

  private:
    ShapesDrawer* drawer;
    IIcon* icon;
    ITexture* color_texture;
    ILabel* num_label;
};

class TestCallback : public IClickCallback {
  public:
    TestCallback();
    virtual ~TestCallback() {}

    virtual void RespondOnClick() override;
  private:
};

class TestSliderCallback : public ISliderCallback {
  public:
    TestSliderCallback();
    virtual ~TestSliderCallback() {};

    virtual void RespondOnSlide(float old_value, float current_value) override;
};

class Negative : public IFilter {
  public:
    Negative();
    virtual ~Negative() {};

    virtual void Apply(ITexture* canvas) override;
    virtual const char* GetName() const override;

    virtual IPreferencesPanel* GetPreferencesPanel() const override;

    void ConstructPreferencePanel();

  private:
    IPreferencesPanel* panel;
};

class ShapesDrawer : public ITool {
  public:
    ShapesDrawer();
    virtual ~ShapesDrawer() {};

    virtual void ActionBegin(ITexture* canvas, int x, int y) override;

    void SetSize(int new_size);
    color_t GetColor() const { return color; };
    void SetColor(color_t new_color) { color = new_color; };
    void SetShape(ShapeType new_type) { type = new_type; };


    virtual void Action     (ITexture* canvas, int x, int y, int dx, int dy) override {};
    virtual void ActionEnd  (ITexture* canvas, int x, int y) override {};

    virtual const char* GetIconFileName() const override;
    virtual const char* GetName() const override;

    virtual IPreferencesPanel* GetPreferencesPanel() const override;

    void ConstructPreferencePanel();

  private:
    IPreferencesPanel* panel;
    ShapeType type;
    int size;
    color_t color;
};

class Plugin : public IPlugin {
  public:
    Plugin(Negative* filter, ShapesDrawer* drawer);
    ~Plugin() {};

    virtual Filters GetFilters() const override;

    virtual Tools GetTools () const override;

  private:
    Negative* filter;
    ShapesDrawer* drawer;
};

}

// extern "C" TOOLAPI plugin::IPlugin* TOOLCALL Create(plugin::IAPI* gui_api) {
//     plugin::api = gui_api;
//     return new plugin::Plugin(new plugin::Negative(), new plugin::ShapesDrawer());
// };

// extern "C" TOOLAPI void TOOLCALL Destroy(plugin::IPlugin* plugin) {
//     delete plugin;
// };

// extern "C" TOOLAPI uint32_t TOOLCALL Version() {
//     return plugin::kVersion;
// };




#undef EXPORT_TOOL