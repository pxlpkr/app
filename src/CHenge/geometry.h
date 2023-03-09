#pragma once

#include "vector.h"

typedef struct {
    CH_Vector* vertices[3];
} CH_Geometry;

CH_Geometry* CH_Geometry_Create(CH_Vector* a, CH_Vector* b, CH_Vector* c);
void CH_Geometry_Destroy(CH_Geometry* geometry);