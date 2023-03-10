#pragma once

#include <stdlib.h>

#include "vector.h"

typedef struct {
    CH_Vector* vertices[3];
    u_int32_t color;
} CH_Geometry;

extern CH_Geometry* CH_Geometry_Create(CH_Vector* a, CH_Vector* b, CH_Vector* c);
extern void CH_Geometry_Destroy(CH_Geometry* geometry);