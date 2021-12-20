#ifndef INCLUDE_PLUGIN_REALIZATION
#define INCLUDE_PLUGIN_REALIZATION

#include "Window.h"
#include "plugin.hpp"

using namespace plugin;


class APITexture : public ITexture {
private:
    Texture* texture;
public:
    APITexture(Texture* texture) : texture(texture) {}

    virtual ~APITexture();

    virtual int32_t GetSizeX();
    virtual int32_t GetSizeY();

    virtual Buffer ReadBuffer();
    virtual void ReleaseBuffer(Buffer buffer);
    virtual void LoadBuffer(Buffer buffer);

    virtual void Clear(color_t color);
    virtual void Present();

    virtual void DrawLine  (const Line& line);
    virtual void DrawCircle(const Circle& circle);
    virtual void DrawRect  (const Rect& rect);

    virtual void CopyTexture(ITexture* source, int32_t x, int32_t y, int32_t size_x, int32_t size_y) override ;
    virtual void CopyTexture(ITexture* source, int32_t x, int32_t y) override;
};

class APITextureFactory : public ITextureFactory {
public:
    virtual ~APITextureFactory() {}

    virtual APITexture* CreateTexture(const char* filename);
    virtual APITexture* CreateTexture(int32_t size_x, int32_t size_y);
};


class APIClickCallback : public IClickCallback, public Functor<> {
public:
    virtual ~APIClickCallback() {}
    virtual bool operator()() {
        RespondOnClick();
        return true;
    }
    // virtual void RespondOnClick();
};

class APISliderCallback : public ISliderCallback, public Functor<float, float> {
public:
    virtual ~APISliderCallback() {}
    virtual bool operator()(float old_value, float new_value) {
        RespondOnSlide(old_value, new_value);
        return true;
    }
    // virtual void RespondOnSlide(float old_value, float current_value);
};

class APIPaletteCallback : public IPaletteCallback, public Functor<color_t> {
public:
    virtual ~APIPaletteCallback() {}
    virtual bool operator()(color_t color) {
        RespondOnChangeColor(color);
        return true;
    }
    // virtual void RespondOnChangeColor(color_t color);  
};

// up is done

class APIWidget : public IWidget {
protected:
    AbstractWindow* window;
public:
    APIWidget() : window(nullptr) {}
    virtual ~APIWidget() {
        delete window;
    }

    virtual int32_t GetSizeX();
    virtual int32_t GetSizeY();
};

class APIButton : public IButton, public APIWidget {
public:
    virtual void SetClickCallback(IClickCallback* callback);
};

class APISlider : public ISlider {
  public:
    // virtual ~APISlider() {}

    virtual void SetSliderCallback(APISliderCallback* callback);

    virtual float GetValue();
    virtual void SetValue(float value);
};

class APILabel : public ILabel, public APIWidget {
  public:
    // virtual ~APILabel() {}

    virtual void SetText(const char* text);
};

class APIIcon : public IIcon, public APIWidget {
  public:
    // virtual ~APIIcon() {}

    virtual void SetIcon(const ITexture* icon);
};

class APIPalette : public IPalette, public APIWidget {
  public:
    // virtual ~APIPalette() {}

    virtual void SetPaletteCallback(APIPaletteCallback* callback);
};

class APIPreferencesPanel : public IPalette, public APIWidget {
  public:
    virtual ~APIPreferencesPanel() {}

    virtual void Attach(APIButton*  button,  int32_t x, int32_t y);
    virtual void Attach(APILabel*   label,   int32_t x, int32_t y);
    virtual void Attach(APISlider*  slider,  int32_t x, int32_t y);
    virtual void Attach(APIIcon*    icon,    int32_t x, int32_t y);
    virtual void Attach(APIPalette* palette, int32_t x, int32_t y);
};

class APIWidgetFactory {
  public:
    virtual ~APIWidgetFactory() {}

    virtual APIButton* CreateDefaultButtonWithIcon(const char* icon_file_name);
    virtual APIButton* CreateDefaultButtonWithText(const char* text);
    virtual APIButton* CreateButtonWithIcon(int32_t size_x, int32_t size_y, const char* icon_file_name);
    virtual APIButton* CreateButtonWithText(int32_t size_x, int32_t size_y, const char* text, int32_t char_size);

    virtual APISlider* CreateDefaultSlider(float range_min, float range_max);
    virtual APISlider* CreateSlider(int32_t size_x, int32_t size_y, float range_min, float range_max);

    virtual APILabel*  CreateDefaultLabel(const char* text);
    virtual APILabel*  CreateLabel(int32_t size_x, int32_t size_y, const char* text, int32_t char_size);

    virtual APIIcon*   CreateIcon(int32_t size_x, int32_t size_y);

    virtual APIPalette* CreatePalette();

    virtual APIPreferencesPanel* CreatePreferencesPanel();
};

class API : public IAPI {
public:
    API();
    // virtual ~API();

    virtual IWidgetFactory*  GetWidgetFactory() override;
    virtual ITextureFactory* GetTextureFactory() override;
};

class APIFilter {
  public:
    virtual ~APIFilter() {}

    virtual void Apply(ITexture* canvas);
    virtual const char* GetName() const;

    virtual APIPreferencesPanel* GetPreferencesPanel() const;
};

class APITool {
  public:
    virtual ~APITool() {}

    virtual void ActionBegin(ITexture* canvas, int x, int y);
    virtual void Action     (ITexture* canvas, int x, int y, int dx, int dy);
    virtual void ActionEnd  (ITexture* canvas, int x, int y);

    virtual const char* GetIconFileName() const;
    virtual const char* GetName() const;

    virtual APIPreferencesPanel* GetPreferencesPanel() const;
};


#endif