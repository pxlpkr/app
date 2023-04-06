#include <time.h>
#include <SDL.h>

#include "CHenge/renderer.h"
#include "CHenge/array.h"

#define MAX_FPS     60
#define WIDTH       600
#define HEIGHT      600
#define TITLE       "Window"

#define MAX_INT     2147483647

struct {
    SDL_Texture* image;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    CH_Renderer* renderer3d;

    u_int32_t buffer[WIDTH * HEIGHT];

    double heights[200][200];

    int exit_requested;

    double cam_dy;
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

void kb_handler(void) {
    const u_int8_t* pressed_keys = SDL_GetKeyboardState(NULL);

    double moveSpeed = 0.2;
    if (pressed_keys[SDL_SCANCODE_R])
        moveSpeed *= 5;
    else if (pressed_keys[SDL_SCANCODE_LSHIFT] || pressed_keys[SDL_SCANCODE_RSHIFT])
        moveSpeed *= 0.5;

    if (pressed_keys[SDL_SCANCODE_W]) {
        CH_Camera_MoveRelative(app.renderer3d->camera, 0, 0, moveSpeed);
    }
    if (pressed_keys[SDL_SCANCODE_A]) {
        CH_Camera_MoveRelative(app.renderer3d->camera, -moveSpeed, 0, 0);
    }
    if (pressed_keys[SDL_SCANCODE_S]) {
        CH_Camera_MoveRelative(app.renderer3d->camera, 0, 0, -moveSpeed);
    }
    if (pressed_keys[SDL_SCANCODE_D]) {
        CH_Camera_MoveRelative(app.renderer3d->camera, moveSpeed, 0, 0);
    }
    if (pressed_keys[SDL_SCANCODE_SPACE]) {
        // CH_Camera_MoveRelative(app.renderer3d->camera, 0, -moveSpeed, 0);
        if (app.cam_dy == 0)
            app.cam_dy -= 0.3;
    }
    if (pressed_keys[SDL_SCANCODE_LSHIFT] || pressed_keys[SDL_SCANCODE_RSHIFT]) {
        CH_Camera_MoveRelative(app.renderer3d->camera, 0, 2 * moveSpeed, 0);
    }

    float rotateSpeed = 3;
    if (pressed_keys[SDL_SCANCODE_RIGHT]) {
        CH_Camera_RotateYaw(app.renderer3d->camera, rotateSpeed);
    }
    if (pressed_keys[SDL_SCANCODE_LEFT]) {
        CH_Camera_RotateYaw(app.renderer3d->camera, -rotateSpeed);
    }
    if (pressed_keys[SDL_SCANCODE_UP]) {
        CH_Camera_RotatePitch(app.renderer3d->camera, -rotateSpeed);
    }
    if (pressed_keys[SDL_SCANCODE_DOWN]) {
        CH_Camera_RotatePitch(app.renderer3d->camera, rotateSpeed);
    }

    // if (app.renderer3d->camera->x > 0 && app.renderer3d->camera->x < 200 &&
    //     app.renderer3d->camera->z > 0 && app.renderer3d->camera->z < 200) {
    //         app.renderer3d->camera->y = app.heights[(int) app.renderer3d->camera->x][(int) app.renderer3d->camera->x];
    //     }
}

void physics_handler(void) {
    double height;
    int x = app.renderer3d->camera->x;
    int y = app.renderer3d->camera->y;
    int z = app.renderer3d->camera->z;

    if (x >= 0 && x < 199 && z >= 0 && z < 199) {
        height = app.heights[x][z] * fmod(x, 1) * fmod(z, 1)
                +app.heights[x+1][z] * (1-fmod(x, 1)) * fmod(z, 1)
                +app.heights[x+1][z+1] * (1-fmod(x, 1)) * (1-fmod(z, 1))
                +app.heights[x][z+1] * fmod(x, 1) * (1-fmod(z, 1)) - 2;
    } else {
        height = 10;
    }
    
    app.renderer3d->camera->y += app.cam_dy;
    if (app.renderer3d->camera->y >= height) {
        app.renderer3d->camera->y = height;
        app.cam_dy = 0;
    } else {
        app.cam_dy += 0.02;
    }
}

void iterate_game_loop(void) {
    clock_t start = clock();

    event_handler();

    physics_handler();

    kb_handler();

    memset(app.buffer, 0xFFFFFF00, sizeof(app.buffer));
    CH_Renderer_Render(app.renderer3d, app.buffer);

    SDL_UpdateTexture(
        app.texture,
        NULL,                       //Select entire texture
        app.buffer,
        WIDTH * 4                   //Bytes per Pixel
    );
    SDL_RenderCopy(
        app.renderer,
        app.image,
        NULL, NULL                  //Select entire source and destination
    );
    SDL_RenderCopy(
        app.renderer,
        app.texture,
        NULL, NULL                  //Select entire source and destination
    );
    SDL_RenderPresent(app.renderer);

    clock_t end = clock();

    double time_elapsed = (double) (end - start) * 1000.0 / CLOCKS_PER_SEC;

    double delay_nanos = 1000000000 / MAX_FPS - time_elapsed * 1000000;

    if (delay_nanos > 0) {
        struct timespec delay = {0, delay_nanos};
        nanosleep(&delay, NULL);
    }

    printf("%f ms \n", time_elapsed);
}

u_int32_t get_mesh_color() {
    const int CLR = rand() % 128;
    const int CHANNEL = rand() % 3;
    const int WHICH = rand() % 2;

    u_int8_t colors[] = {0, 0, 0};

    colors[CHANNEL] = 255;
    colors[(CHANNEL + 1) % 3] = 255;
    colors[(CHANNEL + WHICH) % 3] -= CLR;

    return 0x000000FF
        + colors[0] * 0x100
        + colors[1] * 0x100 * 0x100
        + colors[2] * 0x100 * 0x100 * 0x100;

    // return 0x000000FF
    //     + 128 + CLR % 128 * 0x100
    //     + 128 + CLR % 128 * 0x100 * 0x100
    //     + 128 + CLR % 128 * 0x100 * 0x100 * 0x100;
}

void make_ground_mesh(void) {
    CH_Geometry* g1 = CH_Geometry_Create(
        CH_Vector_Create(0, 10, 40),
        CH_Vector_Create(0, 0, 40),
        CH_Vector_Create(10, 10, 40)
    );

    // g1->color = 0x007832FF + rand() % 180 * 0x100 * 0x100 * 0x100;
    g1->color = 0xFF0000FF;

    CH_Array_Append(app.renderer3d->geometry, g1);



    CH_Geometry* g2 = CH_Geometry_Create(
        CH_Vector_Create(-5, 10, 41),
        CH_Vector_Create(-5, 0, 41),
        CH_Vector_Create(5, 10, 41)
    );

    // g1->color = 0x007832FF + rand() % 180 * 0x100 * 0x100 * 0x100;
    g2->color = 0x0000FFFF;

    CH_Array_Append(app.renderer3d->geometry, g2);
}

void make_ground_mesh2(void) {
    int W = 200;
    int H = 200;

    double I = 1;
    double I_2 = 0.5;

    for (int i = 0; i < W; i++) {
        for (int j = 0; j < W; j++) {
            if (i == 0) {
                if (j == 0) {
                    app.heights[i][j] = 10;
                } else {
                    app.heights[i][j] = app.heights[i][j-1] + (double) rand() * I / MAX_INT - I_2;
                }
            } else {
                if (j == 0) {
                    app.heights[i][j] = app.heights[i-1][j] + (double) rand() * I / MAX_INT - I_2;
                } else {
                    app.heights[i][j] = (app.heights[i][j-1]+app.heights[i-1][j])/2 + (double) rand() * I / MAX_INT - I_2;
                }
            }
        }
    }

    for (int i = 0; i < W - 1; i++) {
        for (int j = 0; j < H - 1; j++) {
            CH_Geometry* g1 = CH_Geometry_Create(
                CH_Vector_Create(i+1, app.heights[i+1][j+1], j+1),
                CH_Vector_Create(i+1, app.heights[i+1][j], j),
                CH_Vector_Create(i, app.heights[i][j], j)
            );

            // g1->color = 0x007832FF + rand() % 180 * 0x100 * 0x100 * 0x100;
            g1->color = get_mesh_color();

            CH_Array_Append(app.renderer3d->geometry, g1);

            CH_Geometry* g2 = CH_Geometry_Create(
                CH_Vector_Create(i+1, app.heights[i+1][j+1], j+1),
                CH_Vector_Create(i, app.heights[i][j], j),
                CH_Vector_Create(i, app.heights[i][j+1], j+1)
            );

            // g2->color = 0x007832FF + rand() % 180 * 0x100 * 0x100 * 0x100;

            g2->color = get_mesh_color();

            CH_Array_Append(app.renderer3d->geometry, g2);
        }
    }
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

    SDL_Surface* image_surface = SDL_LoadBMP("src/background.bmp");

    app.image = SDL_CreateTextureFromSurface(app.renderer, image_surface);

    SDL_SetTextureBlendMode(app.texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(app.image, SDL_BLENDMODE_BLEND);

    SDL_FreeSurface(image_surface);

    app.renderer3d = CH_Renderer_Create(WIDTH, HEIGHT);
    app.renderer3d->camera->render_distance = 999;

    app.cam_dy = 0;

    // app.renderer3d->camera->pitch = 30;
    // app.renderer3d->camera->yaw = 45;

    make_ground_mesh2();

    // CH_Array_Append(app.renderer3d->geometry, CH_Geometry_Create(
    //     CH_Vector_Create(-4, 4, 10),
    //     CH_Vector_Create(-4, 0, 10),
    //     CH_Vector_Create(4, 0, 10)
    // ));
}

int main() {
    initialize();

    while (!app.exit_requested)
        iterate_game_loop();

    SDL_DestroyTexture(app.image);
    SDL_DestroyTexture(app.texture);
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);

    CH_Renderer_Destroy(app.renderer3d);

    return 0;
}