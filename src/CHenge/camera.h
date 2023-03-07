#pragma once

#include "vector.h"

typedef struct {
    CH_Vec2D* view_window;

    int render_distance;

    double x;
    double y;
    double z;

    float yaw;
    float pitch;
} CH_Camera;

extern CH_Camera* CH_Camera_Create(CH_Vec2D* view_window);