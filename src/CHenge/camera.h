#pragma once

#include "vector.h"         //For vector operations
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
extern double CH_Camera_GetAvgDist(CH_Camera* camera, CH_Geometry* geometry);