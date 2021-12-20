#include "GraphicLib.h"
#include "Window.h"

const char* TextureManager::skins_dir;

const char* TextureManager::getSkinsDir() {
    return skins_dir;
}

void TextureManager::setSkinsDir(const char* new_path) {
    skins_dir = new_path;
}

void TextureManager::setDay(Renderer* renderer) {
    for (size_t i = 0; i < textures.size(); ++i) {
        std::string realpath = "skins/light/" + textures[i].second;
        textures[i].first->reloadFromFile(renderer, realpath.c_str());
    }
    skins_dir = "skins/light/";
}
void TextureManager::setNight(Renderer* renderer) {
    for (size_t i = 0; i < textures.size(); ++i) {
        std::string realpath = "skins/dark/" + textures[i].second;
        textures[i].first->reloadFromFile(renderer, realpath.c_str());
    }
    skins_dir = "skins/dark/";
}
void TextureManager::deleteTexture(Texture* texture) {
    for (size_t i = 0; i < textures.size(); ++i) {
        if (textures[i].first == texture) {
            textures.erase(textures.begin() + i);
            break;
        }
    }
}

Texture::Texture() {
    sdl_texture = NULL;
}

Texture::Texture(Renderer* renderer, const Vector2& size) : size(size) {
    sdl_texture = SDL_CreateTexture(renderer->getNativeRenderer(),
                                    SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET,
                                    size.getX(),
                                    size.getY()
                                    );
    SDL_SetTextureBlendMode(sdl_texture, SDL_BLENDMODE_BLEND);
}

Texture::Texture(Renderer* renderer, const Vector2& size, Color color) : size(size) {
    sdl_texture = SDL_CreateTexture(renderer->getNativeRenderer(),
                                    SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET,
                                    size.getX(),
                                    size.getY()
                                    );
    renderer->setTarget(this);
    SDL_SetRenderDrawColor(renderer->getNativeRenderer(), open_color(color));
    SDL_RenderClear(renderer->getNativeRenderer());
    SDL_FRect rect = {0, 0, size.getX(), size.getY()};
    SDL_RenderDrawRectF(renderer->getNativeRenderer(), &rect);
    SDL_RenderFillRectF(renderer->getNativeRenderer(), &rect);
    SDL_SetTextureBlendMode(sdl_texture, SDL_BLENDMODE_BLEND);
}

