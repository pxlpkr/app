#include <stdlib.h>
#include <stdint.h>

#include "renderer.h"

CH_Renderer* CH_Renderer_Create(int width, int height) {
    CH_Renderer* renderer = malloc(sizeof(CH_Renderer));

    renderer->camera = CH_Camera_Create(width, height);

    renderer->width = width;
    renderer->height = height;

    renderer->geometry = CH_Array_Create();

    return renderer;
}

void CH_Renderer_Destroy(CH_Renderer* renderer) {
    CH_Camera_Destroy(renderer->camera);
    for (int i = 0; i < renderer->geometry->len; i++) {
        CH_Geometry_Destroy(renderer->geometry->at[i]);
    }
    CH_Array_Destroy(renderer->geometry);

    free(renderer);
}

void CH_Renderer_Render(CH_Renderer* renderer, u_int32_t buffer[]) {
    for (int i = 0; i < renderer->geometry->len; i++) {
        CH_Geometry* geometry = (CH_Geometry*) renderer->geometry->at[i];
        for (int j = 0; j < 3; j++) {
            CH_Vector* vector = geometry->vertices[j];
            for (int x = -5; x < 5; x++) {
                for (int y = -5; y < 5; y++) {
                    buffer[
                        (int) vector->x + x +
                        ((int) vector->y + y) * renderer->width] = 0xFFFFFFFF;
                }
            }
        }
        // double avgDist = CH_Camera_GetAvgDist(renderer->camera, geometry);
        // printf("%f", avgDist);
    }
}