#include <time.h>
#include <SDL.h>

#define MAX_FPS 60

#define WIDTH 768
#define HEIGHT 364
#define DEFAULT_BALL_SPEED 4

typedef struct {
    int x;
    int y;
    int width;
    int height;

    int speed;
} paddle_t;

struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    u_int32_t buffer[WIDTH * HEIGHT];

    struct {
        paddle_t paddle_1;
        paddle_t paddle_2;

        int score_1;
        int score_2;

        struct {
            int width;
            int height;
            int x;
            int y;
            int dx;
            int dy;
        } ball;
    } game;

    int exit_requested;
} app;

void render_paddle(paddle_t* paddle) {
    for (int p_y = 0; p_y < paddle->height; p_y++) {
        for (int p_x = 0; p_x < paddle->width; p_x++) {
            int x = paddle->x + p_x;
            int y = paddle->y + p_y;

            if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
                continue;

            app.buffer[x + y*WIDTH] = 0xFFFFFFFF;
        }
    } 
}

void update_buf() {
    memset(app.buffer, 0, sizeof(app.buffer));

    for (int p_y = 0; p_y < app.game.ball.height; p_y++) {
        for (int p_x = 0; p_x < app.game.ball.width; p_x++) {
            int x = app.game.ball.x + p_x;
            int y = app.game.ball.y + p_y;

            if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
                continue;

            app.buffer[x + y*WIDTH] = 0xFFFFFFFF;
        }
    }

    render_paddle(&app.game.paddle_1);
    render_paddle(&app.game.paddle_2);
}

void event_handler() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                app.exit_requested = 1;
                break;
        }
    }
}

void kb_handler() {
    const u_int8_t* pressed_keys = SDL_GetKeyboardState(NULL);
    if (pressed_keys[SDL_SCANCODE_UP]) {
        app.game.paddle_2.y -= app.game.paddle_2.speed;
    }

    if (pressed_keys[SDL_SCANCODE_DOWN]) {
        app.game.paddle_2.y += app.game.paddle_2.speed;
    }

    if (pressed_keys[SDL_SCANCODE_W]) {
        app.game.paddle_1.y -= app.game.paddle_1.speed;
    }

    if (pressed_keys[SDL_SCANCODE_S]) {
        app.game.paddle_1.y += app.game.paddle_1.speed;
    }

    if (app.game.paddle_1.y < 0)
        app.game.paddle_1.y = 0;

    if (app.game.paddle_1.y > HEIGHT - app.game.paddle_1.height)
        app.game.paddle_1.y = HEIGHT - app.game.paddle_1.height;

    if (app.game.paddle_2.y < 0)
        app.game.paddle_2.y = 0;

    if (app.game.paddle_2.y > HEIGHT - app.game.paddle_2.height)
        app.game.paddle_2.y = HEIGHT - app.game.paddle_2.height;
}

void update_title() {
    char* new_string;
    asprintf(&new_string, "%i    PONG    %i", app.game.score_1, app.game.score_2);
    SDL_SetWindowTitle(app.window, new_string);
}

void add_y_speed() {
    if (app.game.ball.dy > 0)
        app.game.ball.dy++;
    else
        app.game.ball.dy--;
}

void reset_y_speed() {
    if (app.game.ball.dy > 0)
        app.game.ball.dy = DEFAULT_BALL_SPEED;
    else
        app.game.ball.dy = -DEFAULT_BALL_SPEED;
}

void iterate_game_loop() {
    event_handler();

    kb_handler();

    update_buf();

    int old_x = app.game.ball.x;

    app.game.ball.x += app.game.ball.dx;
    app.game.ball.y += app.game.ball.dy;

    int paddle_1_intersection = app.game.paddle_1.x + app.game.paddle_1.width;
    int paddle_2_intersection = app.game.paddle_2.x;
    if (old_x > paddle_1_intersection && app.game.ball.x <= paddle_1_intersection) {
        if (app.game.ball.y < app.game.paddle_1.y + app.game.paddle_1.height &&
            app.game.ball.y + app.game.ball.height > app.game.paddle_1.y) {
                add_y_speed();
                app.game.ball.dx = -(app.game.ball.dx - 1);
        }
    } else if ( old_x + app.game.ball.width < paddle_2_intersection &&
                app.game.ball.x + app.game.ball.width >= paddle_2_intersection) {
        if (app.game.ball.y < app.game.paddle_2.y + app.game.paddle_2.height &&
            app.game.ball.y + app.game.ball.height > app.game.paddle_2.y) {
                add_y_speed();
                app.game.ball.dx = -(app.game.ball.dx + 1);
        }
    }

    if (app.game.ball.x < 0) {
        app.game.score_2++;
        app.game.ball.x = app.game.paddle_1.x + 2 * app.game.paddle_1.width;
        app.game.ball.y = WIDTH / 2;
        reset_y_speed();
        app.game.ball.dx = DEFAULT_BALL_SPEED;

        update_title();
    } else if (app.game.ball.x > WIDTH - app.game.ball.width) {
        app.game.score_1++;
        app.game.ball.x = app.game.paddle_2.x - app.game.paddle_2.width;
        app.game.ball.y = WIDTH / 2;
        reset_y_speed();
        app.game.ball.dx = -DEFAULT_BALL_SPEED;

        update_title();
    }

    if (app.game.ball.y < 0) {
        app.game.ball.y = -app.game.ball.y;
        app.game.ball.dy = -app.game.ball.dy;
    } else if (app.game.ball.y > HEIGHT - app.game.ball.height) {
        app.game.ball.y = 2 * (HEIGHT - app.game.ball.height) - app.game.ball.y;
        app.game.ball.dy = -app.game.ball.dy;
    }

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

int main() {
    app.game.paddle_1.speed = 4;
    app.game.paddle_2.speed = 4;
    app.game.paddle_1.width = 20;
    app.game.paddle_1.height = 80;
    app.game.paddle_2.width = 20;
    app.game.paddle_2.height = 80;
    app.game.paddle_1.x = app.game.paddle_1.width;
    app.game.paddle_1.y = HEIGHT / 2 - app.game.paddle_1.height / 2;
    app.game.paddle_2.x = WIDTH - 2 * app.game.paddle_2.width;
    app.game.paddle_2.y = HEIGHT / 2 - app.game.paddle_2.height / 2;

    app.game.ball.x =  app.game.paddle_1.x + 2 * app.game.paddle_1.width;
    app.game.ball.y = HEIGHT / 2;
    app.game.ball.dx = DEFAULT_BALL_SPEED;
    app.game.ball.dy = DEFAULT_BALL_SPEED;
    app.game.ball.height = 16;
    app.game.ball.width = 16;

    app.game.score_1 = 0;
    app.game.score_2 = 0;

    app.window = SDL_CreateWindow(
        "0    PONG    0",
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

    while (!app.exit_requested)
        iterate_game_loop();

    SDL_DestroyTexture(app.texture);
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);

    return 0;
}