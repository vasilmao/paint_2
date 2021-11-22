#ifndef RENDERER_INCLUDE
#define RENDERER_INCLUDE

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "CoordSystem.h"
#include "Tools.h"

enum SystemEventTypes {
    NO_EVENT,
    OTHER_EVENTS,
    MOUSE_CLICK,
    MOUSE_MOVE,
    QUIT_EVENT,
};

struct SystemMouseButton {
    Vector2 pos;
    uint8_t button_type;
    bool button_down;
};

struct SystemBigEvent {
    char data[64];
};

struct SystemMouseMove {
    Vector2 prev_pos;
    Vector2 new_pos;
};

struct SystemEvent {
    int event_type;
    union
    {
        SystemMouseButton mb_press_info;
        SystemMouseMove mouse_move;
        SystemBigEvent big_event;
    };
    SystemEvent(){}
    SystemEvent(const SystemEvent& other_event){
        event_type = other_event.event_type;
        for (int i = 0; i < 64; ++i) {
            big_event.data[i] = other_event.big_event.data[i];
        }
    }
    ~SystemEvent(){}
};

SystemEvent getSystemEvent();

//--------------------------------------------------------------------------------------------------------------------------------

class Renderer;

class Texture {
private:
    // friend class Renderer;
    SDL_Texture* sdl_texture;
    Vector2 size;
public:
    Texture();
    ~Texture();
    Texture(Renderer* renderer, const Vector2& size);
    Texture(Renderer* renderer, const Vector2& size, Color color);
    Texture(Renderer* renderer, const char* filename);
    const Vector2& getSize() const;
    SDL_Texture* getNativeTexture();
    bool hitTest(const Vector2& pos) const;
};

Texture* createSign(char* text, const Vector2& size);

//--------------------------------------------------------------------------------------------------------------------------------

class Font {
private:
    TTF_Font* sdl_font;
    int size;
public:
    Font();
    ~Font();
    Font(int size);
    Font(const char* filename, int size);
    int getSize() const;
    TTF_Font* getNativeFont();
};

//--------------------------------------------------------------------------------------------------------------------------------

class Renderer {
private:
    SDL_Window* window     = NULL;
    SDL_Renderer* renderer = NULL;
    // TTF_Font* font         = NULL;
    // SDL_Surface* surface   = NULL;

    // int32_t* pixels = NULL;
    int width = 0;
    int height = 0;

    Color bg_color;

    Font* default_font;

    Texture* current_texture = nullptr;

public:

    Renderer(){}

    ~Renderer();
    Renderer(int width, int height, Color bg_color);
    void drawFilledCircle(const Vector2& center, const float r, Color color);
    void drawRect(const Vector2& p1, const Vector2& size, Color color);
    void drawFilledRect(const Vector2& p1, const Vector2& size, Color color);
    void drawSegment(const Vector2& p1, const Vector2& p2, Color color);
    void drawLine(const Vector2& p1, const Vector2& p2, Color color);
    void drawThickLine(const Vector2& p1, const Vector2& p2, Color color) const;
    void render();
    void drawText(const Vector2& pos, const Vector2& size, const char* text, Color color);
    void drawTextCentered(const Vector2& pos, const Vector2& size, const char* text, Color color);
    void drawText(const Vector2& pos, const char* text, Color color);
    void setTarget(Texture* texture);
    void copyTexture(Texture* texture, const Vector2& pos);
    void copyTexture(Texture* texture, const Vector2& pos, const Vector2& dst_size);
    SDL_Renderer* getNativeRenderer();
};

//--------------------------------------------------------------------------------------------------------------------------------

#endif