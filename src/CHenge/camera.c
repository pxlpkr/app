#include <stdlib.h>
#include <math.h>

#include "camera.h"

#define PI 3.141592654

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
    return PI * degrees / 180;
}

CH_Vector* CH_Camera_GetRelativeCoordinates(CH_Camera* camera, CH_Vector* v) {
    double x, y, z, x_, y_, z_;

    x = v->x-camera->x;
    y = v->y-camera->y;
    z = v->z-camera->z;

    z_ = cos(rad(camera->yaw))       * z + sin(rad(camera->yaw))      * x;
    y_ = sin(rad(camera->pitch+90))  * y + cos(rad(camera->pitch+90)) * z_;
    x_ = cos(rad(camera->yaw))       * x - sin(rad(camera->yaw))      * z;

    z_ = sin(rad(90+camera->pitch)) * z_ - cos(rad(90+camera->pitch)) * y;

    return CH_Vector_Create(x_, y_, z_);
}

CH_Point* CH_Camera_Transform(CH_Camera* camera, CH_Vector* vec) {
    int max_dimension;
    int x, y;

    if (camera->view_height >= camera->view_width)
        max_dimension = camera->view_height;
    else
        max_dimension = camera->view_width;

    if (vec->z <= 0.0001) {
        x = (int) (camera->view_width /2 + (max_dimension/2)*vec->x);
        y = (int) (camera->view_height/2 + (max_dimension/2)*vec->y);
    } else {
        x = (int) (camera->view_width /2 + (max_dimension/2)*vec->x/vec->z);
        y = (int) (camera->view_height/2 + (max_dimension/2)*vec->y/vec->z);
    }
    
    return CH_Point_Create(x, y);
}

void CH_Camera_MoveRelative(CH_Camera* camera, double x, double y, double z) {
    camera->x += x * cos(rad(camera->yaw));
    camera->x += z * sin(rad(camera->yaw));
    camera->z += z * cos(rad(camera->yaw));
    camera->z -= x * sin(rad(camera->yaw));
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