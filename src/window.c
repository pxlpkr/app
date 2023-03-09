#include <time.h>
#include <SDL.h>

#include "CHenge/renderer.h"
#include "CHenge/array.h"

#define MAX_FPS     60
#define WIDTH       400
#define HEIGHT      400
#define TITLE       "Window"

struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    CH_Renderer* renderer3d;

    u_int32_t buffer[WIDTH * HEIGHT];

    int exit_requested;
} app;

void event_handler(void) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                app.exit_requested = 1;
                break;
        }
    }
}

void iterate_game_loop(void) {
    event_handler();

    memset(app.buffer, 0, sizeof(app.buffer));
    CH_Renderer_Render(app.renderer3d, app.buffer);

    SDL_UpdateTexture(
        app.texture,
        NULL,                       //Select entire texture
        app.buffer,
        WIDTH * 4                   //Bytes per Pixel
    );
    SDL_RenderCopyEx(
        app.renderer,
        app.texture,
        NULL, NULL,                 //Select entire source and destination
        0.0, NULL, SDL_FLIP_NONE    //Image modification
    );
    SDL_RenderPresent(app.renderer);

    struct timespec delay = {0, 1000000000 / MAX_FPS};
    nanosleep(&delay, NULL);
}

void initialize(void) {
    app.window = SDL_CreateWindow(
        TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_ALLOW_HIGHDPI
    );

    app.renderer = SDL_CreateRenderer(
        app.window,
        -1,                             //Get default driver
        0
    );

    app.texture = SDL_CreateTexture(
        app.renderer,
        SDL_PIXELFORMAT_RGBA8888,       //1 Byte x 4 Channels (ARGB)
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT
    );

    app.renderer3d = CH_Renderer_Create(WIDTH, HEIGHT);

    CH_Array_Append(app.renderer3d->geometry, CH_Geometry_Create(
        CH_Vector_Create(10, 10, 10),
        CH_Vector_Create(200, 10, 15),
        CH_Vector_Create(20, 30, 10)
    ));
}

int main() {
    initialize();

    while (!app.exit_requested)
        iterate_game_loop();

    SDL_DestroyTexture(app.texture);
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);

    CH_Renderer_Destroy(app.renderer3d);

    return 0;
}