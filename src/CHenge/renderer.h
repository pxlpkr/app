#pragma once

#include <stdint.h>

#include "geometry.h"
#include "camera.h"
#include "array.h"

typedef struct {
    CH_Camera* camera;
    CH_Array* geometry;

    int width;
    int height;
} CH_Renderer;

extern CH_Renderer* CH_Renderer_Create(int width, int height);
extern void CH_Renderer_Render(CH_Renderer* renderer, u_int32_t buffer[]);
extern void CH_Renderer_Destroy(CH_Renderer* renderer);