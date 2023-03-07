#include <stdlib.h>
#include <stdint.h>

#include "renderer.h"

CH_Renderer* CH_Renderer_Create(CH_Vec2D* dim) {
    CH_Renderer* renderer = malloc(sizeof(CH_Renderer));

    renderer->camera = CH_Camera_Create(dim);

    renderer->dim = dim;

    renderer->geometry = CH_Array_Create();

    return renderer;
}

void CH_Renderer_Render(CH_Renderer* renderer, u_int32_t buffer[]) {
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 100; j++) {
            buffer[i * renderer->dim->x + j] = 0xFF0000FF;
        }
    }
}