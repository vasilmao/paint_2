#include "App.h"
#include "PluginRealization.h"

const int WIDTH  = 1000;
const int HEIGHT = 750;

const Color bg_color = {185, 226, 235, 255};

App::App() {

    TextureManager::setSkinsDir("skins/light/");

    // TODO: SPLIT INTO FUNCTIONS!!!!!!!!!!!!!!!!!!!
    // TODO: MORE SKINS
    // TODO: more instruments with pref panel
    // TODO: BETTER SLIDER
    // TODO: FILTERS
    // TODO: canvas from file
    // TODO: PLUGINS!!
    running = false;

    width = WIDTH;
    height = HEIGHT;
    renderer = new Renderer(width, height, bg_color);
    // printf("render created\n");
    main_window = new MainWindow(renderer, {static_cast<float>(width), static_cast<float>(height)});
    // CanvasWindow* canvas_w = new CanvasWindow(renderer, {40, 40}, {500, 300});
    // CanvasViewportWindow* canvas_w1 = new CanvasViewportWindow(renderer, {400, 400}, {500, 300});
    // canvas_w->setInstrument(instrument);
    // main_window->attachWindow(canvas_w);
    // main_window->attachWindow(canvas_w1);
    printf("uh oh society\n");
    // GUIInstrumentChanged event(instrument);
    
    InstrumentPanel::create(renderer, main_window);
    // Texture* brush_texture = new Texture(renderer, "skins/Brush.bmp");

    Texture* kostil_texture = new Texture(renderer, "skins/light/Brush.bmp");
    delete kostil_texture;
    Texture* brush_texture = new Texture(renderer, "skins/light/Brush.bmp");
    Texture* brush_texture_pressed = new Texture(renderer, "skins/light/Brush.bmp");
    // printf("brush texture... %p\n", brush_texture_pressed);
    renderer->setTarget(brush_texture_pressed);
    renderer->drawRect({0, 0}, brush_texture_pressed->getSize(), {255, 0, 0, 255});
    renderer->setTarget(nullptr);

    ButtonSkin* brush_btn_skin = new ButtonSkin(
        brush_texture,
        nullptr,
        brush_texture_pressed
    );
    Brush* brush_instr = new Brush();
    Eraser* eraser_instr = new Eraser();

    Texture* eraser_texture = new Texture(renderer, "skins/light/Eraser.bmp");
    Texture* eraser_texture_pressed = new Texture(renderer, "skins/light/Eraser.bmp");
    renderer->setTarget(eraser_texture_pressed);
    renderer->drawRect({0, 0}, eraser_texture_pressed->getSize(), {255, 0, 0, 255});
    renderer->setTarget(nullptr);

    ButtonSkin* eraser_btn_skin = new ButtonSkin(
        eraser_texture,
        nullptr,
        eraser_texture_pressed
    );

    InstrumentPanel::getInstance()->addInstrument(brush_instr, brush_btn_skin);
    InstrumentPanel::getInstance()->addInstrument(eraser_instr, eraser_btn_skin);
    // InstrumentPanel::getInstance()->setInstrument(brush_instr);
    main_window->attachWindow(InstrumentPanel::getInstance());


    Texture* strange_texture1 = new Texture(renderer, {60, 60}, {0, 0, 0, 0});
    renderer->setTarget(strange_texture1);
    renderer->drawFilledCircle({30, 30}, 30, {255, 255, 255, 255});
    renderer->setTarget(strange_texture1);
    renderer->drawFilledCircle({30, 30}, 15, {0, 0, 0, 0});

    Texture* strange_texture2 = new Texture(renderer, {60, 60}, {0, 0, 0, 0});
    renderer->setTarget(strange_texture2);
    renderer->drawFilledCircle({30, 30}, 30, {255, 0, 0, 255});
    renderer->setTarget(strange_texture2);
    renderer->drawFilledCircle({30, 30}, 15, {0, 0, 0, 0});

    Skin* strange_skin1 = new LeakyCircleSkin(strange_texture1, nullptr, strange_texture2, 30, 15);
    EventHandler* strange_handler = dynamic_cast<EventHandler*>(new ButtonHandler(nullptr, new EmptyFunctor()));
    Button* strange_btn = new Button(
        {940, 690},
        {60, 60},
        strange_handler,
        main_window,
        strange_skin1
    );

    strange_handler->setWindow(strange_btn);

    main_window->attachWindow(strange_btn);

    printf("ya1\n");
    // RayCasterHolder* rc = new RayCasterHolder(renderer, {10, 10}, {430, 430}, main_window);
    // main_window->attachWindow(rc);

    void* handler = dlopen("./plugin.so", RTLD_NOW);
    printf("%p\n", handler);
    // printf("%s\n", dlerror());
    CreateFunction create_f = (plugin::CreateFunction)dlsym(handler, "Create");
    printf("%p\n", create_f);
    printf("%s\n", dlerror());
    API* api = new API();
    IPlugin* plugin_api = (*create_f)(api);

    Tools tools = plugin_api->GetTools();

    for (uint32_t i = 0; i < tools.count; ++i) {
        ITool* cur_tool = tools.tools[i];
        ToolInstrument* instr = new ToolInstrument(cur_tool);
        const char* filename = cur_tool->GetIconFileName();
        Texture* btn_texture = new Texture(renderer, filename);
        Texture* btn_texture_pressed = new Texture(renderer, filename);
        renderer->setTarget(btn_texture_pressed);
        renderer->drawRect({0, 0}, btn_texture_pressed->getSize(), {255, 0, 0, 255});


        ButtonSkin* btn_skin = new ButtonSkin(btn_texture, nullptr, btn_texture_pressed);
        // btn_skin->resize({24, 24});
        InstrumentPanel::getInstance()->addInstrument(instr, btn_skin);
    }
    

    printf("ya2\n");

    printf("constructed\n");
}