Texture::Texture(Renderer* renderer, const char* filename, bool themeTexture) {
    if (!themeTexture) {
        SDL_Surface* surf = SDL_LoadBMP(filename);
        if (!surf) {
            printf("img not opened\n");
            printf("%s\n", filename);
        }
        assert(surf);
        size = {static_cast<float>(surf->w), static_cast<float>(surf->h)};
        SDL_Texture* bmp_texture = SDL_CreateTextureFromSurface(
            renderer->getNativeRenderer(),
            surf
        ); 
        sdl_texture = SDL_CreateTexture(renderer->getNativeRenderer(),
                                        SDL_PIXELFORMAT_RGBA8888,
                                        SDL_TEXTUREACCESS_TARGET,
                                        size.getX(),
                                        size.getY()
                                        );
        assert(sdl_texture);
        SDL_SetRenderTarget(renderer->getNativeRenderer(), sdl_texture);
        SDL_RenderCopy(renderer->getNativeRenderer(), bmp_texture, NULL, NULL);
        SDL_DestroyTexture(bmp_texture);
        SDL_FreeSurface(surf);
        SDL_SetTextureBlendMode(sdl_texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(renderer->getNativeRenderer(), NULL);
    } else {
        char str[100] = {};
        strcpy(str, TextureManager::getSkinsDir());
        strcat(str, filename);
        TextureManager::addTexture(this, filename);
        SDL_Surface* surf = SDL_LoadBMP(str);
        if (!surf) {
            printf("img not opened\n");
            printf("%s\n", str);
        }
        assert(surf);
        size = {static_cast<float>(surf->w), static_cast<float>(surf->h)};
        SDL_Texture* bmp_texture = SDL_CreateTextureFromSurface(
            renderer->getNativeRenderer(),
            surf
        ); 
        sdl_texture = SDL_CreateTexture(renderer->getNativeRenderer(),
                                        SDL_PIXELFORMAT_RGBA8888,
                                        SDL_TEXTUREACCESS_TARGET,
                                        size.getX(),
                                        size.getY()
                                        );
        assert(sdl_texture);
        SDL_SetRenderTarget(renderer->getNativeRenderer(), sdl_texture);
        SDL_RenderCopy(renderer->getNativeRenderer(), bmp_texture, NULL, NULL);
        SDL_DestroyTexture(bmp_texture);
        SDL_FreeSurface(surf);
        SDL_SetTextureBlendMode(sdl_texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(renderer->getNativeRenderer(), NULL);
    }
}

Texture::~Texture() {
    SDL_DestroyTexture(sdl_texture);
    TextureManager::deleteTexture(this);
}

void Texture::reloadFromFile(Renderer* renderer, const char* path) {
    SDL_DestroyTexture(sdl_texture);
    SDL_Surface* surf = SDL_LoadBMP(path);
    if (!surf) {
        printf("img not opened\n");
    }
    assert(surf);
    size = {static_cast<float>(surf->w), static_cast<float>(surf->h)};
    SDL_Texture* bmp_texture = SDL_CreateTextureFromSurface(
        renderer->getNativeRenderer(),
        surf
    ); 
    sdl_texture = SDL_CreateTexture(renderer->getNativeRenderer(),
                                    SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET,
                                    size.getX(),
                                    size.getY()
                                    );
    assert(sdl_texture);
    SDL_SetRenderTarget(renderer->getNativeRenderer(), sdl_texture);
    SDL_RenderCopy(renderer->getNativeRenderer(), bmp_texture, NULL, NULL);
    SDL_DestroyTexture(bmp_texture);
    SDL_FreeSurface(surf);
    SDL_SetTextureBlendMode(sdl_texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer->getNativeRenderer(), NULL);
}

uint32_t* Texture::copyIntBuffer() {
    uint32_t* buffer = new uint32_t[static_cast<size_t>(size.getX() * size.getY())];
    assert(buffer);

    Renderer* renderer = InstrumentPanel::getInstance()->getRenderer(); // BRUH, TODO: FIX
    renderer->setTarget(this);
    assert(!SDL_RenderReadPixels(renderer->getNativeRenderer(), NULL, SDL_PIXELFORMAT_RGBA8888, buffer, size.getX() * sizeof(Color)));

    return buffer;
}

void Texture::updatePixels(uint32_t* pixels) {
    assert(!SDL_UpdateTexture(sdl_texture, NULL, pixels, size.getX() * sizeof(Color)));
}

const Vector2& Texture::getSize() const {
    return size;
}

SDL_Texture* Texture::getNativeTexture() {
    return sdl_texture;
}


Font::Font() {
    sdl_font = NULL;
    size = 0;
}

Font::~Font() {
    if (sdl_font != NULL) {
        TTF_CloseFont(sdl_font);
    }
}

Font::Font(int size) : size(size) {
    // printf("%d\n", size);
    sdl_font = TTF_OpenFont("font.ttf", size);
    // printf("font: %p\n", sdl_font);
    assert(sdl_font);
}

Font::Font(const char* filename, int size) : size(size) {
    sdl_font = TTF_OpenFont(filename, size);
}

int Font::getSize() const {
    return size;
}

TTF_Font* Font::getNativeFont() {
    return sdl_font;
}


Renderer::Renderer(int width, int height, Color bg_color) : width(width), height(height), bg_color(bg_color) {
    SDL_Init(SDL_INIT_EVERYTHING);
    int rendererFlags = SDL_RENDERER_ACCELERATED;
    int windowFlags = 0;
    window = SDL_CreateWindow("Patcher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, windowFlags);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");


    renderer = SDL_CreateRenderer(window, -1, rendererFlags);

    

    SDL_SetRenderTarget(renderer, NULL);

    // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, open_color(bg_color));
    // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderClear(renderer);


    TTF_Init();
    // font = TTF_OpenFont("Anonymous.ttf", 10);
    // assert(font);
    default_font = new Font(30);
}

Renderer::~Renderer() {
    // TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    delete default_font;
    TTF_Quit();
    // SDL_Quit();
    printf("renderer destroyed!\n");
}

void Renderer::drawPoint(const Vector2& point, Color color) {
    SDL_SetRenderDrawColor(renderer, open_color(color));
    SDL_RenderDrawPointF(renderer, point.getX(), point.getY());
}

void Renderer::drawCircle(const Vector2& center, const float r, Color color) {
    SDL_SetRenderDrawColor(renderer, open_color(color));
    float r2 = r * r;
    Vector2 center_point(center);
    Vector2 circle_quadr_diagonal(r, r);
    center_point -= circle_quadr_diagonal;
    Vector2 pixel_left_up_point = center_point;

    center_point += circle_quadr_diagonal;
    center_point += circle_quadr_diagonal;
    Vector2 pixel_right_down_point = center_point;
    for (float px = pixel_left_up_point.getX(); px <= pixel_right_down_point.getX(); px += 1.0) {
        float x_c = (px - center.getX());
        float y1_c = sqrt(r2 - x_c * x_c);
        float py1 = y1_c + center.getY();
        SDL_RenderDrawPointF(renderer, px, py1);
        float y2_c = -sqrt(r2 - x_c * x_c);
        float py2 = y2_c + center.getY();
        SDL_RenderDrawPointF(renderer, px, py2);
    }

    for (float py = pixel_left_up_point.getY(); py <= pixel_right_down_point.getY(); py += 1.0) {
        float y_c = (py - center.getY());
        float x1_c = sqrt(r2 - y_c * y_c);
        float px1 = x1_c + center.getX();
        SDL_RenderDrawPointF(renderer, px1, py);
        float x2_c = -sqrt(r2 - y_c * y_c);
        float px2 = x2_c + center.getX();
        SDL_RenderDrawPointF(renderer, px2, py);
    }
}

void Renderer::drawFilledCircle(const Vector2& center, const float r, Color color) {
    SDL_SetRenderDrawColor(renderer, open_color(color));
    float r2 = r * r;
    Vector2 center_point(center);
    Vector2 circle_quadr_diagonal(r, r);
    center_point -= circle_quadr_diagonal;
    Vector2 pixel_left_up_point = center_point;

    center_point += circle_quadr_diagonal;
    center_point += circle_quadr_diagonal;
    Vector2 pixel_right_down_point = center_point;
    for (int py = pixel_left_up_point.getY(); py <= pixel_right_down_point.getY(); ++py) {
        for (int px = pixel_left_up_point.getX(); px <= pixel_right_down_point.getX(); ++px) {
            Vector2 coord_point = {static_cast<float>(px), static_cast<float>(py)};
            float x_c = (coord_point.getX() - center.getX());
            float y_c = (coord_point.getY() - center.getY()); 
            if (x_c * x_c + y_c * y_c <= r2) {
                SDL_RenderDrawPointF(renderer, px, py);
            }
        }
    }
}

void Renderer::drawRect(const Vector2& p1, const Vector2& size, Color color) {
    SDL_FRect rect = {p1.getX(), p1.getY(), size.getX(), size.getY()};
    SDL_SetRenderDrawColor(renderer, open_color(color));
    SDL_RenderDrawRectF(renderer, &rect);
}

void Renderer::drawFilledRect(const Vector2& p1, const Vector2& size, Color color) {
    SDL_SetRenderDrawColor(renderer, open_color(color));
    // printf("FILLED RECT!! %f, %f, %f, %f\n(%d, %d, %d, %d)\n", p1.getX(), p1.getY(), size.getX(), size.getY(), (int)color.r, (int)color.g, (int)color.b, (int)color.a);
    // printf("right on %p, %p\n", current_texture, current_texture->getNativeTexture());
    // printf("%p\n", SDL_GetRenderTarget(renderer));
    for (int x = p1.getX(); x <= (p1 + size).getX(); ++x) {
        SDL_RenderDrawLineF(renderer, x, p1.getY(), x, (p1 + size).getY());
    }
    // Texture* ct = current_texture;
    // exit(0);
}

void Renderer::drawSegment(const Vector2& p1, const Vector2& p2, Color color) {
    SDL_SetRenderDrawColor(renderer, open_color(color));
    SDL_RenderDrawLineF(renderer, p1.getX(), p1.getY(), p2.getX(), p2.getY());
}

void Renderer::drawLine(const Vector2& p1, const Vector2& p2, Color color) {
    SDL_SetRenderDrawColor(renderer, open_color(color));
    SDL_RenderDrawLineF(renderer, p1.getX(), p1.getY(), p2.getX(), p2.getY());
}

void Renderer::drawThickLine(const Vector2& p1, const Vector2& p2, Color color) const {
    SDL_SetRenderDrawColor(renderer, open_color(color));
    SDL_RenderDrawLineF(renderer, p1.getX(),     p1.getY(),     p2.getX(),     p2.getY()    );
    SDL_RenderDrawLineF(renderer, p1.getX() - 1, p1.getY(),     p2.getX() - 1, p2.getY()    );
    SDL_RenderDrawLineF(renderer, p1.getX(),     p1.getY() - 1, p2.getX(),     p2.getY() - 1);
    SDL_RenderDrawLineF(renderer, p1.getX() - 1, p1.getY() - 1, p2.getX() - 1, p2.getY() - 1);
}

void Renderer::render() {
    SDL_SetRenderTarget(renderer, NULL);
    // printf("renderpresenting...\n");
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, open_color(bg_color));
    SDL_RenderClear(renderer);
    // SDL_SetRenderDrawColor(renderer, open_color(current_color));
}

void Renderer::setTarget(Texture* texture) {
    current_texture = texture;
    if (texture == NULL) {
        SDL_SetRenderTarget(renderer, NULL);
        return;
    }
    SDL_SetRenderTarget(renderer, texture->getNativeTexture());
}

SDL_Renderer* Renderer::getNativeRenderer() {
    return renderer;
}

void Renderer::copyTexture(Texture* texture, const Vector2& pos) {
    SDL_FRect dst_rect = {pos.getX(), pos.getY(), texture->getSize().getX(), texture->getSize().getY()};
    SDL_RenderCopyF(renderer, texture->getNativeTexture(), NULL, &dst_rect);
}

void Renderer::copyTexture(Texture* texture, const Vector2& pos, const Vector2& dst_size) {
    SDL_FRect dst_rect = {pos.getX(), pos.getY(), dst_size.getX(), dst_size.getY()};
    SDL_RenderCopyF(renderer, texture->getNativeTexture(), NULL, &dst_rect);
}

void Renderer::copyTexture(Texture* texture, const Rect2f& src_rect, const Rect2f& dst_rect) {
    SDL_Rect sdl_src_rect = {static_cast<int>(src_rect.x), static_cast<int>(src_rect.y), static_cast<int>(src_rect.width), static_cast<int>(src_rect.height)};
    SDL_FRect sdl_dst_rect = {(dst_rect.x), (dst_rect.y), (dst_rect.width), (dst_rect.height)};
    SDL_RenderCopyF(renderer, texture->getNativeTexture(), &sdl_src_rect, &sdl_dst_rect);
}

// void Renderer::copyTextureRect(Texture* texture, const Vector2& pos, const Rect2f& rect) {
//     SDL_FRect dst_rect = {pos.getX(), pos.getY(), rect.width, rect.height};
//     SDL_Rect src_rect = {static_cast<int>(rect.x), static_cast<int>(rect.y), static_cast<int>(rect.width), static_cast<int>(rect.height)};
//     SDL_RenderCopyF(renderer, texture->getNativeTexture(), &src_rect, &dst_rect);
// }

// void Renderer::drawText(const Vector2& pos, const Vector2& size, const char* text, Color color) {
//     SDL_Surface* text_surface = TTF_RenderText_Solid(font, text, {open_color(color)});
//     SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, text_surface);
//     Vector2 p1 = coord_system->translatePoint(pos);
//     Vector2 p2 = coord_system->translatePoint(pos + size);
//     float y = p1.getY();
//     p1.setY(p2.getY());
//     p2.setY(y);
//     // printf("%f %f %f %f\n", p1.getX(), p1.getY(), p2.getX(), p2.getY());
//     SDL_Rect rect = {static_cast<int>p1.getX(), static_cast<int>p1.getY(), static_cast<int>(p2 - p1).getX(), static_cast<int>(p2 - p1).getY()};
//     SDL_RenderCopy(renderer, texture, NULL, &rect);
//     SDL_DestroyTexture(texture);
//     SDL_FreeSurface(text_surface);
// }

void Renderer::drawText(const Vector2& pos, const Vector2& size, const char* text, Color color) {
    // Font font(25);
    // printf("%p\n", default_font->getNativeFont());
    SDL_Surface* text_surface = TTF_RenderText_Solid(default_font->getNativeFont(), text, {open_color(color)});
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_SetTextureBlendMode(text_texture, SDL_BLENDMODE_BLEND);
    // printf("%f %f %f %f\n", p1.getX(), p1.getY(), p2.getX(), p2.getY());
    SDL_FRect rect = {pos.getX(), pos.getY(), size.getX(), size.getY()};
    // SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopyF(renderer, text_texture, NULL, &rect);
    SDL_RenderPresent(renderer);
    // SDL_Delay(2000);
    // exit(0);
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
}

void Renderer::drawText(const Vector2& pos, const char* text, Color color) {
    // Font font(25);
    // printf("%p\n", default_font->getNativeFont());
    SDL_Surface* text_surface = TTF_RenderText_Solid(default_font->getNativeFont(), text, {open_color(color)});
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_SetTextureBlendMode(text_texture, SDL_BLENDMODE_BLEND);
    // printf("%f %f %f %f\n", p1.getX(), p1.getY(), p2.getX(), p2.getY());
    // printf("BRUH! %s: %d, %d\n", text, text_surface->w, text_surface->h);
    SDL_FRect rect = {pos.getX(), pos.getY(), static_cast<float>(text_surface->w), static_cast<float>(text_surface->h)};
    // SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopyF(renderer, text_texture, NULL, &rect);
    SDL_RenderPresent(renderer);
    // SDL_Delay(2000);
    // exit(0);
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
}

void Renderer::drawTextCentered(const Vector2& pos, const Vector2& size, const char* text, Color color) {
    // Font font(25);
    // printf("%p\n", default_font->getNativeFont());
    SDL_Surface* text_surface = TTF_RenderText_Solid(default_font->getNativeFont(), text, {open_color(color)});
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_SetTextureBlendMode(text_texture, SDL_BLENDMODE_BLEND);
    // printf("%f %f %f %f\n", p1.getX(), p1.getY(), p2.getX(), p2.getY());
    float coefficient = (1.0f * text_surface->w) / text_surface->h;

    float text_size_x = size.getX() / 1.15;
    float text_size_y = size.getY() / 1.15;

    // printf("coefficient: %f\n", coefficient);

    if (text_size_y * coefficient < text_size_x) {
        if (text_size_y < 20) {
            text_size_y = Min(20.0f, size.getY());
        }
        text_size_x = text_size_y * coefficient;
    } else {
        text_size_y = text_size_x / coefficient;
    }

    // printf("text height: %f\n", text_size_y);

    SDL_FRect rect = {
        /*pos.getX() +*/ (size.getX() - text_size_x) / 2 ,
        /*pos.getY() +*/ (size.getY() - text_size_y) / 2,
        text_size_x,
        text_size_y};
    // SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopyF(renderer, text_texture, NULL, &rect);
    SDL_RenderPresent(renderer);
    // SDL_Delay(2000);
    // exit(0);
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
}


SystemEvent getSystemEvent() {
    SDL_Event event;
    int res = SDL_PollEvent(&event);
    SystemEvent result_event; // oh...
    // printf("%d\n", result_event);
    if (res == 0) {
        // printf("there is no event\n");
        result_event.event_type = NO_EVENT;
        return result_event;
    }
    switch (event.type)
    {
        case SDL_QUIT:
            // printf("oh sdl quit\n");
            result_event.event_type = QUIT_EVENT;
            break;

        case SDL_MOUSEBUTTONDOWN:
            // printf("oh sdl mb down\n");
            result_event.event_type = MOUSE_CLICK;
            result_event.mb_press_info = {Vector2(event.button.x, event.button.y), event.button.button, event.button.state == SDL_PRESSED};
            break;

        case SDL_MOUSEBUTTONUP:
            result_event.event_type = MOUSE_CLICK;
            result_event.mb_press_info = {Vector2(event.button.x, event.button.y), event.button.button, event.button.state == SDL_PRESSED};
            break;

        
        case SDL_MOUSEMOTION:
            // printf("oh sdl mb motion\n");
            result_event.event_type = MOUSE_MOVE;
            result_event.mouse_move = {Vector2(event.motion.x, event.motion.y), Vector2(event.motion.x + event.motion.xrel, event.motion.y + event.motion.yrel)};
            break;

        case SDL_KEYDOWN:
            result_event.event_type = KEY_DOWN;
            if (SDL_SCANCODE_A <= event.key.keysym.scancode && event.key.keysym.scancode <= SDL_SCANCODE_Z) {
                result_event.key_event.letter = event.key.keysym.scancode - SDL_SCANCODE_A + 'a';
            } else {
                result_event.key_event.letter = '\0';
            }
            // if (event.key.keysym.scancode == SDL_SCANCODE_D) {
            //     result_event.key_event.letter = 'd';
            // } else if (event.key.keysym.scancode == SDL_SCANCODE_N) {
            //     result_event.key_event.letter = 'n';
            // } else {
            //     result_event.key_event.letter = '\0';
            // }
            break;
        
        default:
            // printf("oh sdl other\n");
            result_event.event_type = OTHER_EVENTS;
            break;
    }
    return result_event;
}