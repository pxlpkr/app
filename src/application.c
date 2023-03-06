#include <stdio.h>
#include <SDL.h>

struct {
    SDL_Window* window;
    int exit_requested;
} app;

int main() {
    app.window = SDL_CreateWindow
    (
        "Test Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        400,
        400,
        SDL_WINDOW_ALLOW_HIGHDPI
    );

    while (!app.exit_requested) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    app.exit_requested = 1;
                    break;
            }
        }
    }

    SDL_DestroyWindow(app.window);

    return 0;
}