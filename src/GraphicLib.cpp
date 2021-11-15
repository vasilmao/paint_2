#include "GraphicLib.h"

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
    // SDL_SetTextureBlendMode(sdl_texture, SDL_BLENDMODE_BLEND);
}

Texture::Texture(Renderer* renderer, const Vector2& size, Color color) : size(size) {
    sdl_texture = SDL_CreateTexture(renderer->getNativeRenderer(),
                                    SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET,
                                    size.getX(),
                                    size.getY()
                                    );
    // SDL_SetTextureBlendMode(sdl_texture, SDL_BLENDMODE_BLEND);
    renderer->setTarget(this);
    SDL_SetRenderDrawColor(renderer->getNativeRenderer(), open_color(color));
    SDL_RenderClear(renderer->getNativeRenderer());
    SDL_FRect rect = {0, 0, size.getX(), size.getY()};
    SDL_RenderDrawRectF(renderer->getNativeRenderer(), &rect);
    SDL_RenderFillRectF(renderer->getNativeRenderer(), &rect);
}

Texture::Texture(Renderer* renderer, const char* filename) {
    SDL_Surface* surf = SDL_LoadBMP(filename);
    if (!surf) {
        printf("img not opened\n");
    }
    assert(surf);
    size = {static_cast<float>(surf->w), static_cast<float>(surf->h)};
    sdl_texture = SDL_CreateTextureFromSurface(
        renderer->getNativeRenderer(),
        surf
    );
    assert(sdl_texture);
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

Font::Font(int size) : size(size) {
    sdl_font = TTF_OpenFont("font.ttf", size);
}

Font::Font(const char* filename, int size) : size(size) {
    sdl_font = TTF_OpenFont(filename, size);
}

int Font::getSize() const {
    return size;
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
    SDL_RenderClear(renderer);

    TTF_Init();
    // font = TTF_OpenFont("Anonymous.ttf", 10);
    // assert(font);

}

Renderer::~Renderer() {
    // TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    printf("renderer destroyed!\n");
}

void Renderer::drawFilledCircle(const Vector2& center, const float r, Color color) const {
    SDL_SetRenderDrawColor(renderer, open_color(color));
    float r2 = r * r;
    Vector2 center_point(center);
    Vector2 circle_quadr_diagonal(r, -r);
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

void Renderer::drawRect(const Vector2& p1, const Vector2& p2, Color color) const {
    SDL_FRect rect = {p1.getX(), p1.getY(), p2.getX() - p1.getX(), p2.getY() - p1.getY()};
    SDL_SetRenderDrawColor(renderer, open_color(color));
    SDL_RenderDrawRectF(renderer, &rect);
}

void Renderer::drawFilledRect(const Vector2& p1, const Vector2& p2, Color color) const {
    SDL_SetRenderDrawColor(renderer, open_color(color));
    for (int x = p1.getX(); x <= p2.getX(); ++x) {
        SDL_RenderDrawLineF(renderer, x, p1.getY(), x, p2.getY());
    }
}

void Renderer::drawSegment(const Vector2& p1, const Vector2& p2, Color color) const {
    SDL_SetRenderDrawColor(renderer, open_color(color));
    SDL_RenderDrawLineF(renderer, p1.getX(), p1.getY(), p2.getX(), p2.getY());
}

void Renderer::drawLine(const Vector2& p1, const Vector2& p2, Color color) const {
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
    // printf("copying texture, (%f, %f), size (%f, %f)\n", dst_rect.x, dst_rect.y, dst_rect.w, dst_rect.h);
    SDL_RenderCopyF(renderer, texture->getNativeTexture(), NULL, &dst_rect);
    // render();
    // SDL_Delay(500);
}

void Renderer::copyTextureRect(Texture* texture, const Vector2& pos, const Rect2f& rect) {
    SDL_FRect dst_rect = {pos.getX(), pos.getY(), rect.width, rect.height};
    SDL_Rect src_rect = {static_cast<int>(rect.x), static_cast<int>(rect.y), static_cast<int>(rect.width), static_cast<int>(rect.height)};
    SDL_RenderCopyF(renderer, texture->getNativeTexture(), &src_rect, &dst_rect);
}

// SystemEvent Renderer::getEvent() const {
//     SDL_Event event;
//     int res = SDL_PollEvent(&event);
//     SystemEvent result_event; // oh...
//     if (res == 0) {
//         result_event.event_type = NO_EVENT;
//         return result_event;
//     }
//     switch (event.type)
//     {
//         case SDL_QUIT:
//             result_event.event_type = QUIT_EVENT;
//             break;

//         case SDL_MOUSEBUTTONDOWN:
//             result_event.event_type = MOUSE_BUTTON_DOWN;
//             result_event.mouse_pos = {coord_system->translatePixel(Vector2(event.button.x, event.button.y))};
//             break;
        
        
//         default:
//             result_event.event_type = OTHER_EVENTS;
//             break;
//     }
//     return result_event;
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
        
        default:
            // printf("oh sdl other\n");
            result_event.event_type = OTHER_EVENTS;
            break;
    }
    return result_event;
}