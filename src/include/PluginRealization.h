#ifndef INCLUDE_PLUGIN_REALIZATION
#define INCLUDE_PLUGIN_REALIZATION

#include "Window.h"
#include "Functors.h"
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

    Texture* getGlibTexture();
};

class APITextureFactory : public ITextureFactory {
public:
    virtual ~APITextureFactory() {}

    virtual APITexture* CreateTexture(const char* filename);
    virtual APITexture* CreateTexture(int32_t size_x, int32_t size_y);
};


class APIClickCallback : public Functor<> {
private:
    IClickCallback* callback;
public:
    APIClickCallback(IClickCallback* callback = nullptr) : callback(callback) {}
    virtual ~APIClickCallback() {}
    virtual bool operator()() {
        if (callback != nullptr) {
            callback->RespondOnClick();
        }
        return true;
    }
    void setCallback(IClickCallback* new_callback) {
        callback = new_callback;
    }
    // virtual void RespondOnClick();
};

class APISliderCallback : public Functor<float, float> {
    ISliderCallback* callback;
public:
    APISliderCallback(ISliderCallback* callback = nullptr) : callback(callback) {}
    virtual ~APISliderCallback() {}
    virtual bool operator()(float old_value, float new_value) {
        if (callback != nullptr) {
            callback->RespondOnSlide(old_value, new_value);
        }
        return true;
    }
    void setCallback(ISliderCallback* new_callback) {
        callback = new_callback;
    }
    // virtual void RespondOnSlide(float old_value, float current_value);
};

class APIPaletteCallback : public Functor<color_t> {
    IPaletteCallback* callback;
public:
    APIPaletteCallback(IPaletteCallback* callback = nullptr) : callback(callback) {}
    virtual ~APIPaletteCallback() {}
    virtual bool operator()(color_t color) {
        if (callback != nullptr) {
            callback->RespondOnChangeColor(color);
        }
        return true;
    }
    void setCallback(IPaletteCallback* new_callback) {
        callback = new_callback;
    }
    // virtual void RespondOnChangeColor(color_t color);  
};


class APIWidget : public IWidget {
protected:
    AbstractWindow* window;
public:
    APIWidget(AbstractWindow* window) : window(window) {}
    virtual ~APIWidget() {
        // delete window;
    }
    AbstractWindow* getGlibWindow() {
        return window;
    }
    virtual int32_t GetSizeX();
    virtual int32_t GetSizeY();
};

class APIButton : public IButton, public APIWidget {
    APIClickCallback* functor_callback;
public:
    APIButton(Button* window, APIClickCallback* callback) : APIWidget(window), functor_callback(callback) {
        assert(callback);
    };
    virtual void SetClickCallback(IClickCallback* callback);
    virtual int32_t GetSizeX() {return window->getSize().getX();};
    virtual int32_t GetSizeY() {return window->getSize().getY();};
};

class APISlider : public ISlider, public APIWidget {
    APISliderCallback* functor_callback;
public:
    // virtual ~APISlider() {}
    APISlider(SliderBody* window, APISliderCallback* callback) : APIWidget(window), functor_callback(callback) {
        assert(callback);
    };
    virtual void SetSliderCallback(ISliderCallback* callback);

    virtual float GetValue();
    virtual void SetValue(float value);
    virtual int32_t GetSizeX() {return window->getSize().getX();};
    virtual int32_t GetSizeY() {return window->getSize().getY();};
};

class APILabel : public ILabel, public APIWidget { // no functors or callbacks
public:
    // virtual ~APILabel() {}
    APILabel(TextLabel* window) : APIWidget(window) {};
    virtual void SetText(const char* text);
    virtual int32_t GetSizeX() {return window->getSize().getX();};
    virtual int32_t GetSizeY() {return window->getSize().getY();};
};

class APIIcon : public IIcon, public APIWidget { // no functors or callbacks
public:
    // virtual ~APIIcon() {}
    APIIcon(AbstractWindow* window) : APIWidget(window) {}; 
    virtual void SetIcon(const ITexture* icon);
    virtual int32_t GetSizeX() {return window->getSize().getX();};
    virtual int32_t GetSizeY() {return window->getSize().getY();};
};

class APIPalette : public IPalette, public APIWidget {
    APIPaletteCallback* functor_callback;
public:
    // virtual ~APIPalette() {}
    APIPalette(AbstractWindow* window, APIPaletteCallback* callback) : APIWidget(window), functor_callback(callback) {
        assert(callback);
    };
    virtual void SetPaletteCallback(IPaletteCallback* callback);
    virtual int32_t GetSizeX() {return window->getSize().getX();};
    virtual int32_t GetSizeY() {return window->getSize().getY();};
};

class APIPreferencesPanel : public IPreferencesPanel, public APIWidget {
public:
    virtual ~APIPreferencesPanel() {}
    APIPreferencesPanel(AbstractWindow* window) : APIWidget(window) {};
    virtual void Attach(IButton*  button,  int32_t x, int32_t y) override;
    virtual void Attach(ILabel*   label,   int32_t x, int32_t y) override;
    virtual void Attach(ISlider*  slider,  int32_t x, int32_t y) override;
    virtual void Attach(IIcon*    icon,    int32_t x, int32_t y) override;
    virtual void Attach(IPalette* palette, int32_t x, int32_t y) override;
    virtual int32_t GetSizeX() {return window->getSize().getX();};
    virtual int32_t GetSizeY() {return window->getSize().getY();};
};

class APIWidgetFactory : public IWidgetFactory {
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
    API() {};
    // virtual ~API();

    virtual IWidgetFactory*  GetWidgetFactory() override;
    virtual ITextureFactory* GetTextureFactory() override;
};

// class APIFilter : IFilter {
//   public:
//     virtual ~APIFilter() {}

//     virtual void Apply(ITexture* canvas);
//     virtual const char* GetName() const;

//     virtual APIPreferencesPanel* GetPreferencesPanel() const;
// };

// class APITool : ITool {
//   public:
//     virtual ~APITool() {}

//     virtual void ActionBegin(ITexture* canvas, int x, int y);
//     virtual void Action     (ITexture* canvas, int x, int y, int dx, int dy);
//     virtual void ActionEnd  (ITexture* canvas, int x, int y);

//     virtual const char* GetIconFileName() const;
//     virtual const char* GetName() const;

//     virtual APIPreferencesPanel* GetPreferencesPanel() const;
// };


#endif