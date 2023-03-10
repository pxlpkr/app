#pragma once

#include <stdlib.h>

#include "vector.h"         //For vector operations
#include "point.h"         //For point operations
#include "geometry.h"       //For geometry operations

typedef struct {
    int view_width;
    int view_height;

    int render_distance;    //TODO! No functionality yet

    double x;               //Position
    double y;
    double z;

    float yaw;              //Rotation
    float pitch;
} CH_Camera;

//Memory Management Functions
extern CH_Camera* CH_Camera_Create(int view_width, int view_height);
extern void CH_Camera_Destroy(CH_Camera* camera);

//Methods
extern CH_Vector* CH_Camera_GetRelativeCoordinates(CH_Camera* camera, CH_Vector* v);
extern CH_Point* CH_Camera_Transform(CH_Camera* camera, CH_Vector* vec);
extern void CH_Camera_MoveRelative(CH_Camera* camera, double x, double y, double z);
extern void CH_Camera_RotateYaw(CH_Camera* camera, float amount);
extern void CH_Camera_RotatePitch(CH_Camera* camera, float amount);