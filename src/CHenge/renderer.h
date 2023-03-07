#pragma once

#include "camera.h"
#include "array.h"

typedef struct {
    CH_Camera* camera;
    CH_Array* geometry;
    CH_Vec2D* dim;
} CH_Renderer;

CH_Renderer* CH_Renderer_Create(CH_Vec2D*);
void CH_Renderer_Render(CH_Renderer* renderer, u_int32_t buffer[]);