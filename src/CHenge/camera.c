#include <stdlib.h>
#include <math.h>

#include "camera.h"

#define PI 3.141592654
#define PI180 0.0298936849

CH_Camera* CH_Camera_Create(int view_width, int view_height) {
    CH_Camera* camera = malloc(sizeof(CH_Camera));

    camera->view_width = view_width;
    camera->view_height = view_height;

    return camera;
}

void CH_Camera_Destroy(CH_Camera* camera) {
    free(camera);
}

/**
 * Private; Converts degrees to radians
 * 
 * \param degrees
 */
double rad(double degrees) {
    return PI * degrees / 180.0;
}

CH_Vector CH_Camera_GetRelativeCoordinates(CH_Camera* camera, CH_Vector* v) {
    double x, y, z, x_, y_, z_, rad_yaw, rad_pitch90;

    x = v->x-camera->x;
    y = v->y-camera->y;
    z = v->z-camera->z;

    rad_yaw = rad(camera->yaw);
    rad_pitch90 = rad(camera->pitch+90);

    z_ = cos(rad_yaw)       * z + sin(rad_yaw)      * x;
    y_ = sin(rad_pitch90)  * y + cos(rad_pitch90) * z_;
    x_ = cos(rad_yaw)       * x - sin(rad_yaw)      * z;

    z_ = sin(rad_pitch90) * z_ - cos(rad_pitch90) * y;

    CH_Vector r = {x_, y_, z_};
    return r;
}

CH_Point CH_Camera_Transform(CH_Camera* camera, CH_Vector* vec) {
    int max_dimension;
    int x, y;

    if (camera->view_height >= camera->view_width)
        max_dimension = camera->view_height;
    else
        max_dimension = camera->view_width;

    if (vec->z <= 0.0001) {
        x = (int) (camera->view_width  * 0.5 + (max_dimension * 0.5)*vec->x);
        y = (int) (camera->view_height * 0.5 + (max_dimension * 0.5)*vec->y);
    } else {
        x = (int) (camera->view_width  * 0.5 + (max_dimension * 0.5)*vec->x/vec->z);
        y = (int) (camera->view_height * 0.5 + (max_dimension * 0.5)*vec->y/vec->z);
    }

    CH_Point r = {x, y};
    return r;
}

void CH_Camera_MoveRelative(CH_Camera* camera, double x, double y, double z) {
    double rad_yaw = rad(camera->yaw);
    double cos_rad_yaw = cos(rad_yaw);
    double sin_rad_yaw = sin(rad_yaw);

    camera->x += x * cos_rad_yaw;
    camera->x += z * sin_rad_yaw;
    camera->z += z * cos_rad_yaw;
    camera->z -= x * sin_rad_yaw;
    camera->y += y;
}

void CH_Camera_RotateYaw(CH_Camera* camera, float amount) {
    camera->yaw += amount;
    if (camera->yaw < 0.0F)
        camera->yaw += 360.0F;
    camera->yaw = fmod(camera->yaw, 360.0F);
}

void CH_Camera_RotatePitch(CH_Camera* camera, float amount) {
    camera->pitch += amount;
    if (camera->pitch < -90.0F)
        camera->pitch = -90.0F;
    else if (camera->pitch > 90.0F)
        camera->pitch = 90.0F;
}