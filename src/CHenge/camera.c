#include <stdlib.h>
#include <math.h>

#include "camera.h"

double CH_GetCameraZScale(CH_Camera* camera, double z) {
    return pow(0.25, z / camera->view_window->y);
}

CH_Camera* CH_Camera_Create(CH_Vec2D* view_window) {
    CH_Camera* camera = malloc(sizeof(CH_Camera));

    camera->view_window = view_window;

    return camera;
}