void App::run() {
    running = true;
    // printf("running\n");
    clock_t time1 = clock();
    clock_t time2 = 0;
    while (running) {
        SystemEvent sys_event = getSystemEvent();
        while (sys_event.event_type != SystemEventTypes::NO_EVENT) {
            // printf("event got\n");
            if (sys_event.event_type == SystemEventTypes::MOUSE_CLICK) {
                printf("uh oh mouse button society\n");
                printf("%d\n", sys_event.mb_press_info.button_type);
                if (sys_event.mb_press_info.button_type == SDL_BUTTON_LEFT) {
                    GUILeftMouseButton event(sys_event);
                    printf("kek\n");
                    main_window->onEvent(&event);
                    printf("lol\n");
                } else if (sys_event.mb_press_info.button_type == SDL_BUTTON_RIGHT) {
                    GUIRightMouseButton event(sys_event);
                    printf("kek\n");
                    main_window->onEvent(&event);
                    printf("lol\n");
                }
            } else if (sys_event.event_type == SystemEventTypes::MOUSE_MOVE) {
                GUIMouseMove event(sys_event);
                main_window->onEvent(&event);
            } else if (sys_event.event_type == SystemEventTypes::QUIT_EVENT) {
                running = false;
            } else if (sys_event.event_type == SystemEventTypes::KEY_DOWN) {
                if (sys_event.key_event.letter == 'n') {
                    TextureManager::setNight(renderer);
                }
                if (sys_event.key_event.letter == 'd') {
                    TextureManager::setDay(renderer);
                }
            }
            if (!running) {
                break;
            }
            // printf("yaa\n");
            sys_event = getSystemEvent();
        }
        time2 = clock();
        GUITimePassed time_event(((float) (time2 - time1)) / CLOCKS_PER_SEC);
        time1 = clock();
        main_window->onEvent(&time_event);
        if (main_window->isToDelete()) {
            // delete main_window;
            running = false;
        }
        main_window->clean();
        // if ((dynamic_cast<MainWindow*>(InstrumentPanel::getInstance()->getParent()))->kek) {
        //     exit(0);
        // }
        if (!running) {
            break;
        }
        // printf("before render\n");
        main_window->render(renderer);
        // printf("present\n");
        renderer->render();
        // time2 = clock();
        // printf("%lf\n", ((double) (time2 - time1)) / CLOCKS_PER_SEC);

        // SDL_Delay(500);
    }
    
}

void App::stop() {
    running = false;
}

App::~App() {
    delete main_window;
    delete renderer;
}