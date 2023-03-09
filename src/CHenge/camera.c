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

double rad(double n) {
    return PI * n / 180;
}

CH_Vector* CH_Camera_GetRelativeCoordinates(CH_Camera* camera, CH_Vector* v) {
    double x = v->x-camera->x;
    double y = v->y-camera->y;
    double z = v->z-camera->z;

    double z_ = cos(rad(camera->yaw))       * z + sin(rad(camera->yaw))      * x;
    double y_ = sin(rad(camera->pitch+90))  * y + cos(rad(camera->pitch+90)) * z_;
    double x_ = cos(rad(camera->yaw))       * x - sin(rad(camera->yaw))      * z;

    z_ = sin(rad(90+camera->pitch)) * z_ - cos(rad(90+camera->pitch)) * y;

    return CH_Vector_Create(x_, y_, z_);
}

double CH_Camera_GetAvgDist(CH_Camera* camera, CH_Geometry* geometry) {
    double total = 0;
    for (int i = 0; i < 3; i++) {
        CH_Vector* vector = CH_Camera_GetRelativeCoordinates(camera, geometry->vertices[i]);
        
        total += CH_Vector_GetMagnitude(vector);

        free(vector);
    }
    
    return total / 3;
}

void CH_Camera_Destroy(CH_Camera* camera) {
    free(camera);
